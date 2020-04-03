#include "pch.h"

#include <Core/Animation/Frame.hpp>
#include <Core/Emitter/VolumeParticleEmitter3.hpp>
#include <Core/Geometry/Sphere3.hpp>
#include <Core/Surface/SurfaceToImplicit3.hpp>

using namespace CubbyFlow;

TEST(VolumeParticleEmitter3, Constructors)
{
	auto sphere = std::make_shared<SurfaceToImplicit3>(
		std::make_shared<Sphere3>(Vector3D(1.0, 2.0, 4.0), 3.0));

	VolumeParticleEmitter3 emitter(
		sphere,
		BoundingBox3D({ 0.0, 0.0, 0.0 }, { 3.0, 3.0, 3.0 }),
		0.1,
		{ -1.0, 0.5, 2.5 },
		30,
		0.01,
		false,
		true);

	EXPECT_EQ(0.01, emitter.GetJitter());
	EXPECT_FALSE(emitter.GetIsOneShot());
	EXPECT_TRUE(emitter.GetAllowOverlapping());
	EXPECT_EQ(30u, emitter.GetMaxNumberOfParticles());
	EXPECT_EQ(0.1, emitter.GetSpacing());
	EXPECT_EQ(-1.0, emitter.GetInitialVelocity().x);
	EXPECT_EQ(0.5, emitter.GetInitialVelocity().y);
	EXPECT_EQ(2.5, emitter.GetInitialVelocity().z);
}

TEST(VolumeParticleEmitter3, Emit)
{
	auto sphere = std::make_shared<SurfaceToImplicit3>(
		std::make_shared<Sphere3>(Vector3D(1.0, 2.0, 4.0), 3.0));

	BoundingBox3D box({ 0.0, 0.0, 0.0 }, { 3.0, 3.0, 3.0 });

	VolumeParticleEmitter3 emitter(
		sphere,
		box,
		0.5,
		{ -1.0, 0.5, 2.5 },
		30,
		0.0,
		false,
		false);

	auto particles = std::make_shared<ParticleSystemData3>();
	emitter.SetTarget(particles);

	Frame frame(0, 1.0);
	emitter.Update(frame.TimeInSeconds(), frame.timeIntervalInSeconds);

	auto pos = particles->GetPositions();
	auto vel = particles->GetVelocities();

	EXPECT_EQ(30u, particles->GetNumberOfParticles());
	for (size_t i = 0; i < particles->GetNumberOfParticles(); ++i)
	{
		EXPECT_GE(3.0, (pos[i] - Vector3D(1.0, 2.0, 4.0)).Length());
		EXPECT_TRUE(box.Contains(pos[i]));

		EXPECT_EQ(-1.0, vel[i].x);
		EXPECT_EQ(0.5, vel[i].y);
		EXPECT_EQ(2.5, vel[i].z);
	}

	++frame;
	emitter.SetMaxNumberOfParticles(80);
	emitter.Update(frame.TimeInSeconds(), frame.timeIntervalInSeconds);

	EXPECT_EQ(69u, particles->GetNumberOfParticles());

	pos = particles->GetPositions();
	for (size_t i = 0; i < particles->GetNumberOfParticles(); ++i)
	{
		pos[i] += Vector3D(2.0, 1.5, 5.0);
	}

	++frame;
	emitter.Update(frame.TimeInSeconds(), frame.timeIntervalInSeconds);
	EXPECT_LT(69u, particles->GetNumberOfParticles());
}

TEST(VolumeParticleEmitter3, Builder)
{
	auto sphere = std::make_shared<Sphere3>(Vector3D(1.0, 2.0, 4.0), 3.0);

	VolumeParticleEmitter3 emitter = VolumeParticleEmitter3::GetBuilder()
		.WithSurface(sphere)
		.WithMaxRegion(BoundingBox3D({ 0.0, 0.0, 0.0 }, { 3.0, 3.0, 3.0 }))
		.WithSpacing(0.1)
		.WithInitialVelocity({ -1.0, 0.5, 2.5 })
		.WithMaxNumberOfParticles(30)
		.WithJitter(0.01)
		.WithIsOneShot(false)
		.WithAllowOverlapping(true)
		.Build();

	EXPECT_EQ(0.01, emitter.GetJitter());
	EXPECT_FALSE(emitter.GetIsOneShot());
	EXPECT_TRUE(emitter.GetAllowOverlapping());
	EXPECT_EQ(30u, emitter.GetMaxNumberOfParticles());
	EXPECT_EQ(0.1, emitter.GetSpacing());
	EXPECT_EQ(-1.0, emitter.GetInitialVelocity().x);
	EXPECT_EQ(0.5, emitter.GetInitialVelocity().y);
	EXPECT_EQ(2.5, emitter.GetInitialVelocity().z);
}