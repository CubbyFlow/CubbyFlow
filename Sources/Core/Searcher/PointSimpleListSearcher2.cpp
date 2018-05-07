/*************************************************************************
> File Name: PointSimpleListSearcher2.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Simple ad-hoc 2-D point searcher.
> Created Time: 2017/05/26
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Searcher/PointSimpleListSearcher2.h>
#include <Core/Utils/FlatbuffersHelper.h>

#include <Flatbuffers/generated/PointSimpleListSearcher2_generated.h>

namespace CubbyFlow
{
	PointSimpleListSearcher2::PointSimpleListSearcher2()
	{
		// Do nothing
	}

	PointSimpleListSearcher2::PointSimpleListSearcher2(const PointSimpleListSearcher2& other)
	{
		Set(other);
	}

	void PointSimpleListSearcher2::Build(const ConstArrayAccessor1<Vector2D>& points)
	{
		m_points.resize(points.size());
		std::copy(points.data(), points.data() + points.size(), m_points.begin());
	}

	void PointSimpleListSearcher2::ForEachNearbyPoint(
		const Vector2D& origin,
		double radius,
		const ForEachNearbyPointFunc& callback) const
	{
		double radiusSquared = radius * radius;

		for (size_t i = 0; i < m_points.size(); ++i)
		{
			Vector2D r = m_points[i] - origin;
			double distanceSquared = r.Dot(r);
			if (distanceSquared <= radiusSquared)
			{
				callback(i, m_points[i]);
			}
		}
	}

	bool PointSimpleListSearcher2::HasNearbyPoint(const Vector2D& origin, double radius) const
	{
		double radiusSquared = radius * radius;
		
		for (size_t i = 0; i < m_points.size(); ++i)
		{
			Vector2D r = m_points[i] - origin;
			double distanceSquared = r.Dot(r);
			if (distanceSquared <= radiusSquared)
			{
				return true;
			}
		}

		return false;
	}

	PointNeighborSearcher2Ptr PointSimpleListSearcher2::Clone() const
	{
		return std::shared_ptr<PointSimpleListSearcher2>(
			new PointSimpleListSearcher2(*this), [](PointSimpleListSearcher2* obj)
		{
			delete obj;
		});
	}

	PointSimpleListSearcher2& PointSimpleListSearcher2::operator=(const PointSimpleListSearcher2& other)
	{
		Set(other);
		return *this;
	}

	void PointSimpleListSearcher2::Set(const PointSimpleListSearcher2& other)
	{
		m_points = other.m_points;
	}

	void PointSimpleListSearcher2::Serialize(std::vector<uint8_t>* buffer) const
	{
		flatbuffers::FlatBufferBuilder builder(1024);

		// Copy points
		std::vector<fbs::Vector2D> points;
		for (const auto& pt : m_points)
		{
			points.push_back(CubbyFlowToFlatbuffers(pt));
		}

		auto fbsPoints = builder.CreateVectorOfStructs(points.data(), points.size());

		// Copy the searcher
		auto fbsSearcher = fbs::CreatePointSimpleListSearcher2(builder, fbsPoints);

		builder.Finish(fbsSearcher);

		uint8_t* buf = builder.GetBufferPointer();
		size_t size = builder.GetSize();

		buffer->resize(size);
		memcpy(buffer->data(), buf, size);
	}

	void PointSimpleListSearcher2::Deserialize(const std::vector<uint8_t>& buffer)
	{
		auto fbsSearcher = fbs::GetPointSimpleListSearcher2(buffer.data());

		// Copy points
		auto fbsPoints = fbsSearcher->points();
		m_points.resize(fbsPoints->size());
		for (uint32_t i = 0; i < fbsPoints->size(); ++i)
		{
			m_points[i] = FlatbuffersToCubbyFlow(*fbsPoints->Get(i));
		}
	}

	PointSimpleListSearcher2 PointSimpleListSearcher2::Builder::Build() const
	{
		return PointSimpleListSearcher2();
	}

	PointSimpleListSearcher2Ptr PointSimpleListSearcher2::Builder::MakeShared() const
	{
		return std::shared_ptr<PointSimpleListSearcher2>(
			new PointSimpleListSearcher2(), [](PointSimpleListSearcher2* obj)
		{
			delete obj;
		});
	}

	PointNeighborSearcher2Ptr PointSimpleListSearcher2::Builder::BuildPointNeighborSearcher() const
	{
		return MakeShared();
	}
	
	PointSimpleListSearcher2::Builder PointSimpleListSearcher2::GetBuilder()
	{
		return Builder();
	}
}