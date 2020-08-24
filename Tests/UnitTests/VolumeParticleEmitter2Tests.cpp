#include "UnitTestsUtils.hpp"
#include "pch.hpp"

#include <Core/Animation/Frame.hpp>
#include <Core/Emitter/VolumeParticleEmitter2.hpp>
#include <Core/Geometry/Sphere2.hpp>
#include <Core/Surface/SurfaceToImplicit2.hpp>

using namespace CubbyFlow;

TEST(VolumeParticleEmitter2, Constructors)
{
    auto sphere = std::make_shared<SurfaceToImplicit2>(
        std::make_shared<Sphere2>(Vector2D(1.0, 2.0), 3.0));

    BoundingBox2D region({ 0.0, 0.0 }, { 3.0, 3.0 });

    VolumeParticleEmitter2 emitter(sphere, region, 0.1, { -1.0, 0.5 },
                                   { 0.0, 0.0 }, 0.0, 30, 0.01, false, true);

    EXPECT_BOUNDING_BOX2_EQ(region, emitter.GetMaxRegion());
    EXPECT_EQ(0.01, emitter.GetJitter());
    EXPECT_FALSE(emitter.GetIsOneShot());
    EXPECT_TRUE(emitter.GetAllowOverlapping());
    EXPECT_EQ(30u, emitter.GetMaxNumberOfParticles());
    EXPECT_EQ(0.1, emitter.GetSpacing());
    EXPECT_EQ(-1.0, emitter.GetInitialVelocity().x);
    EXPECT_EQ(0.5, emitter.GetInitialVelocity().y);
    EXPECT_EQ(Vector2D(), emitter.GetLinearVelocity());
    EXPECT_EQ(0.0, emitter.GetAngularVelocity());
    EXPECT_TRUE(emitter.GetIsEnabled());
}

TEST(VolumeParticleEmitter2, Emit)
{
    auto sphere = std::make_shared<SurfaceToImplicit2>(
        std::make_shared<Sphere2>(Vector2D(1.0, 2.0), 3.0));

    BoundingBox2D box({ 0.0, 0.0 }, { 3.0, 3.0 });

    VolumeParticleEmitter2 emitter(sphere, box, 0.3, { -1.0, 0.5 },
                                   { 3.0, 4.0 }, 5.0, 30, 0.0, false, false);

    auto particles = std::make_shared<ParticleSystemData2>();
    emitter.SetTarget(particles);

    Frame frame(0, 1.0);
    emitter.Update(frame.TimeInSeconds(), frame.timeIntervalInSeconds);

    auto pos = particles->GetPositions();
    auto vel = particles->GetVelocities();

    EXPECT_EQ(30u, particles->GetNumberOfParticles());
    for (size_t i = 0; i < particles->GetNumberOfParticles(); ++i)
    {
        EXPECT_GE(3.0, (pos[i] - Vector2D(1.0, 2.0)).Length());
        EXPECT_TRUE(box.Contains(pos[i]));

        Vector2D r = pos[i];
        Vector2D w = 5.0 * Vector2D(-r.y, r.x);
        EXPECT_VECTOR2_NEAR(Vector2D(2.0, 4.5) + w, vel[i], 1e-9);
    }

    emitter.SetIsEnabled(false);
    ++frame;
    emitter.SetMaxNumberOfParticles(60);
    emitter.Update(frame.TimeInSeconds(), frame.timeIntervalInSeconds);

    EXPECT_EQ(30u, particles->GetNumberOfParticles());
    emitter.SetIsEnabled(true);

    ++frame;
    emitter.Update(frame.TimeInSeconds(), frame.timeIntervalInSeconds);

    EXPECT_EQ(51u, particles->GetNumberOfParticles());

    pos = particles->GetPositions();
    for (size_t i = 0; i < particles->GetNumberOfParticles(); ++i)
    {
        pos[i] += Vector2D(2.0, 1.5);
    }

    ++frame;
    emitter.Update(frame.TimeInSeconds(), frame.timeIntervalInSeconds);
    EXPECT_LT(51u, particles->GetNumberOfParticles());
}

TEST(VolumeParticleEmitter2, Builder)
{
    auto sphere = std::make_shared<Sphere2>(Vector2D(1.0, 2.0), 3.0);

    VolumeParticleEmitter2 emitter =
        VolumeParticleEmitter2::GetBuilder()
            .WithSurface(sphere)
            .WithMaxRegion(BoundingBox2D({ 0.0, 0.0 }, { 3.0, 3.0 }))
            .WithSpacing(0.1)
            .WithInitialVelocity({ -1.0, 0.5 })
            .WithLinearVelocity({ 3.0, 4.0 })
            .WithAngularVelocity(5.0)
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
    EXPECT_VECTOR2_EQ(Vector2D(3.0, 4.0), emitter.GetLinearVelocity());
    EXPECT_EQ(5.0, emitter.GetAngularVelocity());
}