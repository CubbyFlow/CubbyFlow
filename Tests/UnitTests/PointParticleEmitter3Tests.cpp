#include "pch.h"

#include <Core/Animation/Frame.hpp>
#include <Core/Emitter/PointParticleEmitter3.h>

using namespace CubbyFlow;

TEST(PointParticleEmitter3, Constructors)
{
	PointParticleEmitter3 emitter({ 1.0, 2.0, 3.0 }, Vector3D(0.5, 1.0, -2.0).Normalized(), 3.0, 15.0, 4, 18);

	EXPECT_EQ(4u, emitter.GetMaxNumberOfNewParticlesPerSecond());
	EXPECT_EQ(18u, emitter.GetMaxNumberOfParticles());
}

TEST(PointParticleEmitter3, Emit)
{
	Vector3D dir = Vector3D(0.5, 1.0, -2.0).Normalized();

	PointParticleEmitter3 emitter({ 1.0, 2.0, 3.0 }, dir, 3.0, 15.0, 4, 18);

	auto particles = std::make_shared<ParticleSystemData3>();
	emitter.SetTarget(particles);

	Frame frame(0, 1.0);
	emitter.Update(frame.TimeInSeconds(), frame.timeIntervalInSeconds);
	EXPECT_EQ(4u, particles->GetNumberOfParticles());

	frame.Advance();
	emitter.Update(frame.TimeInSeconds(), frame.timeIntervalInSeconds);
	EXPECT_EQ(8u, particles->GetNumberOfParticles());

	frame.Advance();
	emitter.Update(frame.TimeInSeconds(), frame.timeIntervalInSeconds);
	EXPECT_EQ(12u, particles->GetNumberOfParticles());

	frame.Advance();
	emitter.Update(frame.TimeInSeconds(), frame.timeIntervalInSeconds);
	EXPECT_EQ(16u, particles->GetNumberOfParticles());

	frame.Advance();
	emitter.Update(frame.TimeInSeconds(), frame.timeIntervalInSeconds);
	EXPECT_EQ(18u, particles->GetNumberOfParticles());

	auto pos = particles->GetPositions();
	auto vel = particles->GetVelocities();

	for (size_t i = 0; i < particles->GetNumberOfParticles(); ++i)
	{
		EXPECT_DOUBLE_EQ(1.0, pos[i].x);
		EXPECT_DOUBLE_EQ(2.0, pos[i].y);
		EXPECT_DOUBLE_EQ(3.0, pos[i].z);

		EXPECT_LE(std::cos(DegreesToRadians(15.0)), vel[i].Normalized().Dot(dir));
		EXPECT_DOUBLE_EQ(3.0, vel[i].Length());
	}
}

TEST(PointParticleEmitter3, Builder)
{
	PointParticleEmitter3 emitter = PointParticleEmitter3::GetBuilder()
		.WithOrigin({ 1.0, 2.0, 3.0 })
		.WithDirection(Vector3D(0.5, 1.0, -2.0).Normalized())
		.WithSpeed(3.0)
		.WithSpreadAngleInDegrees(15.0)
		.WithMaxNumberOfNewParticlesPerSecond(4)
		.WithMaxNumberOfParticles(18)
		.Build();

	EXPECT_EQ(4u, emitter.GetMaxNumberOfNewParticlesPerSecond());
	EXPECT_EQ(18u, emitter.GetMaxNumberOfParticles());
}