/*************************************************************************
> File Name: PointHashGridSearcher2.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Hash grid-based 2-D point searcher.
> Created Time: 2017/05/23
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#include <Searcher/PointHashGridSearcher2.h>
#include <Utils/FlatbuffersHelper.h>

#include <Flatbuffers/generated/PointHashGridSearcher2_generated.h>

namespace CubbyFlow
{
	PointHashGridSearcher2::PointHashGridSearcher2(const Size2& resolution, double gridSpacing) :
		PointHashGridSearcher2(resolution.x, resolution.y, gridSpacing)
	{
		// Do nothing
	}

	PointHashGridSearcher2::PointHashGridSearcher2(size_t resolutionX, size_t resolutionY, double gridSpacing) :
		m_gridSpacing(gridSpacing)
	{
		m_resolution.x = std::max(static_cast<ssize_t>(resolutionX), static_cast<ssize_t>(1));
		m_resolution.y = std::max(static_cast<ssize_t>(resolutionY), static_cast<ssize_t>(1));
	}

	PointHashGridSearcher2::PointHashGridSearcher2(const PointHashGridSearcher2& other)
	{
		Set(other);
	}

	void PointHashGridSearcher2::Build(const ConstArrayAccessor1<Vector2D>& points)
	{
		m_buckets.clear();
		m_points.clear();

		// Allocate memory chunks
		m_buckets.resize(m_resolution.x * m_resolution.y);
		m_points.resize(points.size());

		if (m_points.size() == 0)
		{
			return;
		}

		// Put points into buckets
		for (size_t i = 0; i < points.size(); ++i)
		{
			m_points[i] = points[i];
			size_t key = GetHashKeyFromPosition(points[i]);
			m_buckets[key].push_back(i);
		}
	}

	void PointHashGridSearcher2::ForEachNearbyPoint(
		const Vector2D& origin,
		double radius,
		const ForEachNearbyPointFunc& callback) const
	{
		if (m_buckets.empty())
		{
			return;
		}

		size_t nearByKeys[4];
		GetNearbyKeys(origin, nearByKeys);

		const double queryRadiusSquared = radius * radius;

		for (size_t i = 0; i < 4; ++i)
		{
			const auto& bucket = m_buckets[nearByKeys[i]];
			size_t numberOfPointsInBucket = bucket.size();

			for (size_t j = 0; j < numberOfPointsInBucket; ++j)
			{
				size_t pointIndex = bucket[j];
				double rSquared = (m_points[pointIndex] - origin).LengthSquared();
				if (rSquared <= queryRadiusSquared)
				{
					callback(pointIndex, m_points[pointIndex]);
				}
			}
		}
	}

	bool PointHashGridSearcher2::HasNearbyPoint(const Vector2D& origin, double radius) const
	{
		if (m_buckets.empty())
		{
			return false;
		}

		size_t nearbyKeys[4];
		GetNearbyKeys(origin, nearbyKeys);

		const double queryRadiusSquared = radius * radius;
		
		for (size_t i = 0; i < 4; ++i)
		{
			const auto& bucket = m_buckets[nearbyKeys[i]];
			size_t numberOfPointsInBucket = bucket.size();

			for (size_t j = 0; j < numberOfPointsInBucket; ++j)
			{
				size_t pointIndex = bucket[j];
				double rSquared = (m_points[pointIndex] - origin).LengthSquared();
				if (rSquared <= queryRadiusSquared)
				{
					return true;
				}
			}
		}

		return false;
	}

	void PointHashGridSearcher2::Add(const Vector2D& point)
	{
		if (m_buckets.empty())
		{
			Array1<Vector2D> arr = { point };
			Build(arr);
		}
		else
		{
			size_t i = m_points.size();
			m_points.push_back(point);
			size_t key = GetHashKeyFromPosition(point);
			m_buckets[key].push_back(i);
		}
	}

	const std::vector<std::vector<size_t>>& PointHashGridSearcher2::GetBuckets() const
	{
		return m_buckets;
	}

	size_t PointHashGridSearcher2::GetHashKeyFromBucketIndex(const Point2I& bucketIndex) const
	{
		Point2I wrappedIndex;
		wrappedIndex.x = bucketIndex.x % m_resolution.x;
		wrappedIndex.y = bucketIndex.y % m_resolution.y;
		
		if (wrappedIndex.x < 0)
		{
			wrappedIndex.x += m_resolution.x;
		}
		
		if (wrappedIndex.y < 0)
		{
			wrappedIndex.y += m_resolution.y;
		}

		return static_cast<size_t>(wrappedIndex.y * m_resolution.x + wrappedIndex.x);
	}

	Point2I PointHashGridSearcher2::GetBucketIndex(const Vector2D& position) const
	{
		Point2I bucketIndex;
		bucketIndex.x = static_cast<ssize_t>(std::floor(position.x / m_gridSpacing));
		bucketIndex.y = static_cast<ssize_t>(std::floor(position.y / m_gridSpacing));

		return bucketIndex;
	}

	PointNeighborSearcher2Ptr PointHashGridSearcher2::Clone() const
	{
		return std::shared_ptr<PointNeighborSearcher2>(
			new PointHashGridSearcher2(*this), [](PointHashGridSearcher2* obj)
		{
			delete obj;
		});
	}

	PointHashGridSearcher2& PointHashGridSearcher2::operator=(const PointHashGridSearcher2& other)
	{
		Set(other);
		return *this;
	}

	void PointHashGridSearcher2::Set(const PointHashGridSearcher2& other)
	{
		m_resolution = other.m_resolution;
		m_gridSpacing = other.m_gridSpacing;
		m_points = other.m_points;
		m_buckets = other.m_buckets;
	}

	void PointHashGridSearcher2::Serialize(std::vector<uint8_t>* buffer) const
	{
		flatbuffers::FlatBufferBuilder builder(1024);

		// Copy simple data
		auto fbsResolution = fbs::Size2(m_resolution.x, m_resolution.y);

		// Copy points
		std::vector<fbs::Vector2D> points;
		for (const auto& pt : m_points)
		{
			points.push_back(CubbyFlowToFlatbuffers(pt));
		}

		auto fbsPoints = builder.CreateVectorOfStructs(points.data(), points.size());

		// Copy buckets
		std::vector<flatbuffers::Offset<fbs::PointHashGridSearcherBucket2>> buckets;
		for (const auto& bucket : m_buckets)
		{
			std::vector<uint64_t> bucket64(bucket.begin(), bucket.end());
			flatbuffers::Offset<fbs::PointHashGridSearcherBucket2> fbsBucket
				= fbs::CreatePointHashGridSearcherBucket2(
					builder,
					builder.CreateVector(bucket64.data(), bucket64.size()));
			buckets.push_back(fbsBucket);
		}

		auto fbsBuckets = builder.CreateVector(buckets);

		// Copy the searcher
		auto fbsSearcher = fbs::CreatePointHashGridSearcher2(
			builder, m_gridSpacing, &fbsResolution, fbsPoints, fbsBuckets);

		builder.Finish(fbsSearcher);

		uint8_t* buf = builder.GetBufferPointer();
		size_t size = builder.GetSize();

		buffer->resize(size);
		memcpy(buffer->data(), buf, size);
	}

	void PointHashGridSearcher2::Deserialize(const std::vector<uint8_t>& buffer)
	{
		auto fbsSearcher = fbs::GetPointHashGridSearcher2(buffer.data());

		// Copy simple data
		auto res = FlatbuffersToCubbyFlow(*fbsSearcher->resolution());
		m_resolution.Set({ res.x, res.y });
		m_gridSpacing = fbsSearcher->gridSpacing();

		// Copy points
		auto fbsPoints = fbsSearcher->points();
		m_points.resize(fbsPoints->size());
		for (uint32_t i = 0; i < fbsPoints->size(); ++i)
		{
			m_points[i] = FlatbuffersToCubbyFlow(*fbsPoints->Get(i));
		}

		// Copy buckets
		auto fbsBuckets = fbsSearcher->buckets();
		m_buckets.resize(fbsBuckets->size());		
		for (uint32_t i = 0; i < fbsBuckets->size(); ++i)
		{
			auto fbsBucket = fbsBuckets->Get(i);
			m_buckets[i].resize(fbsBucket->data()->size());			
			std::transform(
				fbsBucket->data()->begin(),
				fbsBucket->data()->end(),
				m_buckets[i].begin(),
				[](uint64_t val)
			{
				return static_cast<size_t>(val);
			});
		}
	}

	PointHashGridSearcher2::Builder PointHashGridSearcher2::GetBuilder()
	{
		return Builder();
	}

	size_t PointHashGridSearcher2::GetHashKeyFromPosition(const Vector2D& position) const
	{
		Point2I bucketIndex = GetBucketIndex(position);
		return GetHashKeyFromBucketIndex(bucketIndex);
	}

	void PointHashGridSearcher2::GetNearbyKeys(const Vector2D& position, size_t* nearbyKeys) const
	{
		Point2I originIndex = GetBucketIndex(position), nearbyBucketIndices[4];

		for (size_t i = 0; i < 4; ++i)
		{
			nearbyBucketIndices[i] = originIndex;
		}

		if ((originIndex.x + 0.5f) * m_gridSpacing <= position.x)
		{
			nearbyBucketIndices[2].x += 1;
			nearbyBucketIndices[3].x += 1;
		}
		else
		{
			nearbyBucketIndices[2].x -= 1; 
			nearbyBucketIndices[3].x -= 1;
		}

		if ((originIndex.y + 0.5f) * m_gridSpacing <= position.y)
		{
			nearbyBucketIndices[1].y += 1;
			nearbyBucketIndices[3].y += 1;
		}
		else
		{
			nearbyBucketIndices[1].y -= 1;
			nearbyBucketIndices[3].y -= 1;
		}

		for (size_t i = 0; i < 4; ++i)
		{
			nearbyKeys[i] = GetHashKeyFromBucketIndex(nearbyBucketIndices[i]);
		}
	}

	PointHashGridSearcher2::Builder& PointHashGridSearcher2::Builder::WithResolution(const Size2& resolution)
	{
		m_resolution = resolution;
		return *this;
	}

	PointHashGridSearcher2::Builder& PointHashGridSearcher2::Builder::WithGridSpacing(double gridSpacing)
	{
		m_gridSpacing = gridSpacing;
		return *this;
	}

	PointHashGridSearcher2 PointHashGridSearcher2::Builder::Build() const
	{
		return PointHashGridSearcher2(m_resolution, m_gridSpacing);
	}

	PointHashGridSearcher2Ptr PointHashGridSearcher2::Builder::MakeShared() const
	{
		return std::shared_ptr<PointHashGridSearcher2>(new PointHashGridSearcher2(m_resolution,m_gridSpacing),
			[](PointHashGridSearcher2* obj)
		{
			delete obj;
		});
	}

	PointNeighborSearcher2Ptr PointHashGridSearcher2::Builder::BuildPointNeighborSearcher() const
	{
		return MakeShared();
	}
}