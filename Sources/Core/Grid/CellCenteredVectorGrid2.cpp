/*************************************************************************
> File Name: CellCenteredVectorGrid2.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: 2-D Cell-centered vector grid structure.
> Created Time: 2017/08/02
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#include <Core/Grid/CellCenteredVectorGrid2.h>

namespace CubbyFlow
{
	CellCenteredVectorGrid2::CellCenteredVectorGrid2()
	{
		// Do nothing
	}

	CellCenteredVectorGrid2::CellCenteredVectorGrid2(
		size_t resolutionX, size_t resolutionY,
		double gridSpacingX, double gridSpacingY,
		double originX, double originY,
		double initialValueU, double initalValueV)
	{
		Resize(
			resolutionX, resolutionY,
			gridSpacingX, gridSpacingY,
			originX, originY,
			initialValueU, initalValueV);
	}

	CellCenteredVectorGrid2::CellCenteredVectorGrid2(
		const Size2& resolution,
		const Vector2D& gridSpacing,
		const Vector2D& origin,
		const Vector2D& initialValue)
	{
		Resize(resolution, gridSpacing, origin, initialValue);
	}

	CellCenteredVectorGrid2::CellCenteredVectorGrid2(const CellCenteredVectorGrid2& other)
	{
		Set(other);
	}

	Size2 CellCenteredVectorGrid2::GetDataSize() const
	{
		// The size of the data should be the same as the grid resolution.
		return Resolution();
	}

	Vector2D CellCenteredVectorGrid2::GetDataOrigin() const
	{
		return Origin() + 0.5 * GridSpacing();
	}

	void CellCenteredVectorGrid2::Swap(Grid2* other)
	{
		CellCenteredVectorGrid2* sameType = dynamic_cast<CellCenteredVectorGrid2*>(other);
		if (sameType != nullptr)
		{
			SwapCollocatedVectorGrid(sameType);
		}
	}

	void CellCenteredVectorGrid2::Set(const CellCenteredVectorGrid2& other)
	{
		SetCollocatedVectorGrid(other);
	}

	CellCenteredVectorGrid2& CellCenteredVectorGrid2::operator=(const CellCenteredVectorGrid2& other)
	{
		Set(other);
		return *this;
	}

	void CellCenteredVectorGrid2::Fill(const Vector2D& value, ExecutionPolicy policy)
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

	void CellCenteredVectorGrid2::Fill(const std::function<Vector2D(const Vector2D&)>& func, ExecutionPolicy policy)
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

	std::shared_ptr<VectorGrid2> CellCenteredVectorGrid2::Clone() const
	{
		return std::shared_ptr<CellCenteredVectorGrid2>(
			new CellCenteredVectorGrid2(*this), [](CellCenteredVectorGrid2* obj)
		{
			delete obj;
		});
	}

	CellCenteredVectorGrid2::Builder CellCenteredVectorGrid2::GetBuilder()
	{
		return Builder();
	}

	CellCenteredVectorGrid2::Builder& CellCenteredVectorGrid2::Builder::WithResolution(const Size2& resolution)
	{
		m_resolution = resolution;
		return *this;
	}

	CellCenteredVectorGrid2::Builder& CellCenteredVectorGrid2::Builder::WithResolution(size_t resolutionX, size_t resolutionY)
	{
		m_resolution.x = resolutionX;
		m_resolution.y = resolutionY;
		return *this;
	}

	CellCenteredVectorGrid2::Builder& CellCenteredVectorGrid2::Builder::WithGridSpacing(const Vector2D& gridSpacing)
	{
		m_gridSpacing = gridSpacing;
		return *this;
	}

	CellCenteredVectorGrid2::Builder& CellCenteredVectorGrid2::Builder::WithGridSpacing(double gridSpacingX, double gridSpacingY)
	{
		m_gridSpacing.x = gridSpacingX;
		m_gridSpacing.y = gridSpacingY;
		return *this;
	}

	CellCenteredVectorGrid2::Builder& CellCenteredVectorGrid2::Builder::WithOrigin(const Vector2D& gridOrigin)
	{
		m_gridOrigin = gridOrigin;
		return *this;
	}

	CellCenteredVectorGrid2::Builder& CellCenteredVectorGrid2::Builder::WithOrigin(double gridOriginX, double gridOriginY)
	{
		m_gridOrigin.x = gridOriginX;
		m_gridOrigin.y = gridOriginY;
		return *this;
	}

	CellCenteredVectorGrid2::Builder& CellCenteredVectorGrid2::Builder::WithInitialValue(const Vector2D& initialVal)
	{
		m_initialVal = initialVal;
		return *this;
	}
	
	CellCenteredVectorGrid2::Builder& CellCenteredVectorGrid2::Builder::WithInitialValue(double initialValX, double initialValY)
	{
		m_initialVal.x = initialValX;
		m_initialVal.y = initialValY;
		return *this;
	}

	CellCenteredVectorGrid2 CellCenteredVectorGrid2::Builder::Build() const
	{
		return CellCenteredVectorGrid2(m_resolution, m_gridSpacing, m_gridOrigin, m_initialVal);
	}

	CellCenteredVectorGrid2Ptr CellCenteredVectorGrid2::Builder::MakeShared() const
	{
		return std::shared_ptr<CellCenteredVectorGrid2>(
			new CellCenteredVectorGrid2(m_resolution, m_gridSpacing, m_gridOrigin, m_initialVal),
			[](CellCenteredVectorGrid2* obj)
		{
			delete obj;
		});
	}

	VectorGrid2Ptr CellCenteredVectorGrid2::Builder::Build(
		const Size2& resolution,
		const Vector2D& gridSpacing,
		const Vector2D& gridOrigin,
		const Vector2D& initialVal) const
	{
		return std::shared_ptr<CellCenteredVectorGrid2>(
			new CellCenteredVectorGrid2(resolution, gridSpacing, gridOrigin, initialVal),
			[](CellCenteredVectorGrid2* obj)
		{
			delete obj;
		});
	}
}