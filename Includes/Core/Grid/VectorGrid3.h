/*************************************************************************
> File Name: VectorGrid3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for 3-D vector grid structure.
> Created Time: 2017/07/26
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_VECTOR_GRID3_H
#define CUBBYFLOW_VECTOR_GRID3_H

#include <Core/Array/ArrayAccessor3.hpp>
#include <Core/Field/VectorField3.hpp>
#include <Core/Grid/Grid3.h>

namespace CubbyFlow
{
	//! Abstract base class for 3-D vector grid structure.
	class VectorGrid3 : public VectorField3, public Grid3
	{
	public:
		//! Read-write array accessor type.
		using VectorDataAccessor = ArrayAccessor3<Vector3D>;

		//! Read-only array accessor type.
		using ConstVectorDataAccessor = ConstArrayAccessor3<Vector3D>;

		//! Constructs an empty grid.
		VectorGrid3();

		//! Default destructor.
		virtual ~VectorGrid3();

		//! Clears the contents of the grid.
		void Clear();

		//! Resizes the grid using given parameters.
		void Resize(
			size_t resolutionX, size_t resolutionY, size_t resolutionZ,
			double gridSpacingX = 1.0, double gridSpacingY = 1.0, double gridSpacingZ = 1.0,
			double originX = 0.0, double originY = 0.0, double originZ = 0.0,
			double initialValueX = 0.0, double initialValueY = 0.0, double initialValueZ = 0.0);

		//! Resizes the grid using given parameters.
		void Resize(
			const Size3& resolution,
			const Vector3D& gridSpacing = Vector3D(1, 1, 1),
			const Vector3D& origin = Vector3D(),
			const Vector3D& initialValue = Vector3D());

		//! Resizes the grid using given parameters.
		void Resize(
			double gridSpacingX, double gridSpacingY, double gridSpacingZ,
			double originX, double originY, double originZ);

		//! Resizes the grid using given parameters.
		void Resize(const Vector3D& gridSpacing, const Vector3D& origin);

		//! Fills the grid with given value.
		virtual void Fill(const Vector3D& value, ExecutionPolicy policy = ExecutionPolicy::Parallel) = 0;

		//! Fills the grid with given position-to-value mapping function.
		virtual void Fill(const std::function<Vector3D(const Vector3D&)>& func, ExecutionPolicy policy = ExecutionPolicy::Parallel) = 0;

		//! Returns the copy of the grid instance.
		virtual std::shared_ptr<VectorGrid3> Clone() const = 0;

		//! Serializes the grid instance to the output buffer.
		void Serialize(std::vector<uint8_t>* buffer) const override;

		//! Deserializes the input buffer to the grid instance.
		void Deserialize(const std::vector<uint8_t>& buffer) override;

	protected:
		//!
		//! \brief Invoked when the resizing happens.
		//!
		//! This callback function is called when the grid gets resized. The
		//! overriding class should allocate the internal storage based on its
		//! data layout scheme.
		//!
		virtual void OnResize(
			const Size3& resolution,
			const Vector3D& gridSpacing,
			const Vector3D& origin,
			const Vector3D& initialValue) = 0;
	};

	//! Shared pointer for the VectorGrid3 type.
	using VectorGrid3Ptr = std::shared_ptr<VectorGrid3>;

	//! Abstract base class for 3-D vector grid builder.
	class VectorGridBuilder3
	{
	public:
		//! Creates a builder.
		VectorGridBuilder3();

		//! Default destructor.
		virtual ~VectorGridBuilder3();

		//! Returns 3-D vector grid with given parameters.
		virtual VectorGrid3Ptr Build(
			const Size3& resolution,
			const Vector3D& gridSpacing,
			const Vector3D& gridOrigin,
			const Vector3D& initialVal) const = 0;
	};

	//! Shared pointer for the VectorGridBuilder3 type.
	using VectorGridBuilder3Ptr = std::shared_ptr<VectorGridBuilder3>;
}

#endif