/*************************************************************************
> File Name: VolumeGridEmitter3.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D grid-based volumetric emitter.
> Created Time: 2017/08/06
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Emitter/VolumeGridEmitter3.h>
#include <Core/Grid/CollocatedVectorGrid3.h>
#include <Core/Grid/FaceCenteredGrid3.h>
#include <Core/LevelSet/LevelSetUtils.h>
#include <Core/Surface/SurfaceToImplicit3.h>
#include <Core/Utils/Macros.h>

namespace CubbyFlow
{
	VolumeGridEmitter3::VolumeGridEmitter3(const ImplicitSurface3Ptr& sourceRegion, bool isOneShot) :
		m_sourceRegion(sourceRegion), m_isOneShot(isOneShot)
	{
		// Do nothing
	}

	VolumeGridEmitter3::~VolumeGridEmitter3()
	{
		// Do nothing
	}

	void VolumeGridEmitter3::AddSignedDistanceTarget(const ScalarGrid3Ptr& scalarGridTarget)
	{
		auto mapper = [](double sdf, const Vector3D&, double oldVal)
		{
			return std::min(oldVal, sdf);
		};

		AddTarget(scalarGridTarget, mapper);
	}

	void VolumeGridEmitter3::AddStepFunctionTarget(const ScalarGrid3Ptr& scalarGridTarget, double minValue, double maxValue)
	{
		double smoothingWidth = scalarGridTarget->GridSpacing().Min();
		auto mapper = [minValue, maxValue, smoothingWidth, scalarGridTarget](double sdf, const Vector3D&, double oldVal)
		{
			double step = 1.0 - SmearedHeavisideSDF(sdf / smoothingWidth);
			return std::max(oldVal, (maxValue - minValue) * step + minValue);
		};

		AddTarget(scalarGridTarget, mapper);
	}

	void VolumeGridEmitter3::AddTarget(const ScalarGrid3Ptr& scalarGridTarget, const ScalarMapper& customMapper)
	{
		m_customScalarTargets.emplace_back(scalarGridTarget, customMapper);
	}

	void VolumeGridEmitter3::AddTarget(const VectorGrid3Ptr& vectorGridTarget, const VectorMapper& customMapper)
	{
		m_customVectorTargets.emplace_back(vectorGridTarget, customMapper);
	}

	const ImplicitSurface3Ptr& VolumeGridEmitter3::GetSourceRegion() const
	{
		return m_sourceRegion;
	}

	bool VolumeGridEmitter3::GetIsOneShot() const
	{
		return m_isOneShot;
	}

	void VolumeGridEmitter3::OnUpdate(double currentTimeInSeconds, double timeIntervalInSeconds)
	{
		UNUSED_VARIABLE(currentTimeInSeconds);
		UNUSED_VARIABLE(timeIntervalInSeconds);

		if (m_hasEmitted && m_isOneShot)
		{
			return;
		}

		Emit();

		m_hasEmitted = true;
	}

	void VolumeGridEmitter3::Emit()
	{
		if (m_sourceRegion == nullptr)
		{
			return;
		}

		m_sourceRegion->UpdateQueryEngine();

		for (const auto& target : m_customScalarTargets)
		{
			const auto& grid = std::get<0>(target);
			const auto& mapper = std::get<1>(target);

			auto pos = grid->GetDataPosition();
			grid->ParallelForEachDataPointIndex([&](size_t i, size_t j, size_t k)
			{
				Vector3D gx = pos(i, j, k);
				double sdf = GetSourceRegion()->SignedDistance(gx);

				(*grid)(i, j, k) = mapper(sdf, gx, (*grid)(i, j, k));
			});
		}

		for (const auto& target : m_customVectorTargets)
		{
			const auto& grid = std::get<0>(target);
			const auto& mapper = std::get<1>(target);

			CollocatedVectorGrid3Ptr collocated = std::dynamic_pointer_cast<CollocatedVectorGrid3>(grid);
			if (collocated != nullptr)
			{
				auto pos = collocated->GetDataPosition();
				collocated->ParallelForEachDataPointIndex([&](size_t i, size_t j, size_t k)
				{
					Vector3D gx = pos(i, j, k);
					double sdf = GetSourceRegion()->SignedDistance(gx);

					if (IsInsideSDF(sdf))
					{
						(*collocated)(i, j, k) = mapper(sdf, gx, (*collocated)(i, j, k));
					}
				});

				continue;
			}

			FaceCenteredGrid3Ptr faceCentered = std::dynamic_pointer_cast<FaceCenteredGrid3>(grid);
			if (faceCentered != nullptr)
			{
				auto uPos = faceCentered->GetUPosition();
				auto vPos = faceCentered->GetVPosition();
				auto wPos = faceCentered->GetWPosition();

				faceCentered->ParallelForEachUIndex([&](size_t i, size_t j, size_t k)
				{
					Vector3D gx = uPos(i, j, k);
					double sdf = GetSourceRegion()->SignedDistance(gx);
					Vector3D oldVal = faceCentered->Sample(gx);
					Vector3D newVal = mapper(sdf, gx, oldVal);

					faceCentered->GetU(i, j, k) = newVal.x;
				});

				faceCentered->ParallelForEachVIndex([&](size_t i, size_t j, size_t k)
				{
					Vector3D gx = vPos(i, j, k);
					double sdf = GetSourceRegion()->SignedDistance(gx);
					Vector3D oldVal = faceCentered->Sample(gx);
					Vector3D newVal = mapper(sdf, gx, oldVal);

					faceCentered->GetV(i, j, k) = newVal.y;
				});

				faceCentered->ParallelForEachWIndex([&](size_t i, size_t j, size_t k)
				{
					Vector3D gx = wPos(i, j, k);
					double sdf = GetSourceRegion()->SignedDistance(gx);
					Vector3D oldVal = faceCentered->Sample(gx);
					Vector3D newVal = mapper(sdf, gx, oldVal);

					faceCentered->GetW(i, j, k) = newVal.z;
				});
			}
		}
	}

	VolumeGridEmitter3::Builder VolumeGridEmitter3::GetBuilder()
	{
		return Builder();
	}
	
	VolumeGridEmitter3::Builder& VolumeGridEmitter3::Builder::WithSourceRegion(const Surface3Ptr& sourceRegion)
	{
		auto implicit = std::dynamic_pointer_cast<ImplicitSurface3>(sourceRegion);
		if (implicit != nullptr)
		{
			m_sourceRegion = implicit;
		}
		else
		{
			m_sourceRegion = std::make_shared<SurfaceToImplicit3>(sourceRegion);
		}

		return *this;
	}

	VolumeGridEmitter3::Builder& VolumeGridEmitter3::Builder::WithIsOneShot(bool isOneShot)
	{
		m_isOneShot = isOneShot;
		return *this;
	}

	VolumeGridEmitter3 VolumeGridEmitter3::Builder::Build() const
	{
		return VolumeGridEmitter3(m_sourceRegion, m_isOneShot);
	}

	VolumeGridEmitter3Ptr VolumeGridEmitter3::Builder::MakeShared() const
	{
		return std::shared_ptr<VolumeGridEmitter3>(
			new VolumeGridEmitter3(m_sourceRegion, m_isOneShot),
			[](VolumeGridEmitter3* obj)
		{
			delete obj;
		});
	}
}