// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_POINT_KDTREE_SEARCHER2_HPP
#define CUBBYFLOW_POINT_KDTREE_SEARCHER2_HPP

#include <Core/Geometry/KdTree.hpp>
#include <Core/Searcher/PointNeighborSearcher2.hpp>

namespace CubbyFlow
{
//!
//! \brief KdTree-based 2-D point searcher.
//!
//! This class implements 2-D point searcher by using KdTree for its internal
//! acceleration data structure.
//!
class PointKdTreeSearcher2 final : public PointNeighborSearcher2
{
 public:
    CUBBYFLOW_NEIGHBOR_SEARCHER2_TYPE_NAME(PointKdTreeSearcher2)

    class Builder;

    //! Constructs an empty kD-tree instance.
    PointKdTreeSearcher2();

    //! Copy constructor.
    PointKdTreeSearcher2(const PointKdTreeSearcher2& other);

    //! Builds internal acceleration structure for given points list.
    void Build(const ConstArrayAccessor1<Vector2D>& points) override;

    //!
    //! Invokes the callback function for each nearby point around the origin
    //! within given radius.
    //!
    //! \param[in]  origin   The origin position.
    //! \param[in]  radius   The search radius.
    //! \param[in]  callback The callback function.
    //!
    void ForEachNearbyPoint(
        const Vector2D& origin, double radius,
        const ForEachNearbyPointFunc& callback) const override;

    //!
    //! Returns true if there are any nearby points for given origin within
    //! radius.
    //!
    //! \param[in]  origin The origin.
    //! \param[in]  radius The radius.
    //!
    //! \return     True if has nearby point, false otherwise.
    //!
    bool HasNearbyPoint(const Vector2D& origin, double radius) const override;

    //!
    //! \brief      Creates a new instance of the object with same properties
    //!             than original.
    //!
    //! \return     Copy of this object.
    //!
    PointNeighborSearcher2Ptr Clone() const override;

    //! Assignment operator.
    PointKdTreeSearcher2& operator=(const PointKdTreeSearcher2& other);

    //! Copy from the other instance.
    void Set(const PointKdTreeSearcher2& other);

    //! Serializes the neighbor searcher into the buffer.
    void Serialize(std::vector<uint8_t>* buffer) const override;

    //! Deserializes the neighbor searcher from the buffer.
    void Deserialize(const std::vector<uint8_t>& buffer) override;

    //! Returns builder fox PointKdTreeSearcher2.
    static Builder GetBuilder();

 private:
    KdTree<double, 2> m_tree;
};

//! Shared pointer for the PointKdTreeSearcher2 type.
using PointKdTreeSearcher2Ptr = std::shared_ptr<PointKdTreeSearcher2>;

//!
//! \brief Front-end to create PointKdTreeSearcher2 objects step by step.
//!
class PointKdTreeSearcher2::Builder final : public PointNeighborSearcherBuilder2
{
 public:
    //! Builds PointKdTreeSearcher2 instance.
    PointKdTreeSearcher2 Build() const;

    //! Builds shared pointer of PointKdTreeSearcher2 instance.
    PointKdTreeSearcher2Ptr MakeShared() const;

    //! Returns shared pointer of PointNeighborSearcher3 type.
    PointNeighborSearcher2Ptr BuildPointNeighborSearcher() const override;
};
}  // namespace CubbyFlow

#endif