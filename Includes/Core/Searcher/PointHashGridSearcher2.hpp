// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_POINT_HASH_GRID_SEARCHER2_HPP
#define CUBBYFLOW_POINT_HASH_GRID_SEARCHER2_HPP

#include <Core/Searcher/PointNeighborSearcher2.hpp>
#include <Core/Size/Size2.hpp>

#include <vector>

namespace CubbyFlow
{
//!
//! \brief Hash grid-based 2-D point searcher.
//!
//! This class implements 2-D point searcher by using hash grid for its internal
//! acceleration data structure. Each point is recorded to its corresponding
//! bucket where the hashing function is 2-D grid mapping.
//!
class PointHashGridSearcher2 final : public PointNeighborSearcher2
{
 public:
    CUBBYFLOW_NEIGHBOR_SEARCHER2_TYPE_NAME(PointHashGridSearcher2)

    class Builder;

    //!
    //! \brief      Constructs hash grid with given resolution and grid spacing.
    //!
    //! This constructor takes hash grid resolution and its grid spacing as
    //! its input parameters. The grid spacing must be 2x or greater than
    //! search radius.
    //!
    //! \param[in]  resolution  The resolution.
    //! \param[in]  gridSpacing The grid spacing.
    //!
    PointHashGridSearcher2(const Size2& resolution, double gridSpacing);

    //!
    //! \brief      Constructs hash grid with given resolution and grid spacing.
    //!
    //! This constructor takes hash grid resolution and its grid spacing as
    //! its input parameters. The grid spacing must be 2x or greater than
    //! search radius.
    //!
    //! \param[in]  resolutionX The resolution x.
    //! \param[in]  resolutionY The resolution y.
    //! \param[in]  gridSpacing The grid spacing.
    //!
    PointHashGridSearcher2(size_t resolutionX, size_t resolutionY,
                           double gridSpacing);

    //! Copy constructor.
    PointHashGridSearcher2(const PointHashGridSearcher2& other);

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
    //! \brief      Adds a single point to the hash grid.
    //!
    //! This function adds a single point to the hash grid for future queries.
    //! It can be used for a hash grid that is already built by calling function
    //! PointHashGridSearcher2::build.
    //!
    //! \param[in]  point The point to be added.
    //!
    void Add(const Vector2D& point);

    //!
    //! \brief      Returns the internal bucket.
    //!
    //! A bucket is a list of point indices that has same hash value. This
    //! function returns the (immutable) internal bucket structure.
    //!
    //! \return     List of buckets.
    //!
    const std::vector<std::vector<size_t>>& GetBuckets() const;

    //!
    //! Returns the hash value for given 2-D bucket index.
    //!
    //! \param[in]  bucketIndex The bucket index.
    //!
    //! \return     The hash key from bucket index.
    //!
    size_t GetHashKeyFromBucketIndex(const Point2I& bucketIndex) const;

    //!
    //! Gets the bucket index from a point.
    //!
    //! \param[in]  position The position of the point.
    //!
    //! \return     The bucket index.
    //!
    Point2I GetBucketIndex(const Vector2D& position) const;

    //!
    //! \brief      Creates a new instance of the object with same properties
    //!             than original.
    //!
    //! \return     Copy of this object.
    //!
    PointNeighborSearcher2Ptr Clone() const override;

    //! Assignment operator.
    PointHashGridSearcher2& operator=(const PointHashGridSearcher2& other);

    //! Copy from the other instance.
    void Set(const PointHashGridSearcher2& other);

    //! Serializes the neighbor searcher into the buffer.
    void Serialize(std::vector<uint8_t>* buffer) const override;

    //! Deserializes the neighbor searcher from the buffer.
    void Deserialize(const std::vector<uint8_t>& buffer) override;

    //! Returns builder fox PointHashGridSearcher2.
    static Builder GetBuilder();

 private:
    double m_gridSpacing = 1.0;
    Point2I m_resolution = Point2I(1, 1);
    std::vector<Vector2D> m_points;
    std::vector<std::vector<size_t>> m_buckets;

    size_t GetHashKeyFromPosition(const Vector2D& position) const;

    void GetNearbyKeys(const Vector2D& position, size_t* nearbyKeys) const;
};

//! Shared pointer for the PointHashGridSearcher2 type.
using PointHashGridSearcher2Ptr = std::shared_ptr<PointHashGridSearcher2>;

//!
//! \brief Front-end to create PointHashGridSearcher2 objects step by step.
//!
class PointHashGridSearcher2::Builder final
    : public PointNeighborSearcherBuilder2
{
 public:
    //! Returns builder with resolution.
    Builder& WithResolution(const Size2& resolution);

    //! Returns builder with grid spacing.
    Builder& WithGridSpacing(double gridSpacing);

    //! Builds PointHashGridSearcher2 instance.
    PointHashGridSearcher2 Build() const;

    //! Builds shared pointer of PointHashGridSearcher2 instance.
    PointHashGridSearcher2Ptr MakeShared() const;

    //! Returns shared pointer of PointNeighborSearcher2 type.
    PointNeighborSearcher2Ptr BuildPointNeighborSearcher() const override;

 private:
    Size2 m_resolution{ 64, 64 };
    double m_gridSpacing = 1.0;
};
}  // namespace CubbyFlow

#endif