/*************************************************************************
> File Name: PointHashGridSearcher3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Hash grid-based 3-D point searcher.
> Created Time: 2017/05/23
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_POINT_HASH_GRID_SEARCHER3_H
#define CUBBYFLOW_POINT_HASH_GRID_SEARCHER3_H

#include <Searcher/PointNeighborSearcher3.h>
#include <Size/Size3.h>

#include <vector>

namespace CubbyFlow
{
	//!
	//! \brief Hash grid-based 3-D point searcher.
	//!
	//! This class implements 3-D point searcher by using hash grid for its internal
	//! acceleration data structure. Each point is recorded to its corresponding
	//! bucket where the hashing function is 3-D grid mapping.
	//!
	class PointHashGridSearcher3 final : public PointNeighborSearcher3
	{
	public:
		CUBBYFLOW_NEIGHBOR_SEARCHER3_TYPE_NAME(PointHashGridSearcher3)

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
		PointHashGridSearcher3(const Size3& resolution, double gridSpacing);

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
		PointHashGridSearcher3(size_t resolutionX, size_t resolutionY, size_t resolutionZ, double gridSpacing);

		//! Copy constructor.
		PointHashGridSearcher3(const PointHashGridSearcher3& other);

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
			const Vector3D& origin,
			double radius,
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
		//! \brief      Adds a single point to the hash grid.
		//!
		//! This function adds a single point to the hash grid for future queries.
		//! It can be used for a hash grid that is already built by calling function
		//! PointHashGridSearcher3::build.
		//!
		//! \param[in]  point The point to be added.
		//!
		void Add(const Vector3D& point);

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
		//! Returns the hash value for given 3-D bucket index.
		//!
		//! \param[in]  bucketIndex The bucket index.
		//!
		//! \return     The hash key from bucket index.
		//!
		size_t GetHashKeyFromBucketIndex(const Point3I& bucketIndex) const;

		//!
		//! Gets the bucket index from a point.
		//!
		//! \param[in]  position The position of the point.
		//!
		//! \return     The bucket index.
		//!
		Point3I GetBucketIndex(const Vector3D& position) const;

		//!
		//! \brief      Creates a new instance of the object with same properties
		//!             than original.
		//!
		//! \return     Copy of this object.
		//!
		PointNeighborSearcher3Ptr Clone() const override;

		//! Assignment operator.
		PointHashGridSearcher3& operator=(const PointHashGridSearcher3& other);

		//! Copy from the other instance.
		void Set(const PointHashGridSearcher3& other);

		//! Serializes the neighbor searcher into the buffer.
		void Serialize(std::vector<uint8_t>* buffer) const override;

		//! Deserializes the neighbor searcher from the buffer.
		void Deserialize(const std::vector<uint8_t>& buffer) override;

		//! Returns builder fox PointHashGridSearcher3.
		static Builder GetBuilder();

	private:
		double m_gridSpacing = 1.0;
		Point3I m_resolution = Point3I(1, 1, 1);
		std::vector<Vector3D> m_points;
		std::vector<std::vector<size_t>> m_buckets;

		size_t GetHashKeyFromPosition(const Vector3D& position) const;

		void GetNearbyKeys(const Vector3D& position, size_t* nearbyKeys) const;
	};

	//! Shared pointer for the PointHashGridSearcher3 type.
	using PointHashGridSearcher3Ptr = std::shared_ptr<PointHashGridSearcher3>;

	//!
	//! \brief Front-end to create PointHashGridSearcher3 objects step by step.
	//!
	class PointHashGridSearcher3::Builder final : public PointNeighborSearcherBuilder3
	{
	public:
		//! Returns builder with resolution.
		Builder& WithResolution(const Size3& resolution);

		//! Returns builder with grid spacing.
		Builder& WithGridSpacing(double gridSpacing);

		//! Builds PointHashGridSearcher3 instance.
		PointHashGridSearcher3 Build() const;

		//! Builds shared pointer of PointHashGridSearcher3 instance.
		PointHashGridSearcher3Ptr MakeShared() const;

		//! Returns shared pointer of PointHashGridSearcher3 type.
		PointNeighborSearcher3Ptr BuildPointNeighborSearcher() const override;

	private:
		Size3 m_resolution{ 64, 64, 64 };
		double m_gridSpacing = 1.0;
	};
}

#endif