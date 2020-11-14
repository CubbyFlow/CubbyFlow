// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_POINT_PARALLEL_HASH_GRID_SEARCHER3_HPP
#define CUBBYFLOW_POINT_PARALLEL_HASH_GRID_SEARCHER3_HPP

#include <Core/Searcher/PointNeighborSearcher3.hpp>
#include <Core/Size/Size3.hpp>

namespace CubbyFlow
{
//!
//! \brief Parallel version of hash grid-based 3-D point searcher.
//!
//! This class implements parallel version of 3-D point searcher by using hash
//! grid for its internal acceleration data structure. Each point is recorded to
//! its corresponding bucket where the hashing function is 3-D grid mapping.
//!
class PointParallelHashGridSearcher3 final : public PointNeighborSearcher3
{
 public:
    CUBBYFLOW_NEIGHBOR_SEARCHER3_TYPE_NAME(PointParallelHashGridSearcher3)

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
    PointParallelHashGridSearcher3(const Size3& resolution, double gridSpacing);

    //!
    //! \brief      Constructs hash grid with given resolution and grid spacing.
    //!
    //! This constructor takes hash grid resolution and its grid spacing as
    //! its input parameters. The grid spacing must be 2x or greater than
    //! search radius.
    //!
    //! \param[in]  resolutionX The resolution x.
    //! \param[in]  resolutionY The resolution y.
    //! \param[in]  resolutionZ The resolution z.
    //! \param[in]  gridSpacing The grid spacing.
    //!
    PointParallelHashGridSearcher3(size_t resolutionX, size_t resolutionY,
                                   size_t resolutionZ, double gridSpacing);

    //! Copy constructor.
    PointParallelHashGridSearcher3(const PointParallelHashGridSearcher3& other);

    //! Default move constructor.
    PointParallelHashGridSearcher3(PointParallelHashGridSearcher3&&) noexcept =
        default;

    //! Default virtual destructor.
    ~PointParallelHashGridSearcher3() override = default;

    //! Copy assignment operator.
    PointParallelHashGridSearcher3& operator=(
        const PointParallelHashGridSearcher3& other);

    //! Move assignment operator.
    PointParallelHashGridSearcher3& operator=(
        PointParallelHashGridSearcher3&&) noexcept = default;

    //!
    //! \brief Builds internal acceleration structure for given points list.
    //!
    //! This function builds the hash grid for given points in parallel.
    //!
    //! \param[in]  points The points to be added.
    //!
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
    [[nodiscard]] bool HasNearbyPoint(const Vector3D& origin,
                                      double radius) const override;

    //!
    //! \brief      Returns the hash key list.
    //!
    //! The hash key list maps sorted point index i to its hash key value.
    //! The sorting order is based on the key value itself.
    //!
    //! \return     The hash key list.
    //!
    [[nodiscard]] const std::vector<size_t>& Keys() const;

    //!
    //! \brief      Returns the start index table.
    //!
    //! The start index table maps the hash grid bucket index to starting index
    //! of the sorted point list. Assume the hash key list looks like:
    //!
    //! \code
    //! [5|8|8|10|10|10]
    //! \endcode
    //!
    //! Then startIndexTable and endIndexTable should be like:
    //!
    //! \code
    //! [.....|0|...|1|..|3|..]
    //! [.....|1|...|3|..|6|..]
    //!       ^5    ^8   ^10
    //! \endcode
    //!
    //! So that endIndexTable[i] - startIndexTable[i] is the number points
    //! in i-th table bucket.
    //!
    //! \return     The start index table.
    //!
    [[nodiscard]] const std::vector<size_t>& StartIndexTable() const;

