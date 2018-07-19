/*************************************************************************
> File Name: CellCenteredScalarGrid2.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D Cell-centered scalar grid structure.
> Created Time: 2017/08/01
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Grid/CellCenteredScalarGrid2.h>

namespace CubbyFlow
{
	CellCenteredScalarGrid2::CellCenteredScalarGrid2()
	{
		// Do nothing
	}

	CellCenteredScalarGrid2::CellCenteredScalarGrid2(
		size_t resolutionX, size_t resolutionY,
		double gridSpacingX, double gridSpacingY,
		double originX, double originY,
		double initialValue)
	{
		Resize(
			resolutionX, resolutionY,
			gridSpacingX, gridSpacingY,
			originX, originY,
			initialValue);
	}

	CellCenteredScalarGrid2::CellCenteredScalarGrid2(
		const Size2& resolution,
		const Vector2D& gridSpacing,
		const Vector2D& origin,
		double initialValue)
	{
		Resize(resolution, gridSpacing, origin, initialValue);
	}

	CellCenteredScalarGrid2::CellCenteredScalarGrid2(const CellCenteredScalarGrid2& other)
	{
		Set(other);
	}

	Size2 CellCenteredScalarGrid2::GetDataSize() const
	{
		// The size of the data should be the same as the grid resolution.
		return Resolution();
	}

	Vector2D CellCenteredScalarGrid2::GetDataOrigin() const
	{
		return Origin() + 0.5 * GridSpacing();
	}

	std::shared_ptr<ScalarGrid2> CellCenteredScalarGrid2::Clone() const
	{
		return std::shared_ptr<CellCenteredScalarGrid2>(
			new CellCenteredScalarGrid2(*this), [](CellCenteredScalarGrid2* obj)
		{
			delete obj;
		});
	}

	void CellCenteredScalarGrid2::Swap(Grid2* other)
	{
		CellCenteredScalarGrid2* sameType = dynamic_cast<CellCenteredScalarGrid2*>(other);
		if (sameType != nullptr)
		{
			SwapScalarGrid(sameType);
		}
	}

	void CellCenteredScalarGrid2::Set(const CellCenteredScalarGrid2& other)
	{
		SetScalarGrid(other);
	}

	CellCenteredScalarGrid2& CellCenteredScalarGrid2::operator=(const CellCenteredScalarGrid2& other)
	{
		Set(other);
		return *this;
	}

	CellCenteredScalarGrid2::Builder CellCenteredScalarGrid2::GetBuilder()
	{
		return Builder();
	}
	
	CellCenteredScalarGrid2::Builder& CellCenteredScalarGrid2::Builder::WithResolution(const Size2& resolution)
	{
		m_resolution = resolution;
		return *this;
	}

	CellCenteredScalarGrid2::Builder& CellCenteredScalarGrid2::Builder::WithResolution(size_t resolutionX, size_t resolutionY)
	{
		m_resolution.x = resolutionX;
		m_resolution.y = resolutionY;
		return *this;
	}

	CellCenteredScalarGrid2::Builder& CellCenteredScalarGrid2::Builder::WithGridSpacing(const Vector2D& gridSpacing)
	{
		m_gridSpacing = gridSpacing;
		return *this;
	}

	CellCenteredScalarGrid2::Builder& CellCenteredScalarGrid2::Builder::WithGridSpacing(double gridSpacingX, double gridSpacingY)
	{
		m_gridSpacing.x = gridSpacingX;
		m_gridSpacing.y = gridSpacingY;
		return *this;
	}

	CellCenteredScalarGrid2::Builder& CellCenteredScalarGrid2::Builder::WithOrigin(const Vector2D& gridOrigin)
	{
		m_gridOrigin = gridOrigin;
		return *this;
	}

	CellCenteredScalarGrid2::Builder& CellCenteredScalarGrid2::Builder::WithOrigin(double gridOriginX, double gridOriginY)
	{
		m_gridOrigin.x = gridOriginX;
		m_gridOrigin.y = gridOriginY;
		return *this;
	}

	CellCenteredScalarGrid2::Builder& CellCenteredScalarGrid2::Builder::WithInitialValue(double initialVal)
	{
		m_initialVal = initialVal;
		return *this;
	}

	CellCenteredScalarGrid2 CellCenteredScalarGrid2::Builder::Build() const
	{
		return CellCenteredScalarGrid2(m_resolution, m_gridSpacing, m_gridOrigin, m_initialVal);
	}

	CellCenteredScalarGrid2Ptr CellCenteredScalarGrid2::Builder::MakeShared() const
	{
		return std::shared_ptr<CellCenteredScalarGrid2>(
			new CellCenteredScalarGrid2(m_resolution, m_gridSpacing, m_gridOrigin, m_initialVal),
			[](CellCenteredScalarGrid2* obj)
		{
			delete obj;
		});
	}

	ScalarGrid2Ptr CellCenteredScalarGrid2::Builder::Build(
		const Size2& resolution,
		const Vector2D& gridSpacing,
		const Vector2D& gridOrigin,
		double initialVal) const
	{
		return std::shared_ptr<CellCenteredScalarGrid2>(
			new CellCenteredScalarGrid2(resolution, gridSpacing, gridOrigin, initialVal),
			[](CellCenteredScalarGrid2* obj)
		{
			delete obj;
		});
	}
}