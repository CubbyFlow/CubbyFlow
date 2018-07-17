#include "pch.h"

#include <Utils/Timer.h>

#include <thread>

using namespace CubbyFlow;

TEST(Timer, Basics)
{
	Timer timer, timer2;
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	EXPECT_LT(0.01, timer.DurationInSeconds());

	timer.Reset();
	EXPECT_LE(timer.DurationInSeconds(), timer2.DurationInSeconds());
}
