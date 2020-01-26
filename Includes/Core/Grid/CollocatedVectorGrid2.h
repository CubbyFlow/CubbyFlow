/*************************************************************************
> File Name: CollocatedVectorGrid2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for 2-D collocated vector grid structure.
> Created Time: 2017/08/02
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_COLLOCATED_VECTOR_GRID2_H
#define CUBBYFLOW_COLLOCATED_VECTOR_GRID2_H

#include <Core/Array/Array2.hpp>
#include <Core/Array/ArraySamplers2.hpp>
#include <Core/Grid/VectorGrid2.h>

namespace CubbyFlow
{
	//! \brief Abstract base class for 2-D collocated vector grid structure.
	class CollocatedVectorGrid2 : public VectorGrid2
	{
	public:
		//! Constructs an empty grid.
		CollocatedVectorGrid2();

		//! Default destructor.
		virtual ~CollocatedVectorGrid2();

		//! Returns the actual data point size.
		virtual Size2 GetDataSize() const = 0;

		//!
		//! \brief Returns data position for the grid point at (0, 0).
		//!
		//! Note that this is different from origin() since origin() returns
		//! the lower corner point of the bounding box.
		//!
		virtual Vector2D GetDataOrigin() const = 0;

		//! Returns the grid data at given data point.
		const Vector2D& operator()(size_t i, size_t j) const;

		//! Returns the grid data at given data point.
		Vector2D& operator()(size_t i, size_t j);

		//! Returns divergence at data point location.
		double DivergenceAtDataPoint(size_t i, size_t j) const;

		//! Returns curl at data point location.
		double CurlAtDataPoint(size_t i, size_t j) const;

		//! Returns the read-write data array accessor.
		VectorDataAccessor GetDataAccessor();

		//! Returns the read-only data array accessor.
		ConstVectorDataAccessor GetConstDataAccessor() const;

		//! Returns the function that maps data point to its position.
		DataPositionFunc GetDataPosition() const;

		//!
		//! \brief Invokes the given function \p func for each data point.
		//!
		//! This function invokes the given function object \p func for each data
		//! point in serial manner. The input parameters are i and j indices of a
		//! data point. The order of execution is i-first, j-last.
		//!
		void ForEachDataPointIndex(const std::function<void(size_t, size_t)>& func) const;

		//!
		//! \brief Invokes the given function \p func for each data point in parallel.
		//!
		//! This function invokes the given function object \p func for each data
		//! point in parallel manner. The input parameters are i and j indices of a
		//! data point. The order of execution can be arbitrary since it's
		//! multi-threaded.
		//!
		void ParallelForEachDataPointIndex(const std::function<void(size_t, size_t)>& func) const;

		// VectorField2 implementations
		//! Returns sampled value at given position \p x.
		Vector2D Sample(const Vector2D& x) const override;

		//! Returns divergence at given position \p x.
		double Divergence(const Vector2D& x) const override;

		//! Returns curl at given position \p x.
		double Curl(const Vector2D& x) const override;

		//!
		//! \brief Returns the sampler function.
		//!
		//! This function returns the data sampler function object. The sampling
		//! function is linear.
		//!
		std::function<Vector2D(const Vector2D&)> Sampler() const override;

	protected:
		//! Swaps the data storage and predefined samplers with given grid.
		void SwapCollocatedVectorGrid(CollocatedVectorGrid2* other);

		//! Sets the data storage and predefined samplers with given grid.
		void SetCollocatedVectorGrid(const CollocatedVectorGrid2& other);
		
		//! Fetches the data into a continuous linear array.
		void GetData(std::vector<double>* data) const override;

		//! Sets the data from a continuous linear array.
		void SetData(const std::vector<double>& data) override;

	private:
		Array2<Vector2D> m_data;
		LinearArraySampler2<Vector2D, double> m_linearSampler;
		std::function<Vector2D(const Vector2D&)> m_sampler;

		void OnResize(
			const Size2& resolution,
			const Vector2D& gridSpacing,
			const Vector2D& origin,
			const Vector2D& initialValue) final;

		void ResetSampler();
	};

	//! Shared pointer for the CollocatedVectorGrid2 type.
	using CollocatedVectorGrid2Ptr = std::shared_ptr<CollocatedVectorGrid2>;
}

#endif