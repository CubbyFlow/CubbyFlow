#include "gtest/gtest.h"

#include <ManualTests.hpp>

#include <Core/Array/Array.hpp>
#include <Core/Particle/SPHKernels.hpp>

using namespace CubbyFlow;

CUBBYFLOW_TESTS(SPHStdKernel3);

CUBBYFLOW_BEGIN_TEST_F(SPHStdKernel3, Operator)
{
    Array1<double> x(101);
    Array1<double> y(101);

    double r1 = 1.0;
    SPHStdKernel3 kernel1(r1);

    for (int i = 0; i <= 100; ++i)
    {
        double t = 2.0 * (r1 * i / 100.0) - r1;
        x[i] = t;
        y[i] = kernel1(x[i]);
    }

    SaveData(x.View(), "kernel1.#line2,x.npy");
    SaveData(y.View(), "kernel1.#line2,y.npy");

    double r2 = 1.2;
    SPHStdKernel3 kernel2(r2);

    for (int i = 0; i <= 100; ++i)
    {
        double t = 2.0 * (r2 * i / 100.0) - r2;
        x[i] = t;
        y[i] = kernel2(x[i]);
    }

    SaveData(x.View(), "kernel2.#line2,x.npy");
    SaveData(y.View(), "kernel2.#line2,y.npy");

    double r3 = 1.5;
    SPHStdKernel3 kernel3(r3);

    for (int i = 0; i <= 100; ++i)
    {
        double t = 2.0 * (r3 * i / 100.0) - r3;
        x[i] = t;
        y[i] = kernel3(x[i]);
    }

    SaveData(x.View(), "kernel3.#line2,x.npy");
    SaveData(y.View(), "kernel3.#line2,y.npy");
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_TESTS(SPHSpikyKernel3);

CUBBYFLOW_BEGIN_TEST_F(SPHSpikyKernel3, Derivatives)
{
    Array1<double> x(101);
    Array1<double> y0(101);
    Array1<double> y1(101);
    Array1<double> y2(101);

    double r = 1.0;
    SPHSpikyKernel3 spiky(r);

    for (int i = 0; i <= 100; ++i)
    {
        double t = 2.0 * (r * i / 100.0) - r;
        x[i] = t;
        y0[i] = spiky(std::abs(x[i]));
        y1[i] = spiky.FirstDerivative(std::abs(x[i]));
        y2[i] = spiky.SecondDerivative(std::abs(x[i]));
    }

    SaveData(x.View(), "spiky.#line2,x.npy");
    SaveData(y0.View(), "spiky.#line2,y.npy");
    SaveData(x.View(), "spiky_1st.#line2,x.npy");
    SaveData(y1.View(), "spiky_1st.#line2,y.npy");
    SaveData(x.View(), "spiky_2nd.#line2,x.npy");
    SaveData(y2.View(), "spiky_2nd.#line2,y.npy");

    SPHStdKernel3 stdKernel(r);

    for (int i = 0; i <= 100; ++i)
    {
        double t = 2.0 * (r * i / 100.0) - r;
        x[i] = t;
        y0[i] = stdKernel(std::abs(x[i]));
        y1[i] = stdKernel.FirstDerivative(std::abs(x[i]));
        y2[i] = stdKernel.SecondDerivative(std::abs(x[i]));
    }

    SaveData(x.View(), "std.#line2,x.npy");
    SaveData(y0.View(), "std.#line2,y.npy");
    SaveData(x.View(), "std_1st.#line2,x.npy");
    SaveData(y1.View(), "std_1st.#line2,y.npy");
    SaveData(x.View(), "std_2nd.#line2,x.npy");
    SaveData(y2.View(), "std_2nd.#line2,y.npy");
}
CUBBYFLOW_END_TEST_F