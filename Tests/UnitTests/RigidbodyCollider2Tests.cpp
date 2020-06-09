#include "pch.hpp"

#include <Core/Collider/RigidBodyCollider2.hpp>
#include <Core/Geometry/Plane2.hpp>
#include <Core/Surface/ImplicitSurfaceSet2.hpp>

using namespace CubbyFlow;

TEST(RigidBodyCollider2, ResolveCollision)
{
    // 1. No penetration
    {
        RigidBodyCollider2 collider(
            std::make_shared<Plane2>(Vector2D(0, 1), Vector2D(0, 0)));

        Vector2D newPosition(1, 0.1);
        Vector2D newVelocity(1, 0);
        double radius = 0.05;
        double restitutionCoefficient = 0.5;

        collider.ResolveCollision(radius, restitutionCoefficient, &newPosition,
                                  &newVelocity);

        EXPECT_DOUBLE_EQ(1.0, newPosition.x);
        EXPECT_DOUBLE_EQ(0.1, newPosition.y);
        EXPECT_DOUBLE_EQ(1.0, newVelocity.x);
        EXPECT_DOUBLE_EQ(0.0, newVelocity.y);
    }

    // 2. Penetration within radius
    {
        RigidBodyCollider2 collider(
            std::make_shared<Plane2>(Vector2D(0, 1), Vector2D(0, 0)));

        Vector2D newPosition(1, 0.1);
        Vector2D newVelocity(1, 0);
        double radius = 0.2;
        double restitutionCoefficient = 0.5;

        collider.ResolveCollision(radius, restitutionCoefficient, &newPosition,
                                  &newVelocity);

        EXPECT_DOUBLE_EQ(1.0, newPosition.x);
        EXPECT_DOUBLE_EQ(0.2, newPosition.y);
    }

    // 3. Sitting
    {
        RigidBodyCollider2 collider(
            std::make_shared<Plane2>(Vector2D(0, 1), Vector2D(0, 0)));

        Vector2D newPosition(1, 0.1);
        Vector2D newVelocity(1, 0);
        double radius = 0.1;
        double restitutionCoefficient = 0.5;

        collider.ResolveCollision(radius, restitutionCoefficient, &newPosition,
                                  &newVelocity);

        EXPECT_DOUBLE_EQ(1.0, newPosition.x);
        EXPECT_DOUBLE_EQ(0.1, newPosition.y);
        EXPECT_DOUBLE_EQ(1.0, newVelocity.x);
        EXPECT_DOUBLE_EQ(0.0, newVelocity.y);
    }

    // 4. Bounce-back
    {
        RigidBodyCollider2 collider(
            std::make_shared<Plane2>(Vector2D(0, 1), Vector2D(0, 0)));

        Vector2D newPosition(1, -1);
        Vector2D newVelocity(1, -1);
        double radius = 0.1;
        double restitutionCoefficient = 0.5;

        collider.ResolveCollision(radius, restitutionCoefficient, &newPosition,
                                  &newVelocity);

        EXPECT_DOUBLE_EQ(1.0, newPosition.x);
        EXPECT_DOUBLE_EQ(0.1, newPosition.y);
        EXPECT_DOUBLE_EQ(1.0, newVelocity.x);
        EXPECT_DOUBLE_EQ(restitutionCoefficient, newVelocity.y);
    }

    // 4. Friction
    {
        RigidBodyCollider2 collider(
            std::make_shared<Plane2>(Vector2D(0, 1), Vector2D(0, 0)));

        Vector2D newPosition(1, -1);
        Vector2D newVelocity(1, -1);
        double radius = 0.1;
        double restitutionCoefficient = 0.5;

        collider.SetFrictionCoefficient(0.1);

        collider.ResolveCollision(radius, restitutionCoefficient, &newPosition,
                                  &newVelocity);

        EXPECT_DOUBLE_EQ(1.0, newPosition.x);
        EXPECT_DOUBLE_EQ(0.1, newPosition.y);
        EXPECT_GT(1.0, newVelocity.x);
        EXPECT_DOUBLE_EQ(restitutionCoefficient, newVelocity.y);
    }
}

TEST(RigidBodyCollider2, VelocityAt)
{
    RigidBodyCollider2 collider(
        std::make_shared<Plane2>(Vector2D(0, 1), Vector2D(0, 0)));

    collider.GetSurface()->transform.SetTranslation({ -1, -2 });
    collider.GetSurface()->transform.SetOrientation(0.1);
    collider.linearVelocity = { 1, 3 };
    collider.angularVelocity = 4.0;

    Vector2D result = collider.VelocityAt({ 5, 7 });
    EXPECT_DOUBLE_EQ(-35.0, result.x);
    EXPECT_DOUBLE_EQ(27.0, result.y);
}

TEST(RigidBodyCollider2, Empty)
{
    RigidBodyCollider2 collider{ ImplicitSurfaceSet2::Builder().MakeShared() };

    Vector2D newPosition{ 1, 0.1 };
    Vector2D newVelocity{ 1, 0 };
    const double radius = 0.05;
    const double restitutionCoefficient = 0.5;

    collider.ResolveCollision(radius, restitutionCoefficient, &newPosition,
                              &newVelocity);

    EXPECT_DOUBLE_EQ(1.0, newPosition.x);
    EXPECT_DOUBLE_EQ(0.1, newPosition.y);
    EXPECT_DOUBLE_EQ(1.0, newVelocity.x);
    EXPECT_DOUBLE_EQ(0.0, newVelocity.y);
}