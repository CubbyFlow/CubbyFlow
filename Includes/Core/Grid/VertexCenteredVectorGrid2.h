/*************************************************************************
> File Name: VertexCenteredVectorGrid2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D Vertex-centered vector grid structure.
> Created Time: 2017/08/04
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#ifndef CUBBYFLOW_VERTEX_CENTERED_VECTOR_GRID2_H
#define CUBBYFLOW_VERTEX_CENTERED_VECTOR_GRID2_H

#include <Core/Grid/CollocatedVectorGrid2.h>

namespace CubbyFlow
{
	//!
	//! \brief 2-D Vertex-centered vector grid structure.
	//!
	//! This class represents 2-D vertex-centered vector grid which extends
	//! CollocatedVectorGrid2. As its name suggests, the class defines the data
	//! point at the grid vertices (corners). Thus, A x B grid resolution will have
	//! (A+1) x (B+1) data points.
	//!
	class VertexCenteredVectorGrid2 final : public CollocatedVectorGrid2
	{
	public:
		CUBBYFLOW_GRID2_TYPE_NAME(VertexCenteredVectorGrid2)

		class Builder;

		//! Constructs zero-sized grid.
		VertexCenteredVectorGrid2();

		//! Constructs a grid with given resolution, grid spacing, origin and
		//! initial value.
		VertexCenteredVectorGrid2(
			size_t resolutionX, size_t resolutionY,
			double gridSpacingX = 1.0, double gridSpacingY = 1.0,
			double originX = 0.0, double originY = 0.0,
			double initialValueU = 0.0, double initialValueV = 0.0);

		//! Constructs a grid with given resolution, grid spacing, origin and
		//! initial value.
		VertexCenteredVectorGrid2(
			const Size2& resolution,
			const Vector2D& gridSpacing = Vector2D(1.0, 1.0),
			const Vector2D& origin = Vector2D(),
			const Vector2D& initialValue = Vector2D());

		//! Copy constructor.
		VertexCenteredVectorGrid2(const VertexCenteredVectorGrid2& other);

		//! Returns the actual data point size.
		Size2 GetDataSize() const override;

		//! Returns data position for the grid point at (0, 0).
		//! Note that this is different from origin() since origin() returns
		//! the lower corner point of the bounding box.
		Vector2D GetDataOrigin() const override;

		//!
		//! \brief Swaps the contents with the given \p other grid.
		//!
		//! This function swaps the contents of the grid instance with the given
		//! grid object \p other only if \p other has the same type with this grid.
		//!
		void Swap(Grid2* other) override;

		//! Sets the contents with the given \p other grid.
		void Set(const VertexCenteredVectorGrid2& other);

		//! Sets the contents with the given \p other grid.
		VertexCenteredVectorGrid2& operator=(const VertexCenteredVectorGrid2& other);

		//! Fills the grid with given value.
		void Fill(const Vector2D& value, ExecutionPolicy policy = ExecutionPolicy::Parallel) override;

		//! Fills the grid with given function.
		void Fill(const std::function<Vector2D(const Vector2D&)>& func, ExecutionPolicy policy = ExecutionPolicy::Parallel) override;

		//! Returns the copy of the grid instance.
		std::shared_ptr<VectorGrid2> Clone() const override;

		//! Returns builder fox VertexCenteredVectorGrid2.
		static Builder GetBuilder();
	};

	//! Shared pointer for the VertexCenteredVectorGrid2 type.
	using VertexCenteredVectorGrid2Ptr = std::shared_ptr<VertexCenteredVectorGrid2>;

	//! A grid builder class that returns 2-D vertex-centered Vector grid.
	class VertexCenteredVectorGrid2::Builder final : public VectorGridBuilder2
	{
	public:
		//! Returns builder with resolution.
		Builder& WithResolution(const Size2& resolution);

		//! Returns builder with resolution.
		Builder& WithResolution(size_t resolutionX, size_t resolutionY);

		//! Returns builder with grid spacing.
		Builder& WithGridSpacing(const Vector2D& gridSpacing);

		//! Returns builder with grid spacing.
		Builder& WithGridSpacing(double gridSpacingX, double gridSpacingY);

		//! Returns builder with grid origin.
		Builder& WithOrigin(const Vector2D& gridOrigin);

		//! Returns builder with grid origin.
		Builder& WithOrigin(double gridOriginX, double gridOriginY);

		//! Returns builder with initial value.
		Builder& WithInitialValue(double initialValU, double initialValV);

		//! Builds VertexCenteredVectorGrid2 instance.
		VertexCenteredVectorGrid2 Build() const;

		//! Builds shared pointer of VertexCenteredVectorGrid2 instance.
		VertexCenteredVectorGrid2Ptr MakeShared() const;

		//!
		//! \brief Builds shared pointer of VertexCenteredVectorGrid2 instance.
		//!
		//! This is an overriding function that implements VectorGridBuilder2.
		//!
		VectorGrid2Ptr Build(
			const Size2& resolution,
			const Vector2D& gridSpacing,
			const Vector2D& gridOrigin,
			const Vector2D& initialVal) const override;

	private:
		Size2 m_resolution{ 1, 1 };
		Vector2D m_gridSpacing{ 1, 1 };
		Vector2D m_gridOrigin{ 0, 0 };
		Vector2D m_initialVal{ 0, 0 };
	};
}

#endif