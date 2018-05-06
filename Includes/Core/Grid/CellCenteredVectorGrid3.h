/*************************************************************************
> File Name: CellCenteredVectorGrid3.h
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: 3-D Cell-centered vector grid structure.
> Created Time: 2017/08/02
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#ifndef CUBBYFLOW_CELL_CENTERED_VECTOR_GRID3_H
#define CUBBYFLOW_CELL_CENTERED_VECTOR_GRID3_H

#include <Core/Grid/CollocatedVectorGrid3.h>

namespace CubbyFlow
{
	//!
	//! \brief 3-D Cell-centered vector grid structure.
	//!
	//! This class represents 3-D cell-centered vector grid which extends
	//! VectorGrid3. As its name suggests, the class defines the data point at the
	//! center of a grid cell. Thus, the dimension of data points are equal to the
	//! dimension of the cells.
	//!
	class CellCenteredVectorGrid3 final : public CollocatedVectorGrid3
	{
	public:
		CUBBYFLOW_GRID3_TYPE_NAME(CellCenteredVectorGrid3);

		class Builder;

		//! Constructs zero-sized grid.
		CellCenteredVectorGrid3();

		//! Constructs a grid with given resolution, grid spacing, origin
		//! and initial value.
		CellCenteredVectorGrid3(
			size_t resolutionX, size_t resolutionY, size_t resolutionZ,
			double gridSpacingX = 1.0, double gridSpacingY = 1.0, double gridSpacingZ = 1.0,
			double originX = 0.0, double originY = 0.0, double originZ = 0.0,
			double initialValueU = 0.0, double initalValueV = 0.0, double initalValueZ = 0.0);

		//! Constructs a grid with given resolution, grid spacing, origin
		//! and initial value.
		CellCenteredVectorGrid3(
			const Size3& resolution,
			const Vector3D& gridSpacing = Vector3D(1.0, 1.0, 1.0),
			const Vector3D& origin = Vector3D(),
			const Vector3D& initalValue = Vector3D());

		//! Copy constructor.
		CellCenteredVectorGrid3(const CellCenteredVectorGrid3& other);

		//! Returns the actual data point size.
		Size3 GetDataSize() const override;

		//! Returns data position for the grid point at (0, 0, 0).
		//! Note that this is different from origin() since origin() returns
		//! the lower corner point of the bounding box.
		Vector3D GetDataOrigin() const override;

		//! Returns the copy of the grid instance.
		std::shared_ptr<VectorGrid3> Clone() const override;

		//!
		//! \brief Swaps the contents with the given \p other grid.
		//!
		//! This function swaps the contents of the grid instance with the given
		//! grid object \p other only if \p other has the same type with this grid.
		//!
		void Swap(Grid3* other) override;

		//! Fills the grid with given value.
		void Fill(const Vector3D& value, ExecutionPolicy policy = ExecutionPolicy::Parallel) override;

		//! Fills the grid with given function.
		void Fill(const std::function<Vector3D(const Vector3D&)>& func, ExecutionPolicy policy = ExecutionPolicy::Parallel) override;

		//! Sets the contents with the given \p other grid.
		void Set(const CellCenteredVectorGrid3& other);

		//! Sets the contents with the given \p other gird.
		CellCenteredVectorGrid3& operator=(const CellCenteredVectorGrid3& other);

		//! Returns the builder fox CellCenteredVectorGrid3.
		static Builder GetBuilder();
	};

	//! Shared pointer for the CellCenteredVectorGrid3 type.
	using CellCenteredVectorGrid3Ptr = std::shared_ptr<CellCenteredVectorGrid3>;

	//!
	//! \brief Front-end to create CellCenteredVectorGrid3 objects step by step.
	//!
	class CellCenteredVectorGrid3::Builder final : public VectorGridBuilder3
	{
	public:
		//! Returns builder with resolution.
		Builder& WithResolution(const Size3& resolution);

		//! Returns builder with resolution.
		Builder& WithResolution(size_t resolutionX, size_t resolutionY, size_t resolutionZ);

		//! Returns builder with grid spacing.
		Builder& WithGridSpacing(const Vector3D& gridSpacing);

		//! Returns builder with grid spacing.
		Builder& WithGridSpacing(double gridSpacingX, double gridSpacingY, double gridSpacingZ);

		//! Returns builder with grid origin.
		Builder& WithOrigin(const Vector3D& gridOrigin);

		//! Returns builder with grid origin.
		Builder& WithOrigin(double gridOriginX, double gridOriginY, double gridOriginZ);

		//! Returns builder with initial value.
		Builder& WithInitialValue(const Vector3D& initalVal);

		//! Returns builder with initial value.
		Builder& WithInitialValue(double initialValX, double initalValY, double initalValZ);

		//! Builds CellCenteredVectorGrid3 instance.
		CellCenteredVectorGrid3 Build() const;

		//! Builds shared pointer of CellCenteredScalarGrid3 instance.
		CellCenteredVectorGrid3Ptr MakeShared() const;

		//!
		//! \brief Builds shared pointer of CellCenteredVectorGrid3 instance.
		//!
		//! This is an overriding function that implements VectorGridBuilder3.
		//!
		VectorGrid3Ptr Build(
			const Size3& resolution,
			const Vector3D& gridSpacing,
			const Vector3D& gridOrigin,
			const Vector3D& initalVal) const override;

	private:
		Size3 m_resolution{ 1, 1, 1 };
		Vector3D m_gridSpacing{ 1, 1, 1 };
		Vector3D m_gridOrigin{ 0, 0, 0 };
		Vector3D m_initialVal{ 0, 0, 0 };
	};
}

#endif