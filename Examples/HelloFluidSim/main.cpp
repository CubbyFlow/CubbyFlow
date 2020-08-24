// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <string>
#include <thread>

const size_t BUFFER_SIZE = 80;
const std::string GRAY_SCALE_TABLE = " .:-=+*#%@";
const size_t GRAY_SCALE_TABLE_SIZE = GRAY_SCALE_TABLE.length();

void UpdateWave(const double timeInterval, double* x, double* speed)
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

void AccumulateWaveToHeightField(const double x, const double waveLength,
                                 const double maxHeight,
                                 std::array<double, BUFFER_SIZE>* heightField)
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

        const double distance = fabs((i + 0.5) / BUFFER_SIZE - x);
        const double height =
            maxHeight * 0.5 *
            (cos(std::min(distance * M_PI / quarterWaveLength, M_PI)) + 1.0);
        (*heightField)[iNew] += height;
    }
}

void Draw(const std::array<double, BUFFER_SIZE>& heightField)
{
    std::string buffer(BUFFER_SIZE, ' ');

    // Convert height field to grayscale
    for (size_t i = 0; i < BUFFER_SIZE; ++i)
    {
        const double height = heightField[i];
        const size_t tableIndex =
            std::min(static_cast<size_t>(floor(GRAY_SCALE_TABLE_SIZE * height)),
                     GRAY_SCALE_TABLE_SIZE - 1);
        buffer[i] = GRAY_SCALE_TABLE[tableIndex];
    }

    // Clear old prints
    for (size_t i = 0; i < BUFFER_SIZE; ++i)
    {
        printf("\b");
    }

    // Draw new buffer
    printf("%s", buffer.c_str());
    fflush(stdout);
}

int main()
{
    const double waveLengthX = 0.8;
    const double waveLengthY = 1.2;

    const double maxHeightX = 0.5;
    const double maxHeightY = 0.4;

    double x = 0.0;
    double y = 1.0;
    double speedX = 1.0;
    double speedY = -0.5;

    const int fps = 100;
    const double timeInterval = 1.0 / fps;

    std::array<double, BUFFER_SIZE> heightField;

    for (int i = 0; i < 1000; ++i)
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
        AccumulateWaveToHeightField(x, waveLengthX, maxHeightX, &heightField);
        AccumulateWaveToHeightField(y, waveLengthY, maxHeightY, &heightField);

        // Draw height field
        Draw(heightField);

        // Wait
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / fps));
    }

    printf("\n");
    fflush(stdout);

    return 0;
}