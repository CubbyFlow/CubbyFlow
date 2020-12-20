#include "pch.hpp"

#include <Core/Particle/SPH/SPHStdKernel3.hpp>

using namespace CubbyFlow;

TEST(SPHStdKernel3, Constructors)
{
    SPHStdKernel3 kernel;
    EXPECT_DOUBLE_EQ(0.0, kernel.h);

    SPHStdKernel3 kernel2(3.0);
    EXPECT_DOUBLE_EQ(3.0, kernel2.h);
}

TEST(SPHStdKernel3, KernelFunction)
{
    SPHStdKernel3 kernel(10.0);

    double prevValue = kernel(0.0);

    for (int i = 1; i <= 10; ++i)
    {
        double value = kernel(static_cast<double>(i));
        EXPECT_LT(value, prevValue);
    }
}

TEST(SPHStdKernel3, FirstDerivative)
{
    const SPHStdKernel3 kernel{ 10.0 };

    const double value0 = kernel.FirstDerivative(0.0);
    const double value1 = kernel.FirstDerivative(5.0);
    const double value2 = kernel.FirstDerivative(10.0);
    EXPECT_DOUBLE_EQ(0.0, value0);
    EXPECT_DOUBLE_EQ(0.0, value2);

    // Compare with finite difference
    const double e = 0.001;
    const double fdm = (kernel(5.0 + e) - kernel(5.0 - e)) / (2.0 * e);
    EXPECT_NEAR(fdm, value1, 1e-10);
}

TEST(SPHStdKernel3, SecondDerivative)
{
    const SPHStdKernel3 kernel{ 10.0 };

    const double value0 = kernel.SecondDerivative(0.0);
    const double value1 = kernel.SecondDerivative(5.0);
    const double value2 = kernel.SecondDerivative(10.0);

    const double h5 = pow(10.0, 5.0);
    EXPECT_DOUBLE_EQ(-945.0 / (32.0 * PI_DOUBLE * h5), value0);
    EXPECT_DOUBLE_EQ(0.0, value2);

    // Compare with finite difference
    const double e = 0.001;
    const double fdm =
        (kernel(5.0 + e) - 2.0 * kernel(5.0) + kernel(5.0 - e)) / (e * e);
    EXPECT_NEAR(fdm, value1, 1e-10);
}

TEST(SPHStdKernel3, Gradient)
{
    SPHStdKernel3 kernel(10.0);

    Vector3D value0 = kernel.Gradient(0.0, Vector3D(1, 0, 0));
    EXPECT_DOUBLE_EQ(0.0, value0.x);
    EXPECT_DOUBLE_EQ(0.0, value0.y);
    EXPECT_DOUBLE_EQ(0.0, value0.z);

    Vector3D value1 = kernel.Gradient(5.0, Vector3D(0, 1, 0));
    EXPECT_DOUBLE_EQ(0.0, value1.x);
    EXPECT_LT(0.0, value1.y);
    EXPECT_DOUBLE_EQ(0.0, value1.z);

    Vector3D value2 = kernel.Gradient(Vector3D(0, 5, 0));
    EXPECT_EQ(value1, value2);
}

TEST(SPHSpikyKernel3, Constructors)
{
    SPHSpikyKernel3 kernel;
    EXPECT_DOUBLE_EQ(0.0, kernel.h);

    SPHSpikyKernel3 kernel2(3.0);
    EXPECT_DOUBLE_EQ(3.0, kernel2.h);
}

TEST(SPHSpikyKernel3, KernelFunction)
{
    SPHSpikyKernel3 kernel(10.0);

    double prevValue = kernel(0.0);

    for (int i = 1; i <= 10; ++i)
    {
        double value = kernel(static_cast<double>(i));
        EXPECT_LT(value, prevValue);
    }
}

TEST(SPHSpikyKernel3, FirstDerivative)
{
    SPHSpikyKernel3 kernel(10.0);

    double value0 = kernel.FirstDerivative(0.0);
    double value1 = kernel.FirstDerivative(5.0);
    double value2 = kernel.FirstDerivative(10.0);
    EXPECT_LT(value0, value1);
    EXPECT_LT(value1, value2);

    // Compare with finite difference
    const double e = 0.001;
    double fdm = (kernel(5.0 + e) - kernel(5.0 - e)) / (2.0 * e);
    EXPECT_NEAR(fdm, value1, 1e-10);
}

TEST(SPHSpikyKernel3, Gradient)
{
    SPHSpikyKernel3 kernel(10.0);

    Vector3D value0 = kernel.Gradient(0.0, Vector3D(1, 0, 0));
    EXPECT_LT(0.0, value0.x);
    EXPECT_DOUBLE_EQ(0.0, value0.y);
    EXPECT_DOUBLE_EQ(0.0, value0.z);

    Vector3D value1 = kernel.Gradient(5.0, Vector3D(0, 1, 0));
    EXPECT_DOUBLE_EQ(0.0, value1.x);
    EXPECT_LT(0.0, value1.y);
    EXPECT_DOUBLE_EQ(0.0, value1.z);

    Vector3D value2 = kernel.Gradient(Vector3D(0, 5, 0));
    EXPECT_EQ(value1, value2);
}

TEST(SPHSpikyKernel3, SecondDerivative)
{
    const SPHSpikyKernel3 kernel{ 10.0 };

    const double value0 = kernel.SecondDerivative(0.0);
    const double value1 = kernel.SecondDerivative(5.0);
    const double value2 = kernel.SecondDerivative(10.0);

    const double h5 = pow(10.0, 5.0);
    EXPECT_DOUBLE_EQ(90.0 / (PI_DOUBLE * h5), value0);
    EXPECT_DOUBLE_EQ(0.0, value2);

    // Compare with finite difference
    const double e = 0.001;
    const double fdm =
        (kernel(5.0 + e) - 2.0 * kernel(5.0) + kernel(5.0 - e)) / (e * e);
    EXPECT_NEAR(fdm, value1, 1e-10);
}