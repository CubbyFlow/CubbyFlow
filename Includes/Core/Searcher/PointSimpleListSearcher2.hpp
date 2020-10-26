// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_POINT_SIMPLE_LIST_SEARCHER2_HPP
#define CUBBYFLOW_POINT_SIMPLE_LIST_SEARCHER2_HPP

#include <Core/Searcher/PointNeighborSearcher2.hpp>

namespace CubbyFlow
{
//!
//! \brief Simple ad-hoc 2-D point searcher.
//!
//! This class implements 2-D point searcher simply by looking up every point in
//! the list. Thus, this class is not ideal for searches involving large number
//! of points, but only for small set of items.
//!
class PointSimpleListSearcher2 final : public PointNeighborSearcher2
{
 public:
    CUBBYFLOW_NEIGHBOR_SEARCHER2_TYPE_NAME(PointSimpleListSearcher2)

    class Builder;

    //! Default constructor.
    PointSimpleListSearcher2() = default;

    //! Copy constructor.
    PointSimpleListSearcher2(const PointSimpleListSearcher2& other);

    //! Default move constructor.
    PointSimpleListSearcher2(PointSimpleListSearcher2&&) noexcept = default;

    //! Default virtual destructor.
    ~PointSimpleListSearcher2() override = default;

    //! Copy assignment operator.
    PointSimpleListSearcher2& operator=(const PointSimpleListSearcher2& other);

    //! Move assignment operator.
    PointSimpleListSearcher2& operator=(PointSimpleListSearcher2&&) noexcept =
        default;

    //!
    //! \brief      Builds internal structure for given points list.
    //!
    //! For this class, this function simply copies the given point list to the
    //! internal list.
    //!
    //! \param[in]  points The points to search.
    //!
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
    [[nodiscard]] bool HasNearbyPoint(const Vector2D& origin,
                                      double radius) const override;

    //!
    //! \brief      Creates a new instance of the object with same properties
    //!             than original.
    //!
    //! \return     Copy of this object.
    //!
    [[nodiscard]] PointNeighborSearcher2Ptr Clone() const override;

    //! Copy from the other instance.
    void Set(const PointSimpleListSearcher2& other);

    //! Serializes the neighbor searcher into the buffer.
    void Serialize(std::vector<uint8_t>* buffer) const override;

    //! Deserializes the neighbor searcher from the buffer.
    void Deserialize(const std::vector<uint8_t>& buffer) override;

    //! Returns builder fox PointSimpleListSearcher2.
    [[nodiscard]] static Builder GetBuilder();

 private:
    std::vector<Vector2D> m_points;
};

//! Shared pointer for the PointSimpleListSearcher2 type.
using PointSimpleListSearcher2Ptr = std::shared_ptr<PointSimpleListSearcher2>;

//!
//! \brief Front-end to create PointSimpleListSearcher2 objects step by step.
//!
class PointSimpleListSearcher2::Builder final
    : public PointNeighborSearcherBuilder2
{
 public:
    //! Builds PointSimpleListSearcher2 instance.
    [[nodiscard]] static PointSimpleListSearcher2 Build();

    //! Builds shared pointer of PointSimpleListSearcher2 instance.
    [[nodiscard]] static PointSimpleListSearcher2Ptr MakeShared();

    //! Returns shared pointer of PointNeighborSearcher3 type.
    [[nodiscard]] PointNeighborSearcher2Ptr BuildPointNeighborSearcher()
        const override;
};
}  // namespace CubbyFlow

#endif