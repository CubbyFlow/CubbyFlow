// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_POINT_NEIGHBOR_SEARCHER_HPP
#define CUBBYFLOW_POINT_NEIGHBOR_SEARCHER_HPP

#include <Core/Matrix/Matrix.hpp>
#include <Core/Utils/Serialization.hpp>

#include <functional>
#include <memory>
#include <string>

namespace CubbyFlow
{
//!
//! \brief Abstract base class for N-D neighbor point searcher.
//!
//! This class provides interface for N-D neighbor point searcher. For given
//! list of points, the class builds internal cache to accelerate the search.
//! Once built, the data structure is used to search nearby points for given
//! origin point.
//!
template <size_t N>
class PointNeighborSearcher : public Serializable
{
 public:
    //! Callback function for nearby search query. The first parameter is the
    //! index of the nearby point, and the second is the position of the point.
    using ForEachNearbyPointFunc =
        std::function<void(size_t, const Vector<double, N>&)>;

    //! Default constructor.
    PointNeighborSearcher() = default;

    //! Default virtual destructor.
    ~PointNeighborSearcher() override = default;

    //! Default copy constructor.
    PointNeighborSearcher(const PointNeighborSearcher& other) = default;

    //! Default move constructor.
    PointNeighborSearcher(PointNeighborSearcher&& other) noexcept = default;

    //! Default copy assignment operator.
    PointNeighborSearcher& operator=(const PointNeighborSearcher& other) =
        default;

    //! Default move assignment operator.
    PointNeighborSearcher& operator=(PointNeighborSearcher&& other) noexcept =
        default;

    //! Returns the type name of the derived class.
    [[nodiscard]] virtual std::string TypeName() const = 0;

    //! Builds internal acceleration structure for given points list.
    virtual void Build(const ConstArrayView1<Vector<double, N>>& points);

    //! Builds internal acceleration structure for given points list and max
    //! search radius.
    virtual void Build(const ConstArrayView1<Vector<double, N>>& points,
                       double maxSearchRadius) = 0;

    //!
    //! Invokes the callback function for each nearby point around the origin
    //! within given radius.
    //!
    //! \param[in]  origin   The origin position.
    //! \param[in]  radius   The search radius.
    //! \param[in]  callback The callback function.
    //!
    virtual void ForEachNearbyPoint(
        const Vector<double, N>& origin, double radius,
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
    [[nodiscard]] virtual bool HasNearbyPoint(const Vector<double, N>& origin,
                                              double radius) const = 0;

    //!
    //! \brief      Creates a new instance of the object with same properties
    //!             than original.
    //!
    //! \return     Copy of this object.
    //!
    [[nodiscard]] virtual std::shared_ptr<PointNeighborSearcher> Clone()
        const = 0;
};

//! 2-D PointNeighborSearcher type.
using PointNeighborSearcher2 = PointNeighborSearcher<2>;

//! 3-D PointNeighborSearcher type.
using PointNeighborSearcher3 = PointNeighborSearcher<3>;

//! Shared pointer for the PointNeighborSearcher2 type.
using PointNeighborSearcher2Ptr = std::shared_ptr<PointNeighborSearcher2>;

//! Shared pointer for the PointNeighborSearcher3 type.
using PointNeighborSearcher3Ptr = std::shared_ptr<PointNeighborSearcher3>;

//! Abstract base class for N-D point neighbor searcher builders.
template <size_t N>
class PointNeighborSearcherBuilder
{
 public:
    //! Default constructor.
    PointNeighborSearcherBuilder() = default;

    //! Default virtual destructor.
    virtual ~PointNeighborSearcherBuilder() = default;

    //! Deleted copy constructor.
    PointNeighborSearcherBuilder(const PointNeighborSearcherBuilder& other) =
        delete;

    //! Deleted move constructor.
    PointNeighborSearcherBuilder(
        PointNeighborSearcherBuilder&& other) noexcept = delete;

    //! Deleted copy assignment operator.
    PointNeighborSearcherBuilder& operator=(
        const PointNeighborSearcherBuilder& other) = delete;

    //! Deleted move assignment operator.
    PointNeighborSearcherBuilder& operator=(
        PointNeighborSearcherBuilder&& other) noexcept = delete;

    //! Returns shared pointer of PointNeighborSearcher type.
    [[nodiscard]] virtual std::shared_ptr<PointNeighborSearcher<N>>
    BuildPointNeighborSearcher() const = 0;
};

//! 2-D PointNeighborSearcherBuilder type.
using PointNeighborSearcherBuilder2 = PointNeighborSearcherBuilder<2>;

//! 3-D PointNeighborSearcherBuilder type.
using PointNeighborSearcherBuilder3 = PointNeighborSearcherBuilder<3>;

//! Shared pointer for the PointNeighborSearcher2 type.
using PointNeighborSearcherBuilder2Ptr =
    std::shared_ptr<PointNeighborSearcherBuilder2>;

//! Shared pointer for the PointNeighborSearcher3 type.
using PointNeighborSearcherBuilder3Ptr =
    std::shared_ptr<PointNeighborSearcherBuilder3>;

#define CUBBYFLOW_NEIGHBOR_SEARCHER_TYPE_NAME(DerivedClassName, N) \
    [[nodiscard]] std::string TypeName() const override            \
    {                                                              \
        return #DerivedClassName + std::to_string(N);              \
    }
}  // namespace CubbyFlow

#endif