// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Searcher/PointKdTreeSearcher3.hpp>
#include <Core/Utils/FlatbuffersHelper.hpp>
#include <Core/Utils/Logging.hpp>
#include <Core/Utils/Parallel.hpp>

#include <Flatbuffers/generated/PointKdTreeSearcher3_generated.h>

#include <flatbuffers/flatbuffers.h>

namespace CubbyFlow
{
PointKdTreeSearcher3::PointKdTreeSearcher3(const PointKdTreeSearcher3& other)
{
    Set(other);
}

PointKdTreeSearcher3& PointKdTreeSearcher3::operator=(
    const PointKdTreeSearcher3& other)
{
    Set(other);
    return *this;
}

void PointKdTreeSearcher3::Build(const ConstArrayView1<Vector3D>& points)
{
    m_tree.Build(points);
}

void PointKdTreeSearcher3::ForEachNearbyPoint(
    const Vector3D& origin, double radius,
    const ForEachNearbyPointFunc& callback) const
{
    m_tree.ForEachNearbyPoint(origin, radius, callback);
}

bool PointKdTreeSearcher3::HasNearbyPoint(const Vector3D& origin,
                                          double radius) const
{
    return m_tree.HasNearbyPoint(origin, radius);
}

PointNeighborSearcher3Ptr PointKdTreeSearcher3::Clone() const
{
    return std::shared_ptr<PointKdTreeSearcher3>(
        new PointKdTreeSearcher3{ *this },
        [](PointKdTreeSearcher3* obj) { delete obj; });
}

void PointKdTreeSearcher3::Set(const PointKdTreeSearcher3& other)
{
    m_tree = other.m_tree;
}

void PointKdTreeSearcher3::Serialize(std::vector<uint8_t>* buffer) const
{
    flatbuffers::FlatBufferBuilder builder{ 1024 };

    // Copy points
    std::vector<fbs::Vector3D> points;
    for (const auto& iter : m_tree)
    {
        points.push_back(CubbyFlowToFlatbuffers(iter));
    }

    const flatbuffers::Offset<flatbuffers::Vector<const fbs::Vector3D*>>
        fbsPoints = builder.CreateVectorOfStructs(points.data(), points.size());

    // Copy nodes
    std::vector<fbs::PointKdTreeSearcherNode3> nodes;
    for (auto iter = m_tree.BeginNode(); iter != m_tree.EndNode(); ++iter)
    {
        nodes.emplace_back(iter->flags, iter->child, iter->item);
    }

    const flatbuffers::Offset<
        flatbuffers::Vector<const fbs::PointKdTreeSearcherNode3*>>
        fbsNodes = builder.CreateVectorOfStructs(nodes);

    // Copy the searcher
    const flatbuffers::Offset<fbs::PointKdTreeSearcher3> fbsSearcher =
        CreatePointKdTreeSearcher3(builder, fbsPoints, fbsNodes);

    // Finish
    builder.Finish(fbsSearcher);

    uint8_t* buf = builder.GetBufferPointer();
    const size_t size = builder.GetSize();

    buffer->resize(size);
    memcpy(buffer->data(), buf, size);
}

void PointKdTreeSearcher3::Deserialize(const std::vector<uint8_t>& buffer)
{
    const fbs::PointKdTreeSearcher3* fbsSearcher =
        fbs::GetPointKdTreeSearcher3(buffer.data());

    const flatbuffers::Vector<const fbs::Vector3D*>* fbsPoints =
        fbsSearcher->points();
    const flatbuffers::Vector<const fbs::PointKdTreeSearcherNode3*>* fbsNodes =
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
            const fbs::PointKdTreeSearcherNode3*>::return_type fbsNode =
            fbsNodes->Get(i);

        nodesIter[i].flags = fbsNode->flags();
        nodesIter[i].child = fbsNode->child();
        nodesIter[i].item = fbsNode->item();
        nodesIter[i].point = pointsIter[fbsNode->item()];
    }
}

PointKdTreeSearcher3::Builder PointKdTreeSearcher3::GetBuilder()
{
    return Builder{};
}

PointKdTreeSearcher3 PointKdTreeSearcher3::Builder::Build()
{
    return PointKdTreeSearcher3{};
}

PointKdTreeSearcher3Ptr PointKdTreeSearcher3::Builder::MakeShared()
{
    return std::shared_ptr<PointKdTreeSearcher3>(
        new PointKdTreeSearcher3,
        [](PointKdTreeSearcher3* obj) { delete obj; });
}

PointNeighborSearcher3Ptr
PointKdTreeSearcher3::Builder::BuildPointNeighborSearcher() const
{
    return MakeShared();
}
}  // namespace CubbyFlow