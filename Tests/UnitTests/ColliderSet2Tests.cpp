#include "pch.hpp"

#include <Core/Geometry/Box2.hpp>
#include <Core/Geometry/ColliderSet2.hpp>
#include <Core/Geometry/RigidBodyCollider2.hpp>

using namespace CubbyFlow;

TEST(ColliderSet2, Constructors)
{
    auto box1 = Box2::GetBuilder()
                    .WithLowerCorner({ 0, 1 })
                    .WithUpperCorner({ 1, 2 })
                    .MakeShared();

    auto box2 = Box2::GetBuilder()
                    .WithLowerCorner({ 2, 3 })
                    .WithUpperCorner({ 3, 4 })
                    .MakeShared();

    auto col1 = RigidBodyCollider2::GetBuilder().WithSurface(box1).MakeShared();

    auto col2 = RigidBodyCollider2::GetBuilder().WithSurface(box2).MakeShared();

    ColliderSet2 colSet1;
    EXPECT_EQ(0u, colSet1.NumberOfColliders());

    ColliderSet2 colSet2({ col1, col2 });
    EXPECT_EQ(2u, colSet2.NumberOfColliders());
    EXPECT_EQ(col1, colSet2.Collider(0));
    EXPECT_EQ(col2, colSet2.Collider(1));
}

TEST(ColliderSet2, Builder)
{
    auto box1 = Box2::GetBuilder()
                    .WithLowerCorner({ 0, 1 })
                    .WithUpperCorner({ 1, 2 })
                    .MakeShared();

    auto box2 = Box2::GetBuilder()
                    .WithLowerCorner({ 2, 3 })
                    .WithUpperCorner({ 3, 4 })
                    .MakeShared();

    auto col1 = RigidBodyCollider2::GetBuilder().WithSurface(box1).MakeShared();

    auto col2 = RigidBodyCollider2::GetBuilder().WithSurface(box2).MakeShared();

    auto colSet1 = ColliderSet2::GetBuilder().MakeShared();
    EXPECT_EQ(0u, colSet1->NumberOfColliders());

    auto colSet2 =
        ColliderSet2::GetBuilder().WithColliders({ col1, col2 }).MakeShared();
    EXPECT_EQ(2u, colSet2->NumberOfColliders());
    EXPECT_EQ(col1, colSet2->Collider(0));
    EXPECT_EQ(col2, colSet2->Collider(1));

    auto colSet3 = ColliderSet2::GetBuilder().Build();
    EXPECT_EQ(0u, colSet3.NumberOfColliders());
}