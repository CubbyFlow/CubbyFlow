// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Utils/FlatbuffersHelper.h>
#include <Core/Utils/Logging.h>
#include <Core/Utils/Parallel.h>
#include <Core/Searcher/PointParallelHashGridSearcher3.hpp>

#include <Flatbuffers/generated/PointParallelHashGridSearcher3_generated.h>

#include <flatbuffers/flatbuffers.h>

namespace CubbyFlow
{
PointParallelHashGridSearcher3::PointParallelHashGridSearcher3(
    const Size3& resolution, double gridSpacing)
    : PointParallelHashGridSearcher3(resolution.x, resolution.y, resolution.z,
                                     gridSpacing)
{
    // Do nothing
}

PointParallelHashGridSearcher3::PointParallelHashGridSearcher3(
    size_t resolutionX, size_t resolutionY, size_t resolutionZ,
    double gridSpacing)
    : m_gridSpacing(gridSpacing)
{
    m_resolution.x = std::max(static_cast<ssize_t>(resolutionX), ONE_SSIZE);
    m_resolution.y = std::max(static_cast<ssize_t>(resolutionY), ONE_SSIZE);
    m_resolution.z = std::max(static_cast<ssize_t>(resolutionZ), ONE_SSIZE);

    m_startIndexTable.resize(m_resolution.x * m_resolution.y * m_resolution.z,
                             std::numeric_limits<size_t>::max());
    m_endIndexTable.resize(m_resolution.x * m_resolution.y * m_resolution.z,
                           std::numeric_limits<size_t>::max());
}

PointParallelHashGridSearcher3::PointParallelHashGridSearcher3(
    const PointParallelHashGridSearcher3& other)
{
    Set(other);
}

void PointParallelHashGridSearcher3::Build(
    const ConstArrayAccessor1<Vector3D>& points)
{
    m_points.clear();
    m_keys.clear();
    m_startIndexTable.clear();
    m_endIndexTable.clear();
    m_sortedIndices.clear();

    // Allocate memory chunks
    size_t numberOfPoints = points.size();
    std::vector<size_t> tempKeys(numberOfPoints);
    m_startIndexTable.resize(m_resolution.x * m_resolution.y * m_resolution.z);
    m_endIndexTable.resize(m_resolution.x * m_resolution.y * m_resolution.z);
    ParallelFill(m_startIndexTable.begin(), m_startIndexTable.end(),
                 std::numeric_limits<size_t>::max());
    ParallelFill(m_endIndexTable.begin(), m_endIndexTable.end(),
                 std::numeric_limits<size_t>::max());
    m_keys.resize(numberOfPoints);
    m_sortedIndices.resize(numberOfPoints);
    m_points.resize(numberOfPoints);

    if (numberOfPoints == 0)
    {
        return;
    }

    // Initialize indices array and generate hash key for each point
    ParallelFor(ZERO_SIZE, numberOfPoints, [&](size_t i) {
        m_sortedIndices[i] = i;
        m_points[i] = points[i];
        tempKeys[i] = GetHashKeyFromPosition(points[i]);
    });

    // Sort indices based on hash key
    ParallelSort(m_sortedIndices.begin(), m_sortedIndices.end(),
                 [&tempKeys](size_t indexA, size_t indexB) {
                     return tempKeys[indexA] < tempKeys[indexB];
                 });

    // Re-order point and key arrays
    ParallelFor(ZERO_SIZE, numberOfPoints, [&](size_t i) {
        m_points[i] = points[m_sortedIndices[i]];
        m_keys[i] = tempKeys[m_sortedIndices[i]];
    });

    // Now m_points and m_keys are sorted by points' hash key values.
    // Let's fill in start/end index table with m_keys.

    // Assume that m_keys array looks like:
    // [5|8|8|10|10|10]
    // Then m_startIndexTable and m_endIndexTable should be like:
    // [.....|0|...|1|..|3|..]
    // [.....|1|...|3|..|6|..]
    //       ^5    ^8   ^10
    // So that m_endIndexTable[i] - m_startIndexTable[i] is the number points
    // in i-th table bucket.

    m_startIndexTable[m_keys[0]] = 0;
    m_endIndexTable[m_keys[numberOfPoints - 1]] = numberOfPoints;

    ParallelFor(static_cast<size_t>(1), numberOfPoints, [&](size_t i) {
        if (m_keys[i] > m_keys[i - 1])
        {
            m_startIndexTable[m_keys[i]] = i;
            m_endIndexTable[m_keys[i - 1]] = i;
        }
    });

    size_t sumNumberOfPointsPerBucket = 0;
    size_t maxNumberOfPointsPerBucket = 0;
    size_t numberOfNonEmptyBucket = 0;

    for (size_t i = 0; i < m_startIndexTable.size(); ++i)
    {
        if (m_startIndexTable[i] != std::numeric_limits<size_t>::max())
        {
            size_t numberOfPointsInBucket =
                m_endIndexTable[i] - m_startIndexTable[i];
            sumNumberOfPointsPerBucket += numberOfPointsInBucket;
            maxNumberOfPointsPerBucket =
                std::max(maxNumberOfPointsPerBucket, numberOfPointsInBucket);
            ++numberOfNonEmptyBucket;
        }
    }

    CUBBYFLOW_INFO << "Average number of points per non-empty bucket: "
                   << static_cast<float>(sumNumberOfPointsPerBucket) /
                          static_cast<float>(numberOfNonEmptyBucket);
    CUBBYFLOW_INFO << "Max number of points per bucket: "
                   << maxNumberOfPointsPerBucket;
}

void PointParallelHashGridSearcher3::ForEachNearbyPoint(
    const Vector3D& origin, double radius,
    const ForEachNearbyPointFunc& callback) const
{
    size_t nearbyKeys[8];
    GetNearbyKeys(origin, nearbyKeys);

    const double queryRadiusSquared = radius * radius;

    for (int i = 0; i < 8; ++i)
    {
        size_t nearbyKey = nearbyKeys[i];
        size_t start = m_startIndexTable[nearbyKey];
        size_t end = m_endIndexTable[nearbyKey];

        // Empty bucket -- continue to next bucket
        if (start == std::numeric_limits<size_t>::max())
        {
            continue;
        }

        for (size_t j = start; j < end; ++j)
        {
            Vector3D direction = m_points[j] - origin;
            double distanceSquared = direction.LengthSquared();
            if (distanceSquared <= queryRadiusSquared)
            {
                callback(m_sortedIndices[j], m_points[j]);
            }
        }
    }
}

bool PointParallelHashGridSearcher3::HasNearbyPoint(const Vector3D& origin,
                                                    double radius) const
{
    size_t nearbyKeys[8];
    GetNearbyKeys(origin, nearbyKeys);

    const double queryRadiusSquared = radius * radius;

    for (int i = 0; i < 8; ++i)
    {
        size_t nearbyKey = nearbyKeys[i];
        size_t start = m_startIndexTable[nearbyKey];
        size_t end = m_endIndexTable[nearbyKey];

        // Empty bucket -- continue to next bucket
        if (start == std::numeric_limits<size_t>::max())
        {
            continue;
        }

        for (size_t j = start; j < end; ++j)
        {
            Vector3D direction = m_points[j] - origin;
            double distanceSquared = direction.LengthSquared();
            if (distanceSquared <= queryRadiusSquared)
            {
                return true;
            }
        }
    }

    return false;
}

const std::vector<size_t>& PointParallelHashGridSearcher3::Keys() const
{
    return m_keys;
}

const std::vector<size_t>& PointParallelHashGridSearcher3::StartIndexTable()
    const
{
    return m_startIndexTable;
}

const std::vector<size_t>& PointParallelHashGridSearcher3::EndIndexTable() const
{
    return m_endIndexTable;
}

const std::vector<size_t>& PointParallelHashGridSearcher3::SortedIndices() const
{
    return m_sortedIndices;
}

Point3I PointParallelHashGridSearcher3::GetBucketIndex(
    const Vector3D& position) const
{
    Point3I bucketIndex;

    bucketIndex.x =
        static_cast<ssize_t>(std::floor(position.x / m_gridSpacing));
    bucketIndex.y =
        static_cast<ssize_t>(std::floor(position.y / m_gridSpacing));
    bucketIndex.z =
        static_cast<ssize_t>(std::floor(position.z / m_gridSpacing));

    return bucketIndex;
}

size_t PointParallelHashGridSearcher3::GetHashKeyFromPosition(
    const Vector3D& position) const
{
    Point3I bucketIndex = GetBucketIndex(position);
    return GetHashKeyFromBucketIndex(bucketIndex);
}

size_t PointParallelHashGridSearcher3::GetHashKeyFromBucketIndex(
    const Point3I& bucketIndex) const
{
    Point3I wrappedIndex;

    wrappedIndex.x = bucketIndex.x % m_resolution.x;
    wrappedIndex.y = bucketIndex.y % m_resolution.y;
    wrappedIndex.z = bucketIndex.z % m_resolution.z;

    if (wrappedIndex.x < 0)
    {
        wrappedIndex.x += m_resolution.x;
    }
    if (wrappedIndex.y < 0)
    {
        wrappedIndex.y += m_resolution.y;
    }
    if (wrappedIndex.z < 0)
    {
        wrappedIndex.z += m_resolution.z;
    }

    return static_cast<size_t>(
        (wrappedIndex.z * m_resolution.y + wrappedIndex.y) * m_resolution.x +
        wrappedIndex.x);
}

void PointParallelHashGridSearcher3::GetNearbyKeys(const Vector3D& position,
                                                   size_t* nearbyKeys) const
{
    Point3I originIndex = GetBucketIndex(position), nearbyBucketIndices[8];

    for (int i = 0; i < 8; ++i)
    {
        nearbyBucketIndices[i] = originIndex;
    }

    if ((originIndex.x + 0.5f) * m_gridSpacing <= position.x)
    {
        nearbyBucketIndices[4].x += 1;
        nearbyBucketIndices[5].x += 1;
        nearbyBucketIndices[6].x += 1;
        nearbyBucketIndices[7].x += 1;
    }
    else
    {
        nearbyBucketIndices[4].x -= 1;
        nearbyBucketIndices[5].x -= 1;
        nearbyBucketIndices[6].x -= 1;
        nearbyBucketIndices[7].x -= 1;
    }

    if ((originIndex.y + 0.5f) * m_gridSpacing <= position.y)
    {
        nearbyBucketIndices[2].y += 1;
        nearbyBucketIndices[3].y += 1;
        nearbyBucketIndices[6].y += 1;
        nearbyBucketIndices[7].y += 1;
    }
    else
    {
        nearbyBucketIndices[2].y -= 1;
        nearbyBucketIndices[3].y -= 1;
        nearbyBucketIndices[6].y -= 1;
        nearbyBucketIndices[7].y -= 1;
    }

    if ((originIndex.z + 0.5f) * m_gridSpacing <= position.z)
    {
        nearbyBucketIndices[1].z += 1;
        nearbyBucketIndices[3].z += 1;
        nearbyBucketIndices[5].z += 1;
        nearbyBucketIndices[7].z += 1;
    }
    else
    {
        nearbyBucketIndices[1].z -= 1;
        nearbyBucketIndices[3].z -= 1;
        nearbyBucketIndices[5].z -= 1;
        nearbyBucketIndices[7].z -= 1;
    }

    for (int i = 0; i < 8; ++i)
    {
        nearbyKeys[i] = GetHashKeyFromBucketIndex(nearbyBucketIndices[i]);
    }
}

PointNeighborSearcher3Ptr PointParallelHashGridSearcher3::Clone() const
{
    return std::shared_ptr<PointParallelHashGridSearcher3>(
        new PointParallelHashGridSearcher3(*this),
        [](PointParallelHashGridSearcher3* obj) { delete obj; });
}

PointParallelHashGridSearcher3& PointParallelHashGridSearcher3::operator=(
    const PointParallelHashGridSearcher3& other)
{
    Set(other);
    return *this;
}

void PointParallelHashGridSearcher3::Set(
    const PointParallelHashGridSearcher3& other)
{
    m_gridSpacing = other.m_gridSpacing;
    m_resolution = other.m_resolution;
    m_points = other.m_points;
    m_keys = other.m_keys;
    m_startIndexTable = other.m_startIndexTable;
    m_endIndexTable = other.m_endIndexTable;
    m_sortedIndices = other.m_sortedIndices;
}

void PointParallelHashGridSearcher3::Serialize(
    std::vector<uint8_t>* buffer) const
{
    flatbuffers::FlatBufferBuilder builder(1024);

    // Copy simple data
    auto fbsResolution =
        fbs::Size3(m_resolution.x, m_resolution.y, m_resolution.z);

    // Copy points
    std::vector<fbs::Vector3D> points;
    for (const auto& pt : m_points)
    {
        points.push_back(CubbyFlowToFlatbuffers(pt));
    }

    auto fbsPoints =
        builder.CreateVectorOfStructs(points.data(), points.size());

    // Copy key/tables
    std::vector<uint64_t> keys(m_keys.begin(), m_keys.end());
    std::vector<uint64_t> startIndexTable(m_startIndexTable.begin(),
                                          m_startIndexTable.end());
    std::vector<uint64_t> endIndexTable(m_endIndexTable.begin(),
                                        m_endIndexTable.end());
    std::vector<uint64_t> sortedIndices(m_sortedIndices.begin(),
                                        m_sortedIndices.end());

    auto fbsKeys = builder.CreateVector(keys.data(), keys.size());
    auto fbsStartIndexTable =
        builder.CreateVector(startIndexTable.data(), startIndexTable.size());
    auto fbsEndIndexTable =
        builder.CreateVector(endIndexTable.data(), endIndexTable.size());
    auto fbsSortedIndices =
        builder.CreateVector(sortedIndices.data(), sortedIndices.size());

    // Copy the searcher
    auto fbsSearcher = fbs::CreatePointParallelHashGridSearcher3(
        builder, m_gridSpacing, &fbsResolution, fbsPoints, fbsKeys,
        fbsStartIndexTable, fbsEndIndexTable, fbsSortedIndices);

    builder.Finish(fbsSearcher);

    uint8_t* buf = builder.GetBufferPointer();
    size_t size = builder.GetSize();

    buffer->resize(size);
    memcpy(buffer->data(), buf, size);
}

void PointParallelHashGridSearcher3::Deserialize(
    const std::vector<uint8_t>& buffer)
{
    auto fbsSearcher = fbs::GetPointParallelHashGridSearcher3(buffer.data());

    // Copy simple data
    auto res = FlatbuffersToCubbyFlow(*fbsSearcher->resolution());
    m_resolution.Set({ res.x, res.y, res.z });
    m_gridSpacing = fbsSearcher->gridSpacing();

    // Copy points
    auto fbsPoints = fbsSearcher->points();
    m_points.resize(fbsPoints->size());
    for (uint32_t i = 0; i < fbsPoints->size(); ++i)
    {
        m_points[i] = FlatbuffersToCubbyFlow(*fbsPoints->Get(i));
    }

    // Copy key/tables
    auto fbsKeys = fbsSearcher->keys();
    m_keys.resize(fbsKeys->size());
    for (uint32_t i = 0; i < fbsKeys->size(); ++i)
    {
        m_keys[i] = static_cast<size_t>(fbsKeys->Get(i));
    }

    auto fbsStartIndexTable = fbsSearcher->startIndexTable();
    m_startIndexTable.resize(fbsStartIndexTable->size());
    for (uint32_t i = 0; i < fbsStartIndexTable->size(); ++i)
    {
        m_startIndexTable[i] = static_cast<size_t>(fbsStartIndexTable->Get(i));
    }

    auto fbsEndIndexTable = fbsSearcher->endIndexTable();
    m_endIndexTable.resize(fbsEndIndexTable->size());
    for (uint32_t i = 0; i < fbsEndIndexTable->size(); ++i)
    {
        m_endIndexTable[i] = static_cast<size_t>(fbsEndIndexTable->Get(i));
    }

    auto fbsSortedIndices = fbsSearcher->sortedIndices();
    m_sortedIndices.resize(fbsSortedIndices->size());
    for (uint32_t i = 0; i < fbsSortedIndices->size(); ++i)
    {
        m_sortedIndices[i] = static_cast<size_t>(fbsSortedIndices->Get(i));
    }
}

PointParallelHashGridSearcher3::Builder
PointParallelHashGridSearcher3::GetBuilder()
{
    return Builder();
}

PointParallelHashGridSearcher3::Builder&
PointParallelHashGridSearcher3::Builder::WithResolution(const Size3& resolution)
{
    m_resolution = resolution;
    return *this;
}

PointParallelHashGridSearcher3::Builder&
PointParallelHashGridSearcher3::Builder::WithGridSpacing(double gridSpacing)
{
    m_gridSpacing = gridSpacing;
    return *this;
}

PointParallelHashGridSearcher3 PointParallelHashGridSearcher3::Builder::Build()
    const
{
    return PointParallelHashGridSearcher3(m_resolution, m_gridSpacing);
}

PointParallelHashGridSearcher3Ptr
PointParallelHashGridSearcher3::Builder::MakeShared() const
{
    return std::shared_ptr<PointParallelHashGridSearcher3>(
        new PointParallelHashGridSearcher3(m_resolution, m_gridSpacing),
        [](PointParallelHashGridSearcher3* obj) { delete obj; });
}

PointNeighborSearcher3Ptr
PointParallelHashGridSearcher3::Builder::BuildPointNeighborSearcher() const
{
    return MakeShared();
}
}  // namespace CubbyFlow