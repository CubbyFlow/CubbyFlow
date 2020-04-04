// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_POINT_NEIGHBOR_SEARCHER3_HPP
#define CUBBYFLOW_POINT_NEIGHBOR_SEARCHER3_HPP

#include <Core/Utils/Serialization.hpp>
#include <Core/Vector/Vector3.hpp>

#include <functional>
#include <memory>
#include <string>

namespace CubbyFlow
{
//!
//! \brief Abstract base class for 3-D neighbor point searcher.
//!
//! This class provides interface for 3-D neighbor point searcher. For given
//! list of points, the class builds internal cache to accelerate the search.
//! Once built, the data structure is used to search nearby points for given
//! origin point.
//!
class PointNeighborSearcher3 : public Serializable
{
 public:
    //! Callback function for nearby search query. The first parameter is the
    //! index of the nearby point, and the second is the position of the point.
    using ForEachNearbyPointFunc = std::function<void(size_t, const Vector3D&)>;

    //! Default constructor.
    PointNeighborSearcher3();

    //! Destructor.
    virtual ~PointNeighborSearcher3();

    //! Returns the type name of the derived class.
    virtual std::string TypeName() const = 0;

    //! Builds internal acceleration structure for given points list.
    virtual void Build(const ConstArrayAccessor1<Vector3D>& points) = 0;

    //!
    //! Invokes the callback function for each nearby point around the origin
    //! within given radius.
    //!
    //! \param[in]  origin   The origin position.
    //! \param[in]  radius   The search radius.
    //! \param[in]  callback The callback function.
    //!
    virtual void ForEachNearbyPoint(
        const Vector3D& origin, double radius,
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
    virtual bool HasNearbyPoint(const Vector3D& origin,
                                double radius) const = 0;

    //!
    //! \brief      Creates a new instance of the object with same properties
    //!             than original.
    //!
    //! \return     Copy of this object.
    //!
    virtual std::shared_ptr<PointNeighborSearcher3> Clone() const = 0;
};

//! Shared pointer for the PointNeighborSearcher3 type.
using PointNeighborSearcher3Ptr = std::shared_ptr<PointNeighborSearcher3>;

//! Abstract base class for 3-D point neighbor searcher builders.
class PointNeighborSearcherBuilder3
{
 public:
    virtual ~PointNeighborSearcherBuilder3();

    //! Returns shared pointer of PointNeighborSearcher3 type.
    virtual PointNeighborSearcher3Ptr BuildPointNeighborSearcher() const = 0;
};

//! Shared pointer for the PointNeighborSearcherBuilder3 type.
using PointNeighborSearcherBuilder3Ptr =
    std::shared_ptr<PointNeighborSearcherBuilder3>;

#define CUBBYFLOW_NEIGHBOR_SEARCHER3_TYPE_NAME(DerivedClassName) \
    std::string TypeName() const override                        \
    {                                                            \
        return #DerivedClassName;                                \
    }
}  // namespace CubbyFlow

#endif