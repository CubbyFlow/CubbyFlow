// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Searcher/PointKdTreeSearcher2.hpp>
#include <Core/Utils/FlatbuffersHelper.hpp>
#include <Core/Utils/Logging.hpp>
#include <Core/Utils/Parallel.hpp>

#include <Flatbuffers/generated/PointKdTreeSearcher2_generated.h>

#include <flatbuffers/flatbuffers.h>

namespace CubbyFlow
{
PointKdTreeSearcher2::PointKdTreeSearcher2(const PointKdTreeSearcher2& other)
{
    Set(other);
}

PointKdTreeSearcher2& PointKdTreeSearcher2::operator=(
    const PointKdTreeSearcher2& other)
{
    Set(other);
    return *this;
}

void PointKdTreeSearcher2::Build(const ConstArrayAccessor1<Vector2D>& points)
{
    m_tree.Build(points);
}

void PointKdTreeSearcher2::ForEachNearbyPoint(
    const Vector2D& origin, double radius,
    const ForEachNearbyPointFunc& callback) const
{
    m_tree.ForEachNearbyPoint(origin, radius, callback);
}

bool PointKdTreeSearcher2::HasNearbyPoint(const Vector2D& origin,
                                          double radius) const
{
    return m_tree.HasNearbyPoint(origin, radius);
}

PointNeighborSearcher2Ptr PointKdTreeSearcher2::Clone() const
{
    return std::shared_ptr<PointKdTreeSearcher2>(
        new PointKdTreeSearcher2{ *this },
        [](PointKdTreeSearcher2* obj) { delete obj; });
}

void PointKdTreeSearcher2::Set(const PointKdTreeSearcher2& other)
{
    m_tree = other.m_tree;
}

void PointKdTreeSearcher2::Serialize(std::vector<uint8_t>* buffer) const
{
    flatbuffers::FlatBufferBuilder builder{ 1024 };

    // Copy points
    std::vector<fbs::Vector2D> points;
    for (const auto& iter : m_tree)
    {
        points.push_back(CubbyFlowToFlatbuffers(iter));
    }

    const flatbuffers::Offset<flatbuffers::Vector<const fbs::Vector2D*>>
        fbsPoints = builder.CreateVectorOfStructs(points.data(), points.size());

    // Copy nodes
    std::vector<fbs::PointKdTreeSearcherNode2> nodes;
    for (auto iter = m_tree.BeginNode(); iter != m_tree.EndNode(); ++iter)
    {
        nodes.emplace_back(iter->flags, iter->child, iter->item);
    }

    const flatbuffers::Offset<
        flatbuffers::Vector<const fbs::PointKdTreeSearcherNode2*>>
        fbsNodes = builder.CreateVectorOfStructs(nodes);

    // Copy the searcher
    const flatbuffers::Offset<fbs::PointKdTreeSearcher2> fbsSearcher =
        CreatePointKdTreeSearcher2(builder, fbsPoints, fbsNodes);

    // Finish
    builder.Finish(fbsSearcher);

    uint8_t* buf = builder.GetBufferPointer();
    const size_t size = builder.GetSize();

    buffer->resize(size);
    memcpy(buffer->data(), buf, size);
}

void PointKdTreeSearcher2::Deserialize(const std::vector<uint8_t>& buffer)
{
    const fbs::PointKdTreeSearcher2* fbsSearcher =
        fbs::GetPointKdTreeSearcher2(buffer.data());

    const flatbuffers::Vector<const fbs::Vector2D*>* fbsPoints =
        fbsSearcher->points();
    const flatbuffers::Vector<const fbs::PointKdTreeSearcherNode2*>* fbsNodes =
        fbsSearcher->nodes();

    m_tree.Reserve(fbsPoints->size(), fbsNodes->size());

    // Copy points
    const auto pointsIter = m_tree.begin();
    for (uint32_t i = 0; i < fbsPoints->size(); ++i)
    {
        pointsIter[i] = FlatbuffersToCubbyFlow(*fbsPoints->Get(i));
    }

    // Copy nodes
    const auto nodesIter = m_tree.BeginNode();
    for (uint32_t i = 0; i < fbsNodes->size(); ++i)
    {
        const flatbuffers::Vector<
            const fbs::PointKdTreeSearcherNode2*>::return_type fbsNode =
            fbsNodes->Get(i);

        nodesIter[i].flags = fbsNode->flags();
        nodesIter[i].child = fbsNode->child();
        nodesIter[i].item = fbsNode->item();
        nodesIter[i].point = pointsIter[fbsNode->item()];
    }
}

PointKdTreeSearcher2::Builder PointKdTreeSearcher2::GetBuilder()
{
    return Builder{};
}

PointKdTreeSearcher2 PointKdTreeSearcher2::Builder::Build()
{
    return PointKdTreeSearcher2{};
}

PointKdTreeSearcher2Ptr PointKdTreeSearcher2::Builder::MakeShared()
{
    return std::shared_ptr<PointKdTreeSearcher2>(
        new PointKdTreeSearcher2,
        [](PointKdTreeSearcher2* obj) { delete obj; });
}

PointNeighborSearcher2Ptr
PointKdTreeSearcher2::Builder::BuildPointNeighborSearcher() const
{
    return MakeShared();
}
}  // namespace CubbyFlow