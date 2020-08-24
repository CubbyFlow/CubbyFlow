#include "pch.hpp"

#include <ManualTests.hpp>

#include <Core/Array/Array1.hpp>
#include <Core/Array/ArrayUtils.hpp>
#include <Core/Vector/Vector2.hpp>

using namespace CubbyFlow;

CUBBYFLOW_TESTS(ArrayUtils);

CUBBYFLOW_BEGIN_TEST_F(ArrayUtils, ExtrapolateToRegion2)
{
    Array2<double> data(128, 192, 0.0);
    Array2<char> valid(128, 192, 0);

    for (int j = 0; j < 192; ++j)
    {
        for (int i = 0; i < 128; ++i)
        {
            Vector2D pt(i / 128.0, j / 128.0);

            data(i, j) =
                std::sin(4 * PI_DOUBLE * pt.x) * std::sin(4 * PI_DOUBLE * pt.y);

            if (pt.DistanceTo(Vector2D(0.5, 0.5)) < 0.15 ||
                pt.DistanceTo(Vector2D(0.5, 0.9)) < 0.15)
            {
                valid(i, j) = 1;
            }
            else
            {
                valid(i, j) = 0;
            }
        }
    }

    SaveData(data.ConstAccessor(), "data0.npy");
    SaveData(valid.ConstAccessor(), "valid0.npy");

    ExtrapolateToRegion(data.ConstAccessor(), valid.ConstAccessor(), 10,
                        data.Accessor());

    SaveData(data.ConstAccessor(), "data10.npy");
    SaveData(valid.ConstAccessor(), "valid10.npy");
}
CUBBYFLOW_END_TEST_F