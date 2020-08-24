// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_POINT_KDTREE_SEARCHER3_HPP
#define CUBBYFLOW_POINT_KDTREE_SEARCHER3_HPP

#include <Core/Geometry/KdTree.hpp>
#include <Core/Searcher/PointNeighborSearcher3.hpp>

namespace CubbyFlow
{
//!
//! \brief KdTree-based 3-D point searcher.
//!
//! This class implements 3-D point searcher by using KdTree for its internal
//! acceleration data structure.
//!
class PointKdTreeSearcher3 final : public PointNeighborSearcher3
{
 public:
    CUBBYFLOW_NEIGHBOR_SEARCHER3_TYPE_NAME(PointKdTreeSearcher3)

    class Builder;

    //! Constructs an empty kD-tree instance.
    PointKdTreeSearcher3();

    //! Copy constructor.
    PointKdTreeSearcher3(const PointKdTreeSearcher3& other);

    //! Builds internal acceleration structure for given points list.
    void Build(const ConstArrayAccessor1<Vector3D>& points) override;

    //!
    //! Invokes the callback function for each nearby point around the origin
    //! within given radius.
    //!
    //! \param[in]  origin   The origin position.
    //! \param[in]  radius   The search radius.
    //! \param[in]  callback The callback function.
    //!
    void ForEachNearbyPoint(
        const Vector3D& origin, double radius,
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
    bool HasNearbyPoint(const Vector3D& origin, double radius) const override;

    //!
    //! \brief      Creates a new instance of the object with same properties
    //!             than original.
    //!
    //! \return     Copy of this object.
    //!
    PointNeighborSearcher3Ptr Clone() const override;

    //! Assignment operator.
    PointKdTreeSearcher3& operator=(const PointKdTreeSearcher3& other);

    //! Copy from the other instance.
    void Set(const PointKdTreeSearcher3& other);

    //! Serializes the neighbor searcher into the buffer.
    void Serialize(std::vector<uint8_t>* buffer) const override;

    //! Deserializes the neighbor searcher from the buffer.
    void Deserialize(const std::vector<uint8_t>& buffer) override;

    //! Returns builder fox PointKdTreeSearcher3.
    static Builder GetBuilder();

 private:
    KdTree<double, 3> m_tree;
};

//! Shared pointer for the PointKdTreeSearcher3 type.
using PointKdTreeSearcher3Ptr = std::shared_ptr<PointKdTreeSearcher3>;

//!
//! \brief Front-end to create PointKdTreeSearcher3 objects step by step.
//!
class PointKdTreeSearcher3::Builder final : public PointNeighborSearcherBuilder3
{
 public:
    //! Builds PointKdTreeSearcher3 instance.
    PointKdTreeSearcher3 Build() const;

    //! Builds shared pointer of PointKdTreeSearcher3 instance.
    PointKdTreeSearcher3Ptr MakeShared() const;

    //! Returns shared pointer of PointNeighborSearcher3 type.
    PointNeighborSearcher3Ptr BuildPointNeighborSearcher() const override;
};
}  // namespace CubbyFlow

#endif