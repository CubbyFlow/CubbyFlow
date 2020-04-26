#include "pch.h"

#include <Core/Vector/Vector3.h>
#include <Framework/Utils/VectorCalculation.h>

#include "RenderTestsUtils.h"

using namespace CubbyFlow;
using namespace CubbyRender;

TEST(VectorCalculation, ComputeNormal)
{
    Vector3F v1(0.0f, 0.0f, 1.0f), v2(0.5f, 0.0f, 0.0f), v3(-0.5f, 0.0f, 0.0f);

    Vector3F normal = calculateNormal(v1, v2, v3);
    EXPECT_VECTOR3_NEAR(Vector3F(0.0f, 1.0f, 0.0f), normal, 1e-5);
}

TEST(VectorCalculation, ComputeNormalReverseOrder)
{
    Vector3F v1(0.0f, 0.0f, 1.0f), v2(0.5f, 0.0f, 0.0f), v3(-0.5f, 0.0f, 0.0f);

    Vector3F normal = calculateNormal(v3, v2, v1);
    EXPECT_VECTOR3_NEAR(Vector3F(0.0f, -1.0f, 0.0f), normal, 1e-5);
}