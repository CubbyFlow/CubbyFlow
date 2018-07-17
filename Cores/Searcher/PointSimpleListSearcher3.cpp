/*************************************************************************
> File Name: PointSimpleListSearcher3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Simple ad-hoc 3-D point searcher.
> Created Time: 2017/05/26
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#include <Searcher/PointSimpleListSearcher3.h>
#include <Utils/FlatbuffersHelper.h>

#include <Flatbuffers/generated/PointSimpleListSearcher3_generated.h>

namespace CubbyFlow
{
	PointSimpleListSearcher3::PointSimpleListSearcher3()
	{
		// Do nothing
	}

	PointSimpleListSearcher3::PointSimpleListSearcher3(const PointSimpleListSearcher3& other)
	{
		Set(other);
	}

	void PointSimpleListSearcher3::Build(const ConstArrayAccessor1<Vector3D>& points)
	{
		m_points.resize(points.size());
		std::copy(points.data(), points.data() + points.size(), m_points.begin());
	}

	void PointSimpleListSearcher3::ForEachNearbyPoint(
		const Vector3D& origin,
		double radius,
		const ForEachNearbyPointFunc& callback) const
	{
		double radiusSquared = radius * radius;

		for (size_t i = 0; i < m_points.size(); ++i)
		{
			Vector3D r = m_points[i] - origin;
			double distanceSquared = r.Dot(r);
			if (distanceSquared <= radiusSquared)
			{
				callback(i, m_points[i]);
			}
		}
	}

	bool PointSimpleListSearcher3::HasNearbyPoint(const Vector3D& origin, double radius) const
	{
		double radiusSquared = radius * radius;

		for (size_t i = 0; i < m_points.size(); ++i)
		{
			Vector3D r = m_points[i] - origin;
			double distanceSquared = r.Dot(r);
			if (distanceSquared <= radiusSquared)
			{
				return true;
			}
		}

		return false;
	}

	PointNeighborSearcher3Ptr PointSimpleListSearcher3::Clone() const
	{
		return std::shared_ptr<PointSimpleListSearcher3>(
			new PointSimpleListSearcher3(*this), [](PointSimpleListSearcher3* obj)
		{
			delete obj;
		});
	}

	PointSimpleListSearcher3& PointSimpleListSearcher3::operator=(const PointSimpleListSearcher3& other)
	{
		Set(other);
		return *this;
	}

	void PointSimpleListSearcher3::Set(const PointSimpleListSearcher3& other)
	{
		m_points = other.m_points;
	}

	void PointSimpleListSearcher3::Serialize(std::vector<uint8_t>* buffer) const
	{
		flatbuffers::FlatBufferBuilder builder(1024);

		// Copy points
		std::vector<fbs::Vector3D> points;
		for (const auto& pt : m_points)
		{
			points.push_back(CubbyFlowToFlatbuffers(pt));
		}

		auto fbsPoints = builder.CreateVectorOfStructs(points.data(), points.size());

		// Copy the searcher
		auto fbsSearcher = fbs::CreatePointSimpleListSearcher3(builder, fbsPoints);

		builder.Finish(fbsSearcher);

		uint8_t* buf = builder.GetBufferPointer();
		size_t size = builder.GetSize();

		buffer->resize(size);
		memcpy(buffer->data(), buf, size);
	}

	void PointSimpleListSearcher3::Deserialize(const std::vector<uint8_t>& buffer)
	{
		auto fbsSearcher = fbs::GetPointSimpleListSearcher3(buffer.data());

		// Copy points
		auto fbsPoints = fbsSearcher->points();
		m_points.resize(fbsPoints->size());
		for (uint32_t i = 0; i < fbsPoints->size(); ++i)
		{
			m_points[i] = FlatbuffersToCubbyFlow(*fbsPoints->Get(i));
		}
	}

	PointSimpleListSearcher3 PointSimpleListSearcher3::Builder::Build() const
	{
		return PointSimpleListSearcher3();
	}

	PointSimpleListSearcher3Ptr PointSimpleListSearcher3::Builder::MakeShared() const
	{
		return std::shared_ptr<PointSimpleListSearcher3>(
			new PointSimpleListSearcher3(), [](PointSimpleListSearcher3* obj)
		{
			delete obj;
		});
	}

	PointNeighborSearcher3Ptr PointSimpleListSearcher3::Builder::BuildPointNeighborSearcher() const
	{
		return MakeShared();
	}
}