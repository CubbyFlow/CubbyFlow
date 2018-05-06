/*************************************************************************
> File Name: PhysicsAnimation.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Abstract base class for physics-based animation.
> Created Time: 2017/04/23
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Animation/PhysicsAnimation.h>
#include <Core/Utils/Logging.h>
#include <Core/Utils/Macros.h>
#include <Core/Utils/Timer.h>

namespace CubbyFlow
{
	PhysicsAnimation::PhysicsAnimation()
	{
		m_currentFrame.index = -1;
	}

	PhysicsAnimation::~PhysicsAnimation()
	{
		// Do nothing
	}

	bool PhysicsAnimation::GetIsUsingFixedSubTimeSteps() const
	{
		return m_isUsingFixedSubTimeSteps;
	}

	void PhysicsAnimation::SetIsUsingFixedSubTimeSteps(bool isUsing)
	{
		m_isUsingFixedSubTimeSteps = isUsing;
	}

	unsigned int PhysicsAnimation::GetNumberOfFixedSubTimeSteps() const
	{
		return m_numberOfFixedSubTimeSteps;
	}

	void PhysicsAnimation::SetNumberOfFixedSubTimeSteps(unsigned int numberOfSteps)
	{
		m_numberOfFixedSubTimeSteps = numberOfSteps;
	}

	void PhysicsAnimation::AdvanceSingleFrame()
	{
		Frame f = m_currentFrame;
		Update(++f);
	}

	Frame PhysicsAnimation::GetCurrentFrame() const
	{
		return m_currentFrame;
	}

	void PhysicsAnimation::SetCurrentFrame(const Frame& frame)
	{
		m_currentFrame = frame;
	}

	double PhysicsAnimation::GetCurrentTimeInSeconds() const
	{
		return m_currentTime;
	}

	unsigned int PhysicsAnimation::GetNumberOfSubTimeSteps(double timeIntervalInSeconds) const
	{
		UNUSED_VARIABLE(timeIntervalInSeconds);

		// Returns number of fixed sub-timesteps by default
		return m_numberOfFixedSubTimeSteps;
	}

	void PhysicsAnimation::OnUpdate(const Frame& frame)
	{
		if (frame.index > m_currentFrame.index)
		{
			if (m_currentFrame.index < 0)
			{
				Initialize();
			}

			int32_t numberOfFrames = frame.index - m_currentFrame.index;

			for (int32_t i = 0; i < numberOfFrames; ++i)
			{
				AdvanceTimeStep(frame.timeIntervalInSeconds);
			}

			m_currentFrame = frame;
		}
	}

	void PhysicsAnimation::AdvanceTimeStep(double timeIntervalInSeconds)
	{
		m_currentTime = m_currentFrame.TimeInSeconds();

		if (m_isUsingFixedSubTimeSteps)
		{
			CUBBYFLOW_INFO << "Using fixed sub-timesteps: " << m_numberOfFixedSubTimeSteps;

			// Perform fixed time-stepping
			const double actualTimeInterval	= timeIntervalInSeconds	/ static_cast<double>(m_numberOfFixedSubTimeSteps);

			for (unsigned int i = 0; i < m_numberOfFixedSubTimeSteps; ++i)
			{
				CUBBYFLOW_INFO << "Begin onAdvanceTimeStep: " << actualTimeInterval
					<< " (1/" << 1.0 / actualTimeInterval << ") seconds";

				Timer timer;
				OnAdvanceTimeStep(actualTimeInterval);

				CUBBYFLOW_INFO << "End onAdvanceTimeStep (took "
					<< timer.DurationInSeconds() << " seconds)";

				m_currentTime += actualTimeInterval;
			}
		}
		else
		{
			CUBBYFLOW_INFO << "Using adaptive sub-timesteps";

			// Perform adaptive time-stepping
			double remainingTime = timeIntervalInSeconds;
			while (remainingTime > std::numeric_limits<double>::epsilon())
			{
				unsigned int numSteps = GetNumberOfSubTimeSteps(remainingTime);
				double actualTimeInterval = remainingTime / static_cast<double>(numSteps);

				CUBBYFLOW_INFO << "Number of remaining sub-timesteps: " << numSteps;

				CUBBYFLOW_INFO << "Begin onAdvanceTimeStep: " << actualTimeInterval
					<< " (1/" << 1.0 / actualTimeInterval << ") seconds";

				Timer timer;
				OnAdvanceTimeStep(actualTimeInterval);

				CUBBYFLOW_INFO << "End onAdvanceTimeStep (took "
					<< timer.DurationInSeconds() << " seconds)";

				remainingTime -= actualTimeInterval;
				m_currentTime += actualTimeInterval;
			}
		}
	}

	void PhysicsAnimation::Initialize()
	{
		OnInitialize();
	}

	void PhysicsAnimation::OnInitialize()
	{
		// Do nothing
	}
}