/*************************************************************************
> File Name: FaceCenteredGrid3.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D face-centered (a.k.a MAC or staggered) grid.
> Created Time: 2017/08/03
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FACE_CENTERED_GRID3_H
#define CUBBYFLOW_FACE_CENTERED_GRID3_H

#include <Core/Array/Array3.h>
#include <Core/Array/ArraySamplers3.h>
#include <Core/Grid/VectorGrid3.h>

namespace CubbyFlow
{
	//!
	//! \brief 3-D face-centered (a.k.a MAC or staggered) grid.
	//!
	//! This class implements face-centered grid which is also known as
	//! marker-and-cell (MAC) or staggered grid. This vector grid stores each vector
	//! component at face center. Thus, u, v, and w components are not collocated.
	//!
	class FaceCenteredGrid3 final : public VectorGrid3
	{
	public:
		CUBBYFLOW_GRID3_TYPE_NAME(FaceCenteredGrid3)

		class Builder;

		//! Read-write scalar data accessor type.
		using ScalarDataAccessor = ArrayAccessor3<double>;

		//! Read-only scalar data accessor type.
		using ConstScalarDataAccessor = ConstArrayAccessor3<double>;

		//! Constructs empty grid.
		FaceCenteredGrid3();

		//! Resizes the grid using given parameters.
		FaceCenteredGrid3(
			size_t resolutionX, size_t resolutionY, size_t resolutionZ,
			double gridSpacingX = 1.0, double gridSpacingY = 1.0, double gridSpacingZ = 1.0,
			double originX = 0.0, double originY = 0.0, double originZ = 0.0,
			double initialValueU = 0.0, double initialValueV = 0.0, double initialValueW = 0.0);

		//! Resizes the grid using given parameters.
		FaceCenteredGrid3(
			const Size3& resolution,
			const Vector3D& gridSpacing = Vector3D(1.0, 1.0, 1.0),
			const Vector3D& origin = Vector3D(),
			const Vector3D& initialValue = Vector3D());

		//! Copy constructor.
		FaceCenteredGrid3(const FaceCenteredGrid3& other);

		//!
		//! \brief Swaps the contents with the given \p other grid.
		//!
		//! This function swaps the contents of the grid instance with the given
		//! grid object \p other only if \p other has the same type with this grid.
		//!
		void Swap(Grid3* other) override;

		//! Sets the contents with the given \p other grid.
		void Set(const FaceCenteredGrid3& other);

		//! Sets the contents with the given \p other grid.
		FaceCenteredGrid3& operator=(const FaceCenteredGrid3& other);

		//! Returns u-value at given data point.
		double& GetU(size_t i, size_t j, size_t k);

		//! Returns u-value at given data point.
		const double& GetU(size_t i, size_t j, size_t k) const;

		//! Returns v-value at given data point.
		double& GetV(size_t i, size_t j, size_t k);

		//! Returns v-value at given data point.
		const double& GetV(size_t i, size_t j, size_t k) const;

		//! Returns w-value at given data point.
		double& GetW(size_t i, size_t j, size_t k);

		//! Returns w-value at given data point.
		const double& GetW(size_t i, size_t j, size_t k) const;

		//! Returns interpolated value at cell center.
		Vector3D ValueAtCellCenter(size_t i, size_t j, size_t k) const;

		//! Returns divergence at cell-center location.
		double DivergenceAtCellCenter(size_t i, size_t j, size_t k) const;

		//! Returns curl at cell-center location.
		Vector3D CurlAtCellCenter(size_t i, size_t j, size_t k) const;

		//! Returns u data accessor.
		ScalarDataAccessor GetUAccessor();

		//! Returns read-only u data accessor.
		ConstScalarDataAccessor GetUConstAccessor() const;

		//! Returns v data accessor.
		ScalarDataAccessor GetVAccessor();

		//! Returns read-only v data accessor.
		ConstScalarDataAccessor GetVConstAccessor() const;

		//! Returns w data accessor.
		ScalarDataAccessor GetWAccessor();

		//! Returns read-only w data accessor.
		ConstScalarDataAccessor GetWConstAccessor() const;

		//! Returns function object that maps u data point to its actual position.
		DataPositionFunc GetUPosition() const;

		//! Returns function object that maps v data point to its actual position.
		DataPositionFunc GetVPosition() const;

		//! Returns function object that maps w data point to its actual position.
		DataPositionFunc GetWPosition() const;

		//! Returns data size of the u component.
		Size3 GetUSize() const;

		//! Returns data size of the v component.
		Size3 GetVSize() const;

		//! Returns data size of the w component.
		Size3 GetWSize() const;

		//!
		//! \brief Returns u-data position for the grid point at (0, 0, 0).
		//!
		//! Note that this is different from origin() since origin() returns
		//! the lower corner point of the bounding box.
		//!
		Vector3D GetUOrigin() const;

		//!
		//! \brief Returns v-data position for the grid point at (0, 0, 0).
		//!
		//! Note that this is different from origin() since origin() returns
		//! the lower corner point of the bounding box.
		//!
		Vector3D GetVOrigin() const;

		//!
		//! \brief Returns w-data position for the grid point at (0, 0, 0).
		//!
		//! Note that this is different from origin() since origin() returns
		//! the lower corner point of the bounding box.
		//!
		Vector3D GetWOrigin() const;

		//! Fills the grid with given value.
		void Fill(const Vector3D& value, ExecutionPolicy policy = ExecutionPolicy::Parallel) override;

		//! Fills the grid with given function.
		void Fill(const std::function<Vector3D(const Vector3D&)>& func, ExecutionPolicy policy = ExecutionPolicy::Parallel) override;

		//! Returns the copy of the grid instance.
		std::shared_ptr<VectorGrid3> Clone() const override;

		//!
		//! \brief Invokes the given function \p func for each u-data point.
		//!
		//! This function invokes the given function object \p func for each u-data
		//! point in serial manner. The input parameters are i and j indices of a
		//! u-data point. The order of execution is i-first, j-last.
		//!
		void ForEachUIndex(const std::function<void(size_t, size_t, size_t)>& func) const;

		//!
		//! \brief Invokes the given function \p func for each u-data point in parallel.
		//!
		//! This function invokes the given function object \p func for each u-data
		//! point in parallel manner. The input parameters are i and j indices of a
		//! u-data point. The order of execution can be arbitrary since it's
		//! multi-threaded.
		//!
		void ParallelForEachUIndex(const std::function<void(size_t, size_t, size_t)>& func) const;

		//!
		//! \brief Invokes the given function \p func for each v-data point.
		//!
		//! This function invokes the given function object \p func for each v-data
		//! point in serial manner. The input parameters are i and j indices of a
		//! v-data point. The order of execution is i-first, j-last.
		//!
		void ForEachVIndex(const std::function<void(size_t, size_t, size_t)>& func) const;

		//!
		//! \brief Invokes the given function \p func for each v-data point in parallel.
		//!
		//! This function invokes the given function object \p func for each v-data
		//! point in parallel manner. The input parameters are i and j indices of a
		//! v-data point. The order of execution can be arbitrary since it's
		//! multi-threaded.
		//!
		void ParallelForEachVIndex(const std::function<void(size_t, size_t, size_t)>& func) const;

		//!
		//! \brief Invokes the given function \p func for each w-data point.
		//!
		//! This function invokes the given function object \p func for each w-data
		//! point in serial manner. The input parameters are i and j indices of a
		//! w-data point. The order of execution is i-first, j-last.
		//!
		void ForEachWIndex(const std::function<void(size_t, size_t, size_t)>& func) const;

		//!
		//! \brief Invokes the given function \p func for each w-data point in parallel.
		//!
		//! This function invokes the given function object \p func for each w-data
		//! point in parallel manner. The input parameters are i and j indices of a
		//! w-data point. The order of execution can be arbitrary since it's
		//! multi-threaded.
		//!
		void ParallelForEachWIndex(const std::function<void(size_t, size_t, size_t)>& func) const;

		// VectorField3 implementations
		//! Returns sampled value at given position \p x.
		Vector3D Sample(const Vector3D& x) const override;

		//! Returns divergence at given position \p x.
		double Divergence(const Vector3D& x) const override;

		//! Returns curl at given position \p x.
		Vector3D Curl(const Vector3D& x) const override;

		//!
		//! \brief Returns the sampler function.
		//!
		//! This function returns the data sampler function object. The sampling
		//! function is linear.
		//!
		std::function<Vector3D(const Vector3D&)> Sampler() const override;

		//! Returns builder fox FaceCenteredGrid3.
		static Builder GetBuilder();

	protected:
		// VectorGrid3 implementations
		void OnResize(
			const Size3& resolution,
			const Vector3D& gridSpacing,
			const Vector3D& origin,
			const Vector3D& initialValue) final;

		//! Fetches the data into a continuous linear array.
		void GetData(std::vector<double>* data) const override;

		//! Sets the data from a continuous linear array.
		void SetData(const std::vector<double>& data) override;

	private:
		Array3<double> m_dataU;
		Array3<double> m_dataV;
		Array3<double> m_dataW;
		Vector3D m_dataOriginU;
		Vector3D m_dataOriginV;
		Vector3D m_dataOriginW;
		LinearArraySampler3<double, double> m_uLinearSampler;
		LinearArraySampler3<double, double> m_vLinearSampler;
		LinearArraySampler3<double, double> m_wLinearSampler;
		std::function<Vector3D(const Vector3D&)> m_sampler;

		void ResetSampler();
	};

	//! Shared pointer type for the FaceCenteredGrid3.
	using FaceCenteredGrid3Ptr = std::shared_ptr<FaceCenteredGrid3>;

	//!
	//! \brief Front-end to create CellCenteredScalarGrid3 objects step by step.
	//!
	class FaceCenteredGrid3::Builder final : public VectorGridBuilder3
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
		Builder& WithInitialValue(const Vector3D& initialVal);

		//! Returns builder with initial value.
		Builder& WithInitialValue(double initialValX, double initialValY, double initialValZ);

		//! Builds CellCenteredScalarGrid3 instance.
		FaceCenteredGrid3 Build() const;

		//! Builds shared pointer of FaceCenteredGrid3 instance.
		FaceCenteredGrid3Ptr MakeShared() const;

		//!
		//! \brief Builds shared pointer of FaceCenteredGrid3 instance.
		//!
		//! This is an overriding function that implements VectorGridBuilder3.
		//!
		VectorGrid3Ptr Build(
			const Size3& resolution,
			const Vector3D& gridSpacing,
			const Vector3D& gridOrigin,
			const Vector3D& initialVal) const override;

	private:
		Size3 m_resolution{ 1, 1, 1 };
		Vector3D m_gridSpacing{ 1, 1, 1 };
		Vector3D m_gridOrigin{ 0, 0, 0 };
		Vector3D m_initialVal{ 0, 0, 0 };
	};
}

#endif