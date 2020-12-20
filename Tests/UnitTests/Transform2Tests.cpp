#include "UnitTestsUtils.hpp"
#include "pch.hpp"

#include <Core/Geometry/Transform2.hpp>
#include <Core/Utils/Constants.hpp>

using namespace CubbyFlow;

TEST(Transform2, Constructors)
{
    Transform2 t1;

    EXPECT_EQ(Vector2D(), t1.GetTranslation());
    EXPECT_EQ(0.0, t1.GetOrientation());

    Transform2 t2({ 2.0, -5.0 }, QUARTER_PI_DOUBLE);

    EXPECT_EQ(Vector2D(2.0, -5.0), t2.GetTranslation());
    EXPECT_EQ(QUARTER_PI_DOUBLE, t2.GetOrientation());
}

TEST(Transform2, Transform)
{
    Transform2 t({ 2.0, -5.0 }, HALF_PI_DOUBLE);

    auto r1 = t.ToWorld({ 4.0, 1.0 });
    EXPECT_DOUBLE_EQ(1.0, r1.x);
    EXPECT_DOUBLE_EQ(-1.0, r1.y);

    auto r2 = t.ToLocal(r1);
    EXPECT_DOUBLE_EQ(4.0, r2.x);
    EXPECT_DOUBLE_EQ(1.0, r2.y);

    auto r3 = t.ToWorldDirection({ 4.0, 1.0 });
    EXPECT_DOUBLE_EQ(-1.0, r3.x);
    EXPECT_DOUBLE_EQ(4.0, r3.y);

    auto r4 = t.ToLocalDirection(r3);
    EXPECT_DOUBLE_EQ(4.0, r4.x);
    EXPECT_DOUBLE_EQ(1.0, r4.y);

    BoundingBox2D bbox({ -2, -1 }, { 2, 1 });
    auto r5 = t.ToWorld(bbox);
    EXPECT_BOUNDING_BOX2_NEAR(BoundingBox2D({ 1, -7 }, { 3, -3 }), r5, 1e-9);

    auto r6 = t.ToLocal(r5);
    EXPECT_BOUNDING_BOX2_EQ(bbox, r6);
}