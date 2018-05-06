#include "benchmark/benchmark.h"

#include <Core/BoundingBox/BoundingBox3.h>
#include <Core/Emitter/VolumeParticleEmitter3.h>
#include <Core/Geometry/Box3.h>
#include <Core/Particle/ParticleSystemData3.h>
#include <Core/Surface/ImplicitSurfaceSet3.h>

#include <memory>

using CubbyFlow::Box3;
using CubbyFlow::BoundingBox3D;
using CubbyFlow::ImplicitSurfaceSet3;
using CubbyFlow::ParticleSystemData3;

class VolumeParticleEmitter3 : public ::benchmark::Fixture
{
protected:
	CubbyFlow::VolumeParticleEmitter3Ptr emitter;

	void SetUp(const ::benchmark::State&)
	{
		const double dx = 0.2;
		double lx = 30.0;
		double ly = 30.0;
		double lz = 30.0;
		const double pd = 0.001;

		// Build emitter
		auto box1 = Box3::Builder()
			.WithLowerCorner({ 0, 0, 0 })
			.WithUpperCorner({ 0.5 * lx + pd, 0.75 * ly + pd, 0.75 * lz + pd })
			.MakeShared();

		auto box2 = Box3::Builder()
			.WithLowerCorner({ 2.5 * lx - pd, 0, 0.25 * lz - pd })
			.WithUpperCorner({ 3.5 * lx + pd, 0.75 * ly + pd, 1.5 * lz + pd })
			.MakeShared();

		auto boxSet = ImplicitSurfaceSet3::Builder()
			.WithExplicitSurfaces({ box1, box2 })
			.MakeShared();

		emitter = CubbyFlow::VolumeParticleEmitter3::Builder()
			.WithSurface(boxSet)
			.WithMaxRegion(BoundingBox3D({ 0, 0, 0 }, { lx, ly, lz }))
			.WithSpacing(0.5 * dx)
			.WithAllowOverlapping(true)
			.MakeShared();

		auto particles = std::make_shared<ParticleSystemData3>();
		emitter->SetTarget(particles);
	}
};

BENCHMARK_DEFINE_F(VolumeParticleEmitter3, Update)(benchmark::State& state)
{
	while (state.KeepRunning())
	{
		emitter->Update(0.0, 0.01);
	}
}

BENCHMARK_REGISTER_F(VolumeParticleEmitter3, Update);