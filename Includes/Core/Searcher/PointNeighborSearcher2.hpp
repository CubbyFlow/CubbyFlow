// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_POINT_NEIGHBOR_SEARCHER2_HPP
#define CUBBYFLOW_POINT_NEIGHBOR_SEARCHER2_HPP

#include <Core/Matrix/Matrix.hpp>
#include <Core/Utils/Serialization.hpp>

#include <functional>
#include <memory>
#include <string>

namespace CubbyFlow
{
//!
//! \brief Abstract base class for 2-D neighbor point searcher.
//!
//! This class provides interface for 2-D neighbor point searcher. For given
//! list of points, the class builds internal cache to accelerate the search.
//! Once built, the data structure is used to search nearby points for given
//! origin point.
//!
class PointNeighborSearcher2 : public Serializable
{
 public:
    //! Callback function for nearby search query. The first parameter is the
    //! index of the nearby point, and the second is the position of the point.
    using ForEachNearbyPointFunc = std::function<void(size_t, const Vector2D&)>;

    //! Default constructor.
    PointNeighborSearcher2() = default;

    //! Default copy constructor.
    PointNeighborSearcher2(const PointNeighborSearcher2&) = default;

    //! Default move constructor.
    PointNeighborSearcher2(PointNeighborSearcher2&&) noexcept = default;

    //! Default virtual destructor.
    ~PointNeighborSearcher2() override = default;

    //! Default copy assignment operator.
    PointNeighborSearcher2& operator=(const PointNeighborSearcher2&) = default;

    //! Default move assignment operator.
    PointNeighborSearcher2& operator=(PointNeighborSearcher2&&) noexcept =
        default;

    //! Returns the type name of the derived class.
    [[nodiscard]] virtual std::string TypeName() const = 0;

    //! Builds internal acceleration structure for given points list.
    virtual void Build(const ConstArrayView1<Vector2D>& points) = 0;

    //!
    //! Invokes the callback function for each nearby point around the origin
    //! within given radius.
    //!
    //! \param[in]  origin   The origin position.
    //! \param[in]  radius   The search radius.
    //! \param[in]  callback The callback function.
    //!
    virtual void ForEachNearbyPoint(
        const Vector2D& origin, double radius,
        const ForEachNearbyPointFunc& callback) const = 0;

    //!
    //! Returns true if there are any nearby points for given origin within
    //! radius.
    //!
    //! \param[in]  origin The origin.
    //! \param[in]  radius The radius.
    //!
    //! \return     True if has nearby point, false otherwise.
    //!
    [[nodiscard]] virtual bool HasNearbyPoint(const Vector2D& origin,
                                              double radius) const = 0;

    //!
    //! \brief      Creates a new instance of the object with same properties
    //!             than original.
    //!
    //! \return     Copy of this object.
    //!
    [[nodiscard]] virtual std::shared_ptr<PointNeighborSearcher2> Clone()
        const = 0;
};

//! Shared pointer for the PointNeighborSearcher2 type.
using PointNeighborSearcher2Ptr = std::shared_ptr<PointNeighborSearcher2>;

//! Abstract base class for 2-D point neighbor searcher builders.
class PointNeighborSearcherBuilder2
{
 public:
    //! Default constructor.
    PointNeighborSearcherBuilder2() = default;

    //! Default copy constructor.
    PointNeighborSearcherBuilder2(const PointNeighborSearcherBuilder2&) =
        default;

    //! Default move constructor.
    PointNeighborSearcherBuilder2(PointNeighborSearcherBuilder2&&) noexcept =
        default;

    //! Default virtual destructor.
    virtual ~PointNeighborSearcherBuilder2() = default;

    //! Default copy assignment operator.
    PointNeighborSearcherBuilder2& operator=(
        const PointNeighborSearcherBuilder2&) = default;

    //! Default move assignment operator.
    PointNeighborSearcherBuilder2& operator=(
        PointNeighborSearcherBuilder2&&) noexcept = default;

    //! Returns shared pointer of PointNeighborSearcher2 type.
    [[nodiscard]] virtual PointNeighborSearcher2Ptr BuildPointNeighborSearcher()
        const = 0;
};

//! Shared pointer for the PointNeighborSearcherBuilder2 type.
using PointNeighborSearcherBuilder2Ptr =
    std::shared_ptr<PointNeighborSearcherBuilder2>;

#define CUBBYFLOW_NEIGHBOR_SEARCHER2_TYPE_NAME(DerivedClassName) \
    std::string TypeName() const override                        \
    {                                                            \
        return #DerivedClassName;                                \
    }
}  // namespace CubbyFlow

#endif