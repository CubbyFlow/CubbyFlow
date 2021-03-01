// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Searcher/PointSimpleListSearcher2.hpp>
#include <Core/Utils/FlatbuffersHelper.hpp>

#include <Flatbuffers/generated/PointSimpleListSearcher2_generated.h>

namespace CubbyFlow
{
PointSimpleListSearcher2::PointSimpleListSearcher2(
    const PointSimpleListSearcher2& other)
{
    Set(other);
}

PointSimpleListSearcher2& PointSimpleListSearcher2::operator=(
    const PointSimpleListSearcher2& other)
{
    Set(other);
    return *this;
}

void PointSimpleListSearcher2::Build(const ConstArrayView1<Vector2D>& points)
{
    m_points.resize(points.Length());
    std::copy(points.data(), points.data() + points.Length(), m_points.begin());
}

void PointSimpleListSearcher2::ForEachNearbyPoint(
    const Vector2D& origin, double radius,
    const ForEachNearbyPointFunc& callback) const
{
    const double radiusSquared = radius * radius;

    for (size_t i = 0; i < m_points.size(); ++i)
    {
        Vector2D r = m_points[i] - origin;
        const double distanceSquared = r.Dot(r);
        if (distanceSquared <= radiusSquared)
        {
            callback(i, m_points[i]);
        }
    }
}

bool PointSimpleListSearcher2::HasNearbyPoint(const Vector2D& origin,
                                              double radius) const
{
    const double radiusSquared = radius * radius;

    for (const auto& point : m_points)
    {
        Vector2D r = point - origin;
        const double distanceSquared = r.Dot(r);
        if (distanceSquared <= radiusSquared)
        {
            return true;
        }
    }

    return false;
}

PointNeighborSearcher2Ptr PointSimpleListSearcher2::Clone() const
{
    return std::shared_ptr<PointSimpleListSearcher2>(
        new PointSimpleListSearcher2{ *this },
        [](PointSimpleListSearcher2* obj) { delete obj; });
}

void PointSimpleListSearcher2::Set(const PointSimpleListSearcher2& other)
{
    m_points = other.m_points;
}

void PointSimpleListSearcher2::Serialize(std::vector<uint8_t>* buffer) const
{
    flatbuffers::FlatBufferBuilder builder{ 1024 };

    // Copy points
    std::vector<fbs::Vector2D> points;
    for (const auto& pt : m_points)
    {
        points.emplace_back(CubbyFlowToFlatbuffers(pt));
    }

    const flatbuffers::Offset<flatbuffers::Vector<const fbs::Vector2D*>>
        fbsPoints = builder.CreateVectorOfStructs(points.data(), points.size());

    // Copy the searcher
    const flatbuffers::Offset<fbs::PointSimpleListSearcher2> fbsSearcher =
        CreatePointSimpleListSearcher2(builder, fbsPoints);

    builder.Finish(fbsSearcher);

    uint8_t* buf = builder.GetBufferPointer();
    const size_t size = builder.GetSize();

    buffer->resize(size);
    memcpy(buffer->data(), buf, size);
}

void PointSimpleListSearcher2::Deserialize(const std::vector<uint8_t>& buffer)
{
    const fbs::PointSimpleListSearcher2* fbsSearcher =
        fbs::GetPointSimpleListSearcher2(buffer.data());

    // Copy points
    const flatbuffers::Vector<const fbs::Vector2D*>* fbsPoints =
        fbsSearcher->points();
    m_points.resize(fbsPoints->size());
    for (uint32_t i = 0; i < fbsPoints->size(); ++i)
    {
        m_points[i] = FlatbuffersToCubbyFlow(*fbsPoints->Get(i));
    }
}

PointSimpleListSearcher2::Builder PointSimpleListSearcher2::GetBuilder()
{
    return Builder{};
}

PointSimpleListSearcher2 PointSimpleListSearcher2::Builder::Build()
{
    return PointSimpleListSearcher2{};
}

PointSimpleListSearcher2Ptr PointSimpleListSearcher2::Builder::MakeShared()
{
    return std::shared_ptr<PointSimpleListSearcher2>(
        new PointSimpleListSearcher2{},
        [](PointSimpleListSearcher2* obj) { delete obj; });
}

PointNeighborSearcher2Ptr
PointSimpleListSearcher2::Builder::BuildPointNeighborSearcher() const
{
    return MakeShared();
}
}  // namespace CubbyFlow