#include "pch.hpp"

#include <ManualTests.hpp>

using namespace CubbyFlow;

const size_t BUFFER_SIZE = 80;

static void UpdateWave(const double timeInterval, double* x, double* speed)
{
    (*x) += timeInterval * (*speed);

    // Boundary reflection
    if ((*x) > 1.0)
    {
        (*speed) *= -1.0;
        (*x) = 1.0 + timeInterval * (*speed);
    }
    else if ((*x) < 0.0)
    {
        (*speed) *= -1.0;
        (*x) = timeInterval * (*speed);
    }
}

static void AccumulateWaveToHeightField(const double x, const double waveLength,
                                        const double maxHeight,
                                        Array1<double>* heightField)
{
    const double quarterWaveLength = 0.25 * waveLength;
    const int start = static_cast<int>((x - quarterWaveLength) * BUFFER_SIZE);
    const int end = static_cast<int>((x + quarterWaveLength) * BUFFER_SIZE);

    for (int i = start; i < end; ++i)
    {
        int iNew = i;
        if (i < 0)
        {
            iNew = -i - 1;
        }
        else if (i >= static_cast<int>(BUFFER_SIZE))
        {
            iNew = 2 * BUFFER_SIZE - i - 1;
        }

        double distance = fabs((i + 0.5) / BUFFER_SIZE - x);
        double height =
            maxHeight * 0.5 *
            (cos(std::min(distance * M_PI / quarterWaveLength, M_PI)) + 1.0);
        (*heightField)[iNew] += height;
    }
}

CUBBYFLOW_TESTS(HelloFluidSim);

CUBBYFLOW_BEGIN_TEST_F(HelloFluidSim, Run)
{
    const double waveLengthX = 0.8;
    const double waveLengthY = 1.2;

    const double maxHeightX = 0.5;
    const double maxHeightY = 0.4;

    double x = 0.0;
    double y = 1.0;
    double speedX = 1.5;
    double speedY = -1.0;

    const int fps = 100;
    const double timeInterval = 1.0 / fps;

    Array1<double> heightField(BUFFER_SIZE);
    Array1<double> gridPoints(BUFFER_SIZE);
    char fileName[256];

    for (size_t i = 0; i < BUFFER_SIZE; ++i)
    {
        gridPoints[i] = 3.0 * static_cast<double>(i) / BUFFER_SIZE;
    }

    for (int i = 0; i < 500; ++i)
    {
        // March through time
        UpdateWave(timeInterval, &x, &speedX);
        UpdateWave(timeInterval, &y, &speedY);

        // Clear height field
        for (double& height : heightField)
        {
            height = 0.0;
        }

        // Accumulate waves for each center point
        (void)waveLengthX;
        (void)waveLengthY;
        (void)maxHeightX;
        (void)maxHeightY;
        AccumulateWaveToHeightField(x, waveLengthX, maxHeightX, &heightField);
        AccumulateWaveToHeightField(y, waveLengthY, maxHeightY, &heightField);

        snprintf(fileName, sizeof(fileName), "data.#line2,%04d,x.npy", i);
        SaveData(gridPoints.View(), fileName);
        snprintf(fileName, sizeof(fileName), "data.#line2,%04d,y.npy", i);
        SaveData(heightField.View(), fileName);
    }
}
CUBBYFLOW_END_TEST_F