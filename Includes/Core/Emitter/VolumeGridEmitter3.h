/*************************************************************************
> File Name: VolumeGridEmitter3.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D grid-based volumetric emitter.
> Created Time: 2017/08/06
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_VOLUME_GRID_EMITTER3_H
#define CUBBYFLOW_VOLUME_GRID_EMITTER3_H

#include <Core/Emitter/GridEmitter3.h>
#include <Core/Grid/ScalarGrid3.h>
#include <Core/Grid/VectorGrid3.h>
#include <Core/Surface/ImplicitSurface3.h>
#include <Core/Vector/Vector3.h>

#include <memory>

namespace CubbyFlow
{
	//!
	//! \brief 3-D grid-based volumetric emitter.
	//!
	class VolumeGridEmitter3 final : public GridEmitter3
	{
	public:
		class Builder;

		//! Maps to a scalar value for given signed-dist, location, and old value.
		using ScalarMapper = std::function<double(double, const Vector3D&, double)>;

		//! Maps to a vector value for given signed-dist, location, and old value.
		using VectorMapper = std::function<Vector3D(double, const Vector3D&, const Vector3D&)>;

		//! Constructs an emitter with a source and is-one-shot flag.
		explicit VolumeGridEmitter3(const ImplicitSurface3Ptr& sourceRegion, bool isOneShot = true);

		//! Destructor.
		virtual ~VolumeGridEmitter3();

		//! Adds signed-distance target to the scalar grid.
		void AddSignedDistanceTarget(const ScalarGrid3Ptr& scalarGridTarget);

		//!
		//! \brief      Adds step function target to the scalar grid.
		//!
		//! \param[in]  scalarGridTarget The scalar grid target.
		//! \param[in]  minValue         The minimum value of the step function.
		//! \param[in]  maxValue         The maximum value of the step function.
		//!
		void AddStepFunctionTarget(const ScalarGrid3Ptr& scalarGridTarget, double minValue, double maxValue);

		//!
		//! \brief      Adds a scalar grid target.
		//!
		//! This function adds a custom target to the emitter. The first parameter
		//! defines which grid should it write to. The second parameter,
		//! \p customMapper, defines how to map signed-distance field from the
		//! volume geometry and location of the point to the final scalar value that
		//! is going to be written to the target grid. The third parameter defines
		//! how to blend the old value from the target grid and the new value from
		//! the mapper function.
		//!
		//! \param[in]  scalarGridTarget The scalar grid target
		//! \param[in]  customMapper     The custom mapper.
		//!
		void AddTarget(const ScalarGrid3Ptr& scalarGridTarget, const ScalarMapper& customMapper);

		//!
		//! \brief      Adds a vector grid target.
		//!
		//! This function adds a custom target to the emitter. The first parameter
		//! defines which grid should it write to. The second parameter,
		//! \p customMapper, defines how to map signed-distance field from the
		//! volume geometry and location of the point to the final vector value that
		//! is going to be written to the target grid. The third parameter defines
		//! how to blend the old value from the target grid and the new value from
		//! the mapper function.
		//!
		//! \param[in]  vectorGridTarget The vector grid target
		//! \param[in]  customMapper     The custom mapper.
		//!
		void AddTarget(const VectorGrid3Ptr& vectorGridTarget, const VectorMapper& customMapper);

		//! Returns implicit surface which defines the source region.
		const ImplicitSurface3Ptr& GetSourceRegion() const;

		//! Returns true if this emits only once.
		bool GetIsOneShot() const;

		//! Returns builder fox VolumeGridEmitter3.
		static Builder GetBuilder();

	private:
		using ScalarTarget = std::tuple<ScalarGrid3Ptr, ScalarMapper>;
		using VectorTarget = std::tuple<VectorGrid3Ptr, VectorMapper>;

		ImplicitSurface3Ptr m_sourceRegion;
		bool m_isOneShot = true;
		bool m_hasEmitted = false;
		std::vector<ScalarTarget> m_customScalarTargets;
		std::vector<VectorTarget> m_customVectorTargets;

		void OnUpdate(double currentTimeInSeconds, double timeIntervalInSeconds) override;

		void Emit();
	};

	//! Shared pointer type for the VolumeGridEmitter3.
	using VolumeGridEmitter3Ptr = std::shared_ptr<VolumeGridEmitter3>;
	
	//!
	//! \brief Front-end to create VolumeGridEmitter3 objects step by step.
	//!
	class VolumeGridEmitter3::Builder final
	{
	public:
		//! Returns builder with surface defining source region.
		Builder& WithSourceRegion(const Surface3Ptr& sourceRegion);

		//! Returns builder with one-shot flag.
		Builder& WithIsOneShot(bool isOneShot);

		//! Builds VolumeGridEmitter3.
		VolumeGridEmitter3 Build() const;

		//! Builds shared pointer of VolumeGridEmitter3 instance.
		VolumeGridEmitter3Ptr MakeShared() const;

	private:
		ImplicitSurface3Ptr m_sourceRegion;
		bool m_isOneShot = true;
	};
}

#endif