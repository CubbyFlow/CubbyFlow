/*************************************************************************
> File Name: CellCenteredVectorGrid3.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: 3-D Cell-centered vector grid structure.
> Created Time: 2017/08/02
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#include <Core/Grid/CellCenteredVectorGrid3.h>

namespace CubbyFlow
{
	CellCenteredVectorGrid3::CellCenteredVectorGrid3()
	{
		// Do nothing
	}

	CellCenteredVectorGrid3::CellCenteredVectorGrid3(
		size_t resolutionX, size_t resolutionY, size_t resolutionZ,
		double gridSpacingX, double gridSpacingY, double gridSpacingZ,
		double originX, double originY, double originZ,
		double initialValueU, double initalValueV, double initalValueW)
	{
		Resize(
			resolutionX, resolutionY, resolutionZ,
			gridSpacingX, gridSpacingY, gridSpacingZ,
			originX, originY, originZ, 
			initialValueU, initalValueV, initalValueW);
	}

	CellCenteredVectorGrid3::CellCenteredVectorGrid3(
		const Size3& resolution,
		const Vector3D& gridSpacing,
		const Vector3D& origin,
		const Vector3D& initialValue)
	{
		Resize(resolution, gridSpacing, origin, initialValue);
	}

	CellCenteredVectorGrid3::CellCenteredVectorGrid3(const CellCenteredVectorGrid3& other)
	{
		Set(other);
	}

	Size3 CellCenteredVectorGrid3::GetDataSize() const
	{
		// The size of the data should be the same as the grid resolution.
		return Resolution();
	}

	Vector3D CellCenteredVectorGrid3::GetDataOrigin() const
	{
		return Origin() + 0.5 * GridSpacing();
	}

	void CellCenteredVectorGrid3::Swap(Grid3* other)
	{
		CellCenteredVectorGrid3* sameType = dynamic_cast<CellCenteredVectorGrid3*>(other);
		if (sameType != nullptr)
		{
			SwapCollocatedVectorGrid(sameType);
		}
	}

	void CellCenteredVectorGrid3::Set(const CellCenteredVectorGrid3& other)
	{
		SetCollocatedVectorGrid(other);
	}

	CellCenteredVectorGrid3& CellCenteredVectorGrid3::operator=(const CellCenteredVectorGrid3& other)
	{
		Set(other);
		return *this;
	}

	void CellCenteredVectorGrid3::Fill(const Vector3D& value, ExecutionPolicy policy)
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

	void CellCenteredVectorGrid3::Fill(const std::function<Vector3D(const Vector3D&)>& func, ExecutionPolicy policy)
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

	std::shared_ptr<VectorGrid3> CellCenteredVectorGrid3::Clone() const
	{
		return std::shared_ptr<CellCenteredVectorGrid3>(
			new CellCenteredVectorGrid3(*this), [](CellCenteredVectorGrid3* obj)
		{
			delete obj;
		});
	}

	CellCenteredVectorGrid3::Builder CellCenteredVectorGrid3::GetBuilder()
	{
		return Builder();
	}

	CellCenteredVectorGrid3::Builder& CellCenteredVectorGrid3::Builder::WithResolution(const Size3& resolution)
	{
		m_resolution = resolution;
		return *this;
	}

	CellCenteredVectorGrid3::Builder& CellCenteredVectorGrid3::Builder::WithResolution(size_t resolutionX, size_t resolutionY, size_t resolutionZ)
	{
		m_resolution.x = resolutionX;
		m_resolution.y = resolutionY;
		m_resolution.z = resolutionZ;
		return *this;
	}

	CellCenteredVectorGrid3::Builder& CellCenteredVectorGrid3::Builder::WithGridSpacing(const Vector3D& gridSpacing)
	{
		m_gridSpacing = gridSpacing;
		return *this;
	}

	CellCenteredVectorGrid3::Builder& CellCenteredVectorGrid3::Builder::WithGridSpacing(double gridSpacingX, double gridSpacingY, double gridSpacingZ)
	{
		m_gridSpacing.x = gridSpacingX;
		m_gridSpacing.y = gridSpacingY;
		m_gridSpacing.z = gridSpacingZ;
		return *this;
	}

	CellCenteredVectorGrid3::Builder& CellCenteredVectorGrid3::Builder::WithOrigin(const Vector3D& gridOrigin)
	{
		m_gridOrigin = gridOrigin;
		return *this;
	}

	CellCenteredVectorGrid3::Builder& CellCenteredVectorGrid3::Builder::WithOrigin(double gridOriginX, double gridOriginY, double gridOriginZ)
	{
		m_gridOrigin.x = gridOriginX;
		m_gridOrigin.y = gridOriginY;
		m_gridOrigin.z = gridOriginZ;
		return *this;
	}

	CellCenteredVectorGrid3::Builder& CellCenteredVectorGrid3::Builder::WithInitialValue(const Vector3D& initialVal)
	{
		m_initialVal = initialVal;
		return *this;
	}

	CellCenteredVectorGrid3::Builder& CellCenteredVectorGrid3::Builder::WithInitialValue(double initalValX, double initalValY, double initalValZ)
	{
		m_initialVal.x = initalValX;
		m_initialVal.y = initalValY;
		m_initialVal.z = initalValZ;
		return *this;
	}

	CellCenteredVectorGrid3 CellCenteredVectorGrid3::Builder::Build() const
	{
		return CellCenteredVectorGrid3(m_resolution, m_gridSpacing, m_gridOrigin, m_initialVal);
	}

	CellCenteredVectorGrid3Ptr CellCenteredVectorGrid3::Builder::MakeShared() const
	{
		return std::shared_ptr<CellCenteredVectorGrid3>(
			new CellCenteredVectorGrid3(m_resolution, m_gridSpacing, m_gridOrigin, m_initialVal),
			[](CellCenteredVectorGrid3* obj)
		{
			delete obj;
		});
	}

	VectorGrid3Ptr CellCenteredVectorGrid3::Builder::Build(
		const Size3& resolution,
		const Vector3D& gridSpacing,
		const Vector3D& gridOrigin,
		const Vector3D& initialVal) const
	{
		return std::shared_ptr<CellCenteredVectorGrid3>(
			new CellCenteredVectorGrid3(resolution, gridSpacing, gridOrigin, initialVal),
			[](CellCenteredVectorGrid3* obj)
		{
			delete obj;
		});
	}
}
