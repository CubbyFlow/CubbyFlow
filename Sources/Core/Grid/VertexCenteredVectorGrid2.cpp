/*************************************************************************
> File Name: VertexCenteredVectorGrid2.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: 2-D Vertex-centered vector grid structure.
> Created Time: 2017/08/04
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#include <Core/Grid/VertexCenteredVectorGrid2.h>

namespace CubbyFlow
{
	VertexCenteredVectorGrid2::VertexCenteredVectorGrid2()
	{
		// Do nothing
	}

	VertexCenteredVectorGrid2::VertexCenteredVectorGrid2(
		size_t resolutionX, size_t resolutionY,
		double gridSpacingX, double gridSpacingY,
		double originX, double originY,
		double initialValueU, double initialValueV)
	{
		Resize(
			resolutionX, resolutionY,
			gridSpacingX, gridSpacingY,
			originX, originY,
			initialValueU, initialValueV);
	}

	VertexCenteredVectorGrid2::VertexCenteredVectorGrid2(
		const Size2& resolution,
		const Vector2D& gridSpacing,
		const Vector2D& origin,
		const Vector2D& initialValue)
	{
		Resize(resolution, gridSpacing, origin, initialValue);
	}

	VertexCenteredVectorGrid2::VertexCenteredVectorGrid2(const VertexCenteredVectorGrid2& other)
	{
		Set(other);
	}

	Size2 VertexCenteredVectorGrid2::GetDataSize() const
	{
		if (Resolution() != Size2(0, 0))
		{
			return Resolution() + Size2(1, 1);
		}

		return Size2(0, 0);
	}

	Vector2D VertexCenteredVectorGrid2::GetDataOrigin() const
	{
		return Origin();
	}

	void VertexCenteredVectorGrid2::Swap(Grid2* other)
	{
		VertexCenteredVectorGrid2* sameType = dynamic_cast<VertexCenteredVectorGrid2*>(other);
		if (sameType != nullptr)
		{
			SwapCollocatedVectorGrid(sameType);
		}
	}

	void VertexCenteredVectorGrid2::Fill(const Vector2D& value, ExecutionPolicy policy)
	{
		Size2 size = GetDataSize();
		auto acc = GetDataAccessor();

		ParallelFor(
			ZERO_SIZE, size.x,
			ZERO_SIZE, size.y,
			[value, &acc](size_t i, size_t j)
		{
			acc(i, j) = value;
		}, policy);
	}

	void VertexCenteredVectorGrid2::Fill(const std::function<Vector2D(const Vector2D&)>& func, ExecutionPolicy policy)
	{
		Size2 size = GetDataSize();
		auto acc = GetDataAccessor();
		DataPositionFunc pos = GetDataPosition();

		ParallelFor(
			ZERO_SIZE, size.x,
			ZERO_SIZE, size.y,
			[&func, &acc, &pos](size_t i, size_t j)
		{
			acc(i, j) = func(pos(i, j));
		}, policy);
	}

	std::shared_ptr<VectorGrid2> VertexCenteredVectorGrid2::Clone() const
	{
		return std::shared_ptr<VertexCenteredVectorGrid2>(
			new VertexCenteredVectorGrid2(*this), [](VertexCenteredVectorGrid2* obj)
		{
			delete obj;
		});
	}

	void VertexCenteredVectorGrid2::Set(const VertexCenteredVectorGrid2& other)
	{
		SetCollocatedVectorGrid(other);
	}

	VertexCenteredVectorGrid2& VertexCenteredVectorGrid2::operator=(const VertexCenteredVectorGrid2& other)
	{
		Set(other);
		return *this;
	}

	VertexCenteredVectorGrid2::Builder VertexCenteredVectorGrid2::GetBuilder()
	{
		return Builder();
	}

	VertexCenteredVectorGrid2::Builder& VertexCenteredVectorGrid2::Builder::WithResolution(const Size2& resolution)
	{
		m_resolution = resolution;
		return *this;
	}

	VertexCenteredVectorGrid2::Builder& VertexCenteredVectorGrid2::Builder::WithResolution(size_t resolutionX, size_t resolutionY)
	{
		m_resolution.x = resolutionX;
		m_resolution.y = resolutionY;
		return *this;
	}

	VertexCenteredVectorGrid2::Builder& VertexCenteredVectorGrid2::Builder::WithGridSpacing(const Vector2D& gridSpacing)
	{
		m_gridSpacing = gridSpacing;
		return *this;
	}

	VertexCenteredVectorGrid2::Builder& VertexCenteredVectorGrid2::Builder::WithGridSpacing(double gridSpacingX, double gridSpacingY)
	{
		m_gridSpacing.x = gridSpacingX;
		m_gridSpacing.y = gridSpacingY;
		return *this;
	}

	VertexCenteredVectorGrid2::Builder& VertexCenteredVectorGrid2::Builder::WithOrigin(const Vector2D& gridOrigin)
	{
		m_gridOrigin = gridOrigin;
		return *this;
	}

	VertexCenteredVectorGrid2::Builder& VertexCenteredVectorGrid2::Builder::WithOrigin(double gridOriginX, double gridOriginY)
	{
		m_gridOrigin.x = gridOriginX;
		m_gridOrigin.y = gridOriginY;
		return *this;
	}

	VertexCenteredVectorGrid2::Builder& VertexCenteredVectorGrid2::Builder::WithInitialValue(double initialValU, double initialValV)
	{
		m_initialVal.x = initialValU;
		m_initialVal.y = initialValV;
		return *this;
	}

	VertexCenteredVectorGrid2 VertexCenteredVectorGrid2::Builder::Build() const
	{
		return VertexCenteredVectorGrid2(m_resolution, m_gridSpacing, m_gridOrigin, m_initialVal);
	}

	VertexCenteredVectorGrid2Ptr VertexCenteredVectorGrid2::Builder::MakeShared() const
	{
		return std::shared_ptr<VertexCenteredVectorGrid2>(
			new VertexCenteredVectorGrid2(m_resolution, m_gridSpacing, m_gridOrigin, m_initialVal),
			[](VertexCenteredVectorGrid2* obj)
		{
			delete obj;
		});
	}

	VectorGrid2Ptr VertexCenteredVectorGrid2::Builder::Build(
		const Size2& resolution,
		const Vector2D& gridSpacing,
		const Vector2D& gridOrigin,
		const Vector2D& initialVal) const
	{
		return std::shared_ptr<VertexCenteredVectorGrid2>(
			new VertexCenteredVectorGrid2(resolution, gridSpacing, gridOrigin, initialVal),
			[](VertexCenteredVectorGrid2* obj)
		{
			delete obj;
		});
	}
}