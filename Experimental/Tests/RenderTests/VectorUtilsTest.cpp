#include "gtest/gtest.h"
#include <Core/Vector/Vector3.h>
#include <Vox/VectorUtils.hpp>

#include "RenderTestsUtils.hpp"

using namespace CubbyFlow;

TEST(TriangleValidation, CheckTriangle)
{
    constexpr Vector3F v1(0.0f, 0.0f, 1.0f), v2(0.5f, 0.0f, 0.0f), v3(-0.5f, 0.0f, 0.0f);

    bool result = Vox::CheckTriangle(v1, v2, v3);
    EXPECT_EQ(true, result);
}

TEST(VectorCalculation, ComputeNormal)
{
    constexpr Vector3F v1(0.0f, 0.0f, 1.0f), v2(0.5f, 0.0f, 0.0f), v3(-0.5f, 0.0f, 0.0f);

    Vector3F normal = Vox::CalculateNormal(v1, v2, v3);
    EXPECT_VECTOR3_NEAR(Vector3F(0.0f, 1.0f, 0.0f), normal, 1e-5);
}

TEST(VectorCalculation, ComputeNormalReverseOrder)
{
    constexpr Vector3F v1(0.0f, 0.0f, 1.0f), v2(0.5f, 0.0f, 0.0f), v3(-0.5f, 0.0f, 0.0f);

    Vector3F normal = Vox::CalculateNormal(v3, v2, v1);
    EXPECT_VECTOR3_NEAR(Vector3F(0.0f, -1.0f, 0.0f), normal, 1e-5);
}