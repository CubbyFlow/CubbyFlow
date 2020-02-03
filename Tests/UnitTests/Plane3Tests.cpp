#include "pch.h"

#include <Core/Geometry/Plane3.hpp>

using namespace CubbyFlow;

TEST(Plane3, Builder)
{
	Plane3 plane = Plane3::GetBuilder()
		.WithNormal({ 1, 0, 0 })
		.WithPoint({ 2, 3, 4 })
		.Build();

	EXPECT_EQ(Vector3D(1, 0, 0), plane.normal);
	EXPECT_EQ(Vector3D(2, 3, 4), plane.point);
}
