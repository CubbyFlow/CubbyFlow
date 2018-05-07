/*************************************************************************
> File Name: GridSystemData2.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D grid system data.
> Created Time: 2017/08/05
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Grid/GridSystemData2.h>
#include <Core/Utils/Factory.h>
#include <Core/Utils/FlatbuffersHelper.h>

#include <Flatbuffers/generated/GridSystemData2_generated.h>

namespace CubbyFlow
{
	GridSystemData2::GridSystemData2() :
		GridSystemData2({ 0, 0 }, { 1, 1 }, { 0, 0 })
	{
		// Do nothing
	}

	GridSystemData2::GridSystemData2(const Size2& resolution, const Vector2D& gridSpacing, const Vector2D& origin)
	{
		m_velocity = std::make_shared<FaceCenteredGrid2>();
		m_advectableVectorDataList.push_back(m_velocity);
		m_velocityIdx = 0;

		Resize(resolution, gridSpacing, origin);
	}

	GridSystemData2::GridSystemData2(const GridSystemData2& other)
	{
		Resize(other.m_resolution, other.m_gridSpacing, other.m_origin);

		for (auto& data : other.m_scalarDataList)
		{
			m_scalarDataList.push_back(data->Clone());
		}
		for (auto& data : other.m_vectorDataList)
		{
			m_vectorDataList.push_back(data->Clone());
		}
		for (auto& data : other.m_advectableScalarDataList)
		{
			m_advectableScalarDataList.push_back(data->Clone());
		}
		for (auto& data : other.m_advectableVectorDataList)
		{
			m_advectableVectorDataList.push_back(data->Clone());
		}

		assert(m_advectableVectorDataList.size() > 0);

		m_velocity = std::dynamic_pointer_cast<FaceCenteredGrid2>(m_advectableVectorDataList[0]);

		assert(m_velocity != nullptr);

		m_velocityIdx = 0;
	}

	GridSystemData2::~GridSystemData2()
	{
		// Do nothing
	}

	void GridSystemData2::Resize(const Size2& resolution, const Vector2D& gridSpacing, const Vector2D& origin)
	{
		m_resolution = resolution;
		m_gridSpacing = gridSpacing;
		m_origin = origin;

		for (auto& data : m_scalarDataList)
		{
			data->Resize(resolution, gridSpacing, origin);
		}
		for (auto& data : m_vectorDataList)
		{
			data->Resize(resolution, gridSpacing, origin);
		}
		for (auto& data : m_advectableScalarDataList)
		{
			data->Resize(resolution, gridSpacing, origin);
		}
		for (auto& data : m_advectableVectorDataList)
		{
			data->Resize(resolution, gridSpacing, origin);
		}
	}

	Size2 GridSystemData2::GetResolution() const
	{
		return m_resolution;
	}

	Vector2D GridSystemData2::GetGridSpacing() const
	{
		return m_gridSpacing;
	}

	Vector2D GridSystemData2::GetOrigin() const
	{
		return m_origin;
	}

	BoundingBox2D GridSystemData2::GetBoundingBox() const
	{
		return m_velocity->BoundingBox();
	}

	size_t GridSystemData2::AddScalarData(const ScalarGridBuilder2Ptr& builder, double initialVal)
	{
		size_t attrIdx = m_scalarDataList.size();
		m_scalarDataList.push_back(builder->Build(GetResolution(), GetGridSpacing(), GetOrigin(), initialVal));
		return attrIdx;
	}

	size_t GridSystemData2::AddVectorData(const VectorGridBuilder2Ptr& builder, const Vector2D& initialVal)
	{
		size_t attrIdx = m_vectorDataList.size();
		m_vectorDataList.push_back(builder->Build(GetResolution(), GetGridSpacing(), GetOrigin(), initialVal));
		return attrIdx;
	}

	size_t GridSystemData2::AddAdvectableScalarData(const ScalarGridBuilder2Ptr& builder, double initialVal)
	{
		size_t attrIdx = m_advectableScalarDataList.size();
		m_advectableScalarDataList.push_back(builder->Build(GetResolution(), GetGridSpacing(), GetOrigin(), initialVal));
		return attrIdx;
	}

	size_t GridSystemData2::AddAdvectableVectorData(const VectorGridBuilder2Ptr& builder, const Vector2D& initialVal)
	{
		size_t attrIdx = m_advectableVectorDataList.size();
		m_advectableVectorDataList.push_back(builder->Build(GetResolution(), GetGridSpacing(), GetOrigin(), initialVal));
		return attrIdx;
	}

	const FaceCenteredGrid2Ptr& GridSystemData2::GetVelocity() const
	{
		return m_velocity;
	}

	size_t GridSystemData2::GetVelocityIndex() const
	{
		return m_velocityIdx;
	}

	const ScalarGrid2Ptr& GridSystemData2::GetScalarDataAt(size_t idx) const
	{
		return m_scalarDataList[idx];
	}

	const VectorGrid2Ptr& GridSystemData2::GetVectorDataAt(size_t idx) const
	{
		return m_vectorDataList[idx];
	}

	const ScalarGrid2Ptr& GridSystemData2::GetAdvectableScalarDataAt(size_t idx) const
	{
		return m_advectableScalarDataList[idx];
	}

	const VectorGrid2Ptr& GridSystemData2::GetAdvectableVectorDataAt(size_t idx) const
	{
		return m_advectableVectorDataList[idx];
	}

	size_t GridSystemData2::GetNumberOfScalarData() const
	{
		return m_scalarDataList.size();
	}

	size_t GridSystemData2::GetNumberOfVectorData() const
	{
		return m_vectorDataList.size();
	}

	size_t GridSystemData2::GetNumberOfAdvectableScalarData() const
	{
		return m_advectableScalarDataList.size();
	}

	size_t GridSystemData2::GetNumberOfAdvectableVectorData() const
	{
		return m_advectableVectorDataList.size();
	}

	void GridSystemData2::Serialize(std::vector<uint8_t>* buffer) const
	{
		flatbuffers::FlatBufferBuilder builder(1024);

		auto resolution = CubbyFlowToFlatbuffers(m_resolution);
		auto gridSpacing = CubbyFlowToFlatbuffers(m_gridSpacing);
		auto origin = CubbyFlowToFlatbuffers(m_origin);

		std::vector<flatbuffers::Offset<fbs::ScalarGridSerialized2>> scalarDataList;
		std::vector<flatbuffers::Offset<fbs::VectorGridSerialized2>> vectorDataList;
		std::vector<flatbuffers::Offset<fbs::ScalarGridSerialized2>> advScalarDataList;
		std::vector<flatbuffers::Offset<fbs::VectorGridSerialized2>> advVectorDataList;

		SerializeGrid(&builder, m_scalarDataList, fbs::CreateScalarGridSerialized2, &scalarDataList);
		SerializeGrid(&builder, m_vectorDataList, fbs::CreateVectorGridSerialized2, &vectorDataList);
		SerializeGrid(&builder, m_advectableScalarDataList, fbs::CreateScalarGridSerialized2, &advScalarDataList);
		SerializeGrid(&builder, m_advectableVectorDataList, fbs::CreateVectorGridSerialized2, &advVectorDataList);

		auto gsd = fbs::CreateGridSystemData2(
			builder, &resolution, &gridSpacing, &origin, m_velocityIdx,
			builder.CreateVector(scalarDataList),
			builder.CreateVector(vectorDataList),
			builder.CreateVector(advScalarDataList),
			builder.CreateVector(advVectorDataList));

		builder.Finish(gsd);

		uint8_t* buf = builder.GetBufferPointer();
		size_t size = builder.GetSize();

		buffer->resize(size);
		memcpy(buffer->data(), buf, size);
	}

	void GridSystemData2::Deserialize(const std::vector<uint8_t>& buffer)
	{
		auto gsd = fbs::GetGridSystemData2(buffer.data());

		Resize(
			FlatbuffersToCubbyFlow(*gsd->resolution()),
			FlatbuffersToCubbyFlow(*gsd->gridSpacing()),
			FlatbuffersToCubbyFlow(*gsd->origin()));

		m_scalarDataList.clear();
		m_vectorDataList.clear();
		m_advectableScalarDataList.clear();
		m_advectableVectorDataList.clear();

		DeserializeGrid(gsd->scalarData(), Factory::BuildScalarGrid2, &m_scalarDataList);
		DeserializeGrid(gsd->vectorData(), Factory::BuildVectorGrid2, &m_vectorDataList);
		DeserializeGrid(gsd->advectableScalarData(), Factory::BuildScalarGrid2, &m_advectableScalarDataList);
		DeserializeGrid(gsd->advectableVectorData(), Factory::BuildVectorGrid2, &m_advectableVectorDataList);

		m_velocityIdx = static_cast<size_t>(gsd->velocityIdx());
		m_velocity = std::dynamic_pointer_cast<FaceCenteredGrid2>(m_advectableVectorDataList[m_velocityIdx]);
	}
}
