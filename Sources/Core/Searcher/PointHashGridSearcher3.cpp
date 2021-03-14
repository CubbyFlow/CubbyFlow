// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Searcher/PointHashGridSearcher3.hpp>
#include <Core/Utils/FlatbuffersHelper.hpp>

#include <Flatbuffers/generated/PointHashGridSearcher3_generated.h>

namespace CubbyFlow
{
PointHashGridSearcher3::PointHashGridSearcher3(const Vector3UZ& resolution,
                                               double gridSpacing)
    : PointHashGridSearcher3{ resolution.x, resolution.y, resolution.z,
                              gridSpacing }
{
    // Do nothing
}

PointHashGridSearcher3::PointHashGridSearcher3(size_t resolutionX,
                                               size_t resolutionY,
                                               size_t resolutionZ,
                                               double gridSpacing)
    : m_gridSpacing(gridSpacing)
{
    m_resolution.x =
        std::max(static_cast<ssize_t>(resolutionX), static_cast<ssize_t>(1));
    m_resolution.y =
        std::max(static_cast<ssize_t>(resolutionY), static_cast<ssize_t>(1));
    m_resolution.z =
        std::max(static_cast<ssize_t>(resolutionZ), static_cast<ssize_t>(1));
}

PointHashGridSearcher3::PointHashGridSearcher3(
    const PointHashGridSearcher3& other)
{
    Set(other);
}

PointHashGridSearcher3& PointHashGridSearcher3::operator=(
    const PointHashGridSearcher3& other)
{
    Set(other);
    return *this;
}

void PointHashGridSearcher3::Build(const ConstArrayView1<Vector3D>& points)
{
    m_buckets.clear();
    m_points.clear();

    // Allocate memory chunks
    m_buckets.resize(m_resolution.x * m_resolution.y * m_resolution.z);
    m_points.resize(points.Length());

    if (m_points.empty())
    {
        return;
    }

    // Put points into buckets
    for (size_t i = 0; i < points.Length(); ++i)
    {
        m_points[i] = points[i];
        const size_t key = GetHashKeyFromPosition(points[i]);
        m_buckets[key].push_back(i);
    }
}

void PointHashGridSearcher3::ForEachNearbyPoint(
    const Vector3D& origin, double radius,
    const ForEachNearbyPointFunc& callback) const
{
    if (m_buckets.empty())
    {
        return;
    }

    size_t nearbyKeys[8];
    GetNearbyKeys(origin, nearbyKeys);

    const double queryRadiusSquared = radius * radius;

    for (const auto& key : nearbyKeys)
    {
        const std::vector<size_t>& bucket = m_buckets[key];
        const size_t numberOfPointsInBucket = bucket.size();

        for (size_t j = 0; j < numberOfPointsInBucket; ++j)
        {
            const size_t pointIndex = bucket[j];
            const double rSquared =
                (m_points[pointIndex] - origin).LengthSquared();
            if (rSquared <= queryRadiusSquared)
            {
                callback(pointIndex, m_points[pointIndex]);
            }
        }
    }
}

bool PointHashGridSearcher3::HasNearbyPoint(const Vector3D& origin,
                                            double radius) const
{
    if (m_buckets.empty())
    {
        return false;
    }

    size_t nearbyKeys[8];
    GetNearbyKeys(origin, nearbyKeys);

    const double queryRadiusSquared = radius * radius;

    for (const auto& key : nearbyKeys)
    {
        const std::vector<size_t>& bucket = m_buckets[key];
        const size_t numberOfPointsInBucket = bucket.size();

        for (size_t j = 0; j < numberOfPointsInBucket; ++j)
        {
            const size_t pointIndex = bucket[j];
            const double rSquared =
                (m_points[pointIndex] - origin).LengthSquared();
            if (rSquared <= queryRadiusSquared)
            {
                return true;
            }
        }
    }

    return false;
}

void PointHashGridSearcher3::Add(const Vector3D& point)
{
    if (m_buckets.empty())
    {
        Array1<Vector3D> arr = { point };
        Build(arr);
    }
    else
    {
        size_t i = m_points.size();
        m_points.push_back(point);
        size_t key = GetHashKeyFromPosition(point);
        m_buckets[key].push_back(i);
    }
}

const std::vector<std::vector<size_t>>& PointHashGridSearcher3::GetBuckets()
    const
{
    return m_buckets;
}

size_t PointHashGridSearcher3::GetHashKeyFromBucketIndex(
    const Vector3Z& bucketIndex) const
{
    Vector3Z wrappedIndex;
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

Vector3Z PointHashGridSearcher3::GetBucketIndex(const Vector3D& position) const
{
    Vector3Z bucketIndex;
    bucketIndex.x =
        static_cast<ssize_t>(std::floor(position.x / m_gridSpacing));
    bucketIndex.y =
        static_cast<ssize_t>(std::floor(position.y / m_gridSpacing));
    bucketIndex.z =
        static_cast<ssize_t>(std::floor(position.z / m_gridSpacing));

    return bucketIndex;
}

PointNeighborSearcher3Ptr PointHashGridSearcher3::Clone() const
{
    return std::shared_ptr<PointNeighborSearcher3>(
        new PointHashGridSearcher3{ *this },
        [](PointHashGridSearcher3* obj) { delete obj; });
}

void PointHashGridSearcher3::Set(const PointHashGridSearcher3& other)
{
    m_resolution = other.m_resolution;
    m_gridSpacing = other.m_gridSpacing;
    m_points = other.m_points;
    m_buckets = other.m_buckets;
}

void PointHashGridSearcher3::Serialize(std::vector<uint8_t>* buffer) const
{
    flatbuffers::FlatBufferBuilder builder{ 1024 };

    // Copy simple data
    auto fbsResolution =
        fbs::Vector3UZ(m_resolution.x, m_resolution.y, m_resolution.z);

    // Copy points
    std::vector<fbs::Vector3D> points;
    for (const auto& pt : m_points)
    {
        points.push_back(CubbyFlowToFlatbuffers(pt));
    }

    const flatbuffers::Offset<flatbuffers::Vector<const fbs::Vector3D*>>
        fbsPoints = builder.CreateVectorOfStructs(points.data(), points.size());

    // Copy buckets
    std::vector<flatbuffers::Offset<fbs::PointHashGridSearcherBucket3>> buckets;
    for (const auto& bucket : m_buckets)
    {
        std::vector<uint64_t> bucket64{ bucket.begin(), bucket.end() };
        flatbuffers::Offset<fbs::PointHashGridSearcherBucket3> fbsBucket =
            fbs::CreatePointHashGridSearcherBucket3(
                builder,
                builder.CreateVector(bucket64.data(), bucket64.size()));
        buckets.push_back(fbsBucket);
    }

    const flatbuffers::Offset<flatbuffers::Vector<
        flatbuffers::Offset<fbs::PointHashGridSearcherBucket3>>>
        fbsBuckets = builder.CreateVector(buckets);

    // Copy the searcher
    const flatbuffers::Offset<fbs::PointHashGridSearcher3> fbsSearcher =
        CreatePointHashGridSearcher3(builder, m_gridSpacing, &fbsResolution,
                                     fbsPoints, fbsBuckets);

    builder.Finish(fbsSearcher);

    uint8_t* buf = builder.GetBufferPointer();
    const size_t size = builder.GetSize();

    buffer->resize(size);
    memcpy(buffer->data(), buf, size);
}

void PointHashGridSearcher3::Deserialize(const std::vector<uint8_t>& buffer)
{
    const fbs::PointHashGridSearcher3* fbsSearcher =
        fbs::GetPointHashGridSearcher3(buffer.data());

    // Copy simple data
    const Vector3UZ res = FlatbuffersToCubbyFlow(*fbsSearcher->resolution());
    m_resolution = { static_cast<ssize_t>(res.x), static_cast<ssize_t>(res.y),
                     static_cast<ssize_t>(res.z) };
    m_gridSpacing = fbsSearcher->gridSpacing();

    // Copy points
    const flatbuffers::Vector<const fbs::Vector3D*>* fbsPoints =
        fbsSearcher->points();
    m_points.resize(fbsPoints->size());
    for (uint32_t i = 0; i < fbsPoints->size(); ++i)
    {
        m_points[i] = FlatbuffersToCubbyFlow(*fbsPoints->Get(i));
    }

    // Copy buckets
    const flatbuffers::Vector<
        flatbuffers::Offset<fbs::PointHashGridSearcherBucket3>>* fbsBuckets =
        fbsSearcher->buckets();
    m_buckets.resize(fbsBuckets->size());
    for (uint32_t i = 0; i < fbsBuckets->size(); ++i)
    {
        const flatbuffers::Vector<
            flatbuffers::Offset<fbs::PointHashGridSearcherBucket3>>::return_type
            fbsBucket = fbsBuckets->Get(i);
        m_buckets[i].resize(fbsBucket->data()->size());
        std::transform(fbsBucket->data()->begin(), fbsBucket->data()->end(),
                       m_buckets[i].begin(),
                       [](uint64_t val) { return static_cast<size_t>(val); });
    }
}

PointHashGridSearcher3::Builder PointHashGridSearcher3::GetBuilder()
{
    return Builder{};
}

size_t PointHashGridSearcher3::GetHashKeyFromPosition(
    const Vector3D& position) const
{
    const Vector3Z bucketIndex = GetBucketIndex(position);
    return GetHashKeyFromBucketIndex(bucketIndex);
}

void PointHashGridSearcher3::GetNearbyKeys(const Vector3D& position,
                                           size_t* nearbyKeys) const
{
    const Vector3Z originIndex = GetBucketIndex(position);
    Vector3Z nearbyBucketIndices[8];

    for (auto& bucketIndex : nearbyBucketIndices)
    {
        bucketIndex = originIndex;
    }

    if ((static_cast<double>(originIndex.x) + 0.5) * m_gridSpacing <=
        position.x)
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

    if ((static_cast<double>(originIndex.y) + 0.5) * m_gridSpacing <=
        position.y)
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

    if ((static_cast<double>(originIndex.z) + 0.5) * m_gridSpacing <=
        position.z)
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

PointHashGridSearcher3::Builder&
PointHashGridSearcher3::Builder::WithResolution(const Vector3UZ& resolution)
{
    m_resolution = resolution;
    return *this;
}

PointHashGridSearcher3::Builder&
PointHashGridSearcher3::Builder::WithGridSpacing(double gridSpacing)
{
    m_gridSpacing = gridSpacing;
    return *this;
}

PointHashGridSearcher3 PointHashGridSearcher3::Builder::Build() const
{
    return PointHashGridSearcher3{ m_resolution, m_gridSpacing };
}

PointHashGridSearcher3Ptr PointHashGridSearcher3::Builder::MakeShared() const
{
    return std::shared_ptr<PointHashGridSearcher3>(
        new PointHashGridSearcher3(m_resolution, m_gridSpacing),
        [](PointHashGridSearcher3* obj) { delete obj; });
}

PointNeighborSearcher3Ptr
PointHashGridSearcher3::Builder::BuildPointNeighborSearcher() const
{
    return MakeShared();
}
}  // namespace CubbyFlow