    //!
    //! \brief      Returns the end index table.
    //!
    //! The end index table maps the hash grid bucket index to starting index
    //! of the sorted point list. Assume the hash key list looks like:
    //!
    //! \code
    //! [5|8|8|10|10|10]
    //! \endcode
    //!
    //! Then startIndexTable and endIndexTable should be like:
    //!
    //! \code
    //! [.....|0|...|1|..|3|..]
    //! [.....|1|...|3|..|6|..]
    //!       ^5    ^8   ^10
    //! \endcode
    //!
    //! So that endIndexTable[i] - startIndexTable[i] is the number points
    //! in i-th table bucket.
    //!
    //! \return     The end index table.
    //!
    [[nodiscard]] const std::vector<size_t>& EndIndexTable() const;

    //!
    //! \brief      Returns the sorted indices of the points.
    //!
    //! When the hash grid is built, it sorts the points in hash key order. But
    //! rather than sorting the original points, this class keeps the shuffled
    //! indices of the points. The list this function returns maps sorted index
    //! i to original index j.
    //!
    //! \return     The sorted indices of the points.
    //!
    [[nodiscard]] const std::vector<size_t>& SortedIndices() const;

    //!
    //! Returns the hash value for given 3-D bucket index.
    //!
    //! \param[in]  bucketIndex The bucket index.
    //!
    //! \return     The hash key from bucket index.
    //!
    [[nodiscard]] size_t GetHashKeyFromBucketIndex(
        const Point3I& bucketIndex) const;

    //!
    //! Gets the bucket index from a point.
    //!
    //! \param[in]  position The position of the point.
    //!
    //! \return     The bucket index.
    //!
    [[nodiscard]] Point3I GetBucketIndex(const Vector3D& position) const;

    //!
    //! \brief      Creates a new instance of the object with same properties
    //!             than original.
    //!
    //! \return     Copy of this object.
    //!
    [[nodiscard]] PointNeighborSearcher3Ptr Clone() const override;

    //! Copy from the other instance.
    void Set(const PointParallelHashGridSearcher3& other);

    //! Serializes the neighbor searcher into the buffer.
    void Serialize(std::vector<uint8_t>* buffer) const override;

    //! Deserializes the neighbor searcher from the buffer.
    void Deserialize(const std::vector<uint8_t>& buffer) override;

    //! Returns builder fox PointParallelHashGridSearcher3.
    [[nodiscard]] static Builder GetBuilder();

 private:
    [[nodiscard]] size_t GetHashKeyFromPosition(const Vector3D& position) const;

    void GetNearbyKeys(const Vector3D& position, size_t* nearbyKeys) const;

    double m_gridSpacing = 1.0;
    Point3I m_resolution = Point3I{ 1, 1, 1 };
    std::vector<Vector3D> m_points;
    std::vector<size_t> m_keys;
    std::vector<size_t> m_startIndexTable;
    std::vector<size_t> m_endIndexTable;
    std::vector<size_t> m_sortedIndices;
};

//! Shared pointer for the PointParallelHashGridSearcher3 type.
using PointParallelHashGridSearcher3Ptr =
    std::shared_ptr<PointParallelHashGridSearcher3>;

//!
//! \brief Front-end to create PointParallelHashGridSearcher3 objects step by
//! step.
//!
class PointParallelHashGridSearcher3::Builder final
    : public PointNeighborSearcherBuilder3
{
 public:
    //! Returns builder with resolution.
    [[nodiscard]] Builder& WithResolution(const Size3& resolution);

    //! Returns builder with grid spacing.
    [[nodiscard]] Builder& WithGridSpacing(double gridSpacing);

    //! Builds PointParallelHashGridSearcher3 instance.
    [[nodiscard]] PointParallelHashGridSearcher3 Build() const;

    //! Builds shared pointer of PointParallelHashGridSearcher3 instance.
    [[nodiscard]] PointParallelHashGridSearcher3Ptr MakeShared() const;

    //! Returns shared pointer of PointNeighborSearcher3 type.
    [[nodiscard]] PointNeighborSearcher3Ptr BuildPointNeighborSearcher()
        const override;

 private:
    Size3 m_resolution{ 64, 64, 64 };
    double m_gridSpacing = 1.0;
};
}  // namespace CubbyFlow

#endif