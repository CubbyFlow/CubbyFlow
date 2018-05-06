/*************************************************************************
> File Name: VertexCenteredVectorGrid3.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: 3-D Vertex-centered vector grid structure.
> Created Time: 2017/08/04
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#include <Core/Grid/VertexCenteredVectorGrid3.h>

namespace CubbyFlow
{
	VertexCenteredVectorGrid3::VertexCenteredVectorGrid3()
	{
		// Do nothing
	}

	VertexCenteredVectorGrid3::VertexCenteredVectorGrid3(
		size_t resolutionX, size_t resolutionY, size_t resolutionZ,
		double gridSpacingX, double gridSpacingY, double gridSpacingZ,
		double originX, double originY, double originZ,
		double initialValueU, double initialValueV, double initialValueW)
	{
		Resize(
			resolutionX, resolutionY, resolutionZ,
			gridSpacingX, gridSpacingY, gridSpacingZ,
			originX, originY, originZ, 
			initialValueU, initialValueV, initialValueW);
	}

	VertexCenteredVectorGrid3::VertexCenteredVectorGrid3(
		const Size3& resolution,
		const Vector3D& gridSpacing,
		const Vector3D& origin,
		const Vector3D& initialValue)
	{
		Resize(resolution, gridSpacing, origin, initialValue);
	}

	VertexCenteredVectorGrid3::VertexCenteredVectorGrid3(const VertexCenteredVectorGrid3& other)
	{
		Set(other);
	}

	Size3 VertexCenteredVectorGrid3::GetDataSize() const
	{
		if (Resolution() != Size3(0, 0, 0))
		{
			return Resolution() + Size3(1, 1, 1);
		}

		return Size3(0, 0, 0);
	}

	Vector3D VertexCenteredVectorGrid3::GetDataOrigin() const
	{
		return Origin();
	}

	void VertexCenteredVectorGrid3::Swap(Grid3* other)
	{
		VertexCenteredVectorGrid3* sameType = dynamic_cast<VertexCenteredVectorGrid3*>(other);
		if (sameType != nullptr)
		{
			SwapCollocatedVectorGrid(sameType);
		}
	}

	void VertexCenteredVectorGrid3::Fill(const Vector3D& value, ExecutionPolicy policy)
	{
		Size3 size = GetDataSize();
		auto acc = GetDataAccessor();

		ParallelFor(
			ZERO_SIZE, size.x,
			ZERO_SIZE, size.y,
			ZERO_SIZE, size.z,
			[value, &acc](size_t i, size_t j, size_t k)
		{
			acc(i, j, k) = value;
		}, policy);
	}

	void VertexCenteredVectorGrid3::Fill(const std::function<Vector3D(const Vector3D&)>& func, ExecutionPolicy policy)
	{
		Size3 size = GetDataSize();
		auto acc = GetDataAccessor();
		DataPositionFunc pos = GetDataPosition();

		ParallelFor(
			ZERO_SIZE, size.x,
			ZERO_SIZE, size.y,
			ZERO_SIZE, size.z,
			[&func, &acc, &pos](size_t i, size_t j, size_t k)
		{
			acc(i, j, k) = func(pos(i, j, k));
		}, policy);
	}

	std::shared_ptr<VectorGrid3> VertexCenteredVectorGrid3::Clone() const
	{
		return std::shared_ptr<VertexCenteredVectorGrid3>(
			new VertexCenteredVectorGrid3(*this), [](VertexCenteredVectorGrid3* obj)
		{
			delete obj;
		});
	}

	void VertexCenteredVectorGrid3::Set(const VertexCenteredVectorGrid3& other)
	{
		SetCollocatedVectorGrid(other);
	}

	VertexCenteredVectorGrid3& VertexCenteredVectorGrid3::operator=(const VertexCenteredVectorGrid3& other)
	{
		Set(other);
		return *this;
	}

	VertexCenteredVectorGrid3::Builder VertexCenteredVectorGrid3::GetBuilder()
	{
		return Builder();
	}

	VertexCenteredVectorGrid3::Builder& VertexCenteredVectorGrid3::Builder::WithResolution(const Size3& resolution)
	{
		m_resolution = resolution;
		return *this;
	}

	VertexCenteredVectorGrid3::Builder& VertexCenteredVectorGrid3::Builder::WithResolution(size_t resolutionX, size_t resolutionY, size_t resolutionZ)
	{
		m_resolution.x = resolutionX;
		m_resolution.y = resolutionY;
		m_resolution.z = resolutionZ;
		return *this;
	}

	VertexCenteredVectorGrid3::Builder& VertexCenteredVectorGrid3::Builder::WithGridSpacing(const Vector3D& gridSpacing)
	{
		m_gridSpacing = gridSpacing;
		return *this;
	}

	VertexCenteredVectorGrid3::Builder& VertexCenteredVectorGrid3::Builder::WithGridSpacing(double gridSpacingX, double gridSpacingY, double gridSpacingZ)
	{
		m_gridSpacing.x = gridSpacingX;
		m_gridSpacing.y = gridSpacingY;
		m_gridSpacing.z = gridSpacingZ;
		return *this;
	}

	VertexCenteredVectorGrid3::Builder& VertexCenteredVectorGrid3::Builder::WithOrigin(const Vector3D& gridOrigin)
	{
		m_gridOrigin = gridOrigin;
		return *this;
	}

	VertexCenteredVectorGrid3::Builder& VertexCenteredVectorGrid3::Builder::WithOrigin(double gridOriginX, double gridOriginY, double gridOriginZ)
	{
		m_gridOrigin.x = gridOriginX;
		m_gridOrigin.y = gridOriginY;
		m_gridOrigin.z = gridOriginZ;
		return *this;
	}

	VertexCenteredVectorGrid3::Builder& VertexCenteredVectorGrid3::Builder::WithInitialValue(double initialValU, double initialValV, double initialValW)
	{
		m_initialVal.x = initialValU;
		m_initialVal.y = initialValV;
		m_initialVal.z = initialValW;
		return *this;
	}

	VertexCenteredVectorGrid3 VertexCenteredVectorGrid3::Builder::Build() const
	{
		return VertexCenteredVectorGrid3(m_resolution, m_gridSpacing, m_gridOrigin, m_initialVal);
	}

	VertexCenteredVectorGrid3Ptr VertexCenteredVectorGrid3::Builder::MakeShared() const
	{
		return std::shared_ptr<VertexCenteredVectorGrid3>(
			new VertexCenteredVectorGrid3(m_resolution, m_gridSpacing, m_gridOrigin, m_initialVal),
			[](VertexCenteredVectorGrid3* obj)
		{
			delete obj;
		});
	}

	VectorGrid3Ptr VertexCenteredVectorGrid3::Builder::Build(
		const Size3& resolution,
		const Vector3D& gridSpacing,
		const Vector3D& gridOrigin,
		const Vector3D& initialVal) const
	{
		return std::shared_ptr<VertexCenteredVectorGrid3>(
			new VertexCenteredVectorGrid3(resolution, gridSpacing, gridOrigin, initialVal),
			[](VertexCenteredVectorGrid3* obj)
		{
			delete obj;
		});
	}
}