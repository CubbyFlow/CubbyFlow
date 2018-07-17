#include "pch.h"
#include "UnitTestsUtils.h"

#include <Transform/Transform3.h>
#include <Utils/Constants.h>

using namespace CubbyFlow;

TEST(Transform3, Constructors)
{
	Transform3 t1;

	EXPECT_EQ(Vector3D(), t1.GetTranslation());
	EXPECT_EQ(0.0, t1.GetOrientation().Angle());

	Transform3 t2({ 2.0, -5.0, 1.0 }, QuaternionD({ 0.0, 1.0, 0.0 }, QUARTER_PI_DOUBLE));

	EXPECT_EQ(Vector3D(2.0, -5.0, 1.0), t2.GetTranslation());
	EXPECT_EQ(Vector3D(0.0, 1.0, 0.0), t2.GetOrientation().Axis());
	EXPECT_DOUBLE_EQ(QUARTER_PI_DOUBLE, t2.GetOrientation().Angle());
}

TEST(Transform3, Transform)
{
	Transform3 t({ 2.0, -5.0, 1.0 }, QuaternionD({ 0.0, 1.0, 0.0 }, HALF_PI_DOUBLE));

	auto r1 = t.ToWorld({ 4.0, 1.0, -3.0 });
	EXPECT_NEAR(-1.0, r1.x, 1e-9);
	EXPECT_NEAR(-4.0, r1.y, 1e-9);
	EXPECT_NEAR(-3.0, r1.z, 1e-9);

	auto r2 = t.ToLocal(r1);
	EXPECT_NEAR(4.0, r2.x, 1e-9);
	EXPECT_NEAR(1.0, r2.y, 1e-9);
	EXPECT_NEAR(-3.0, r2.z, 1e-9);

	auto r3 = t.ToWorldDirection({ 4.0, 1.0, -3.0 });
	EXPECT_NEAR(-3.0, r3.x, 1e-9);
	EXPECT_NEAR(1.0, r3.y, 1e-9);
	EXPECT_NEAR(-4.0, r3.z, 1e-9);

	auto r4 = t.ToLocalDirection(r3);
	EXPECT_NEAR(4.0, r4.x, 1e-9);
	EXPECT_NEAR(1.0, r4.y, 1e-9);
	EXPECT_NEAR(-3.0, r4.z, 1e-9);

	BoundingBox3D bbox({ -2, -1, -3 }, { 2, 1, 3 });
	auto r5 = t.ToWorld(bbox);
	EXPECT_BOUNDING_BOX3_NEAR(BoundingBox3D({ -1, -6, -1 }, { 5, -4, 3 }), r5, 1e-9);
	
	auto r6 = t.ToLocal(r5);
	EXPECT_BOUNDING_BOX3_EQ(bbox, r6);
}