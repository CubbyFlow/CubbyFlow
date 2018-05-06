/*************************************************************************
> File Name: GridEmitterSet3.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D grid-based emitter set.
> Created Time: 2017/08/06
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Emitter/GridEmitterSet3.h>

namespace CubbyFlow
{
	GridEmitterSet3::GridEmitterSet3()
	{
		// Do nothing
	}

	GridEmitterSet3::GridEmitterSet3(const std::vector<GridEmitter3Ptr>& emitters)
	{
		for (const auto& e : emitters)
		{
			AddEmitter(e);
		}
	}

	GridEmitterSet3::~GridEmitterSet3()
	{
		// Do nothing
	}

	void GridEmitterSet3::AddEmitter(const GridEmitter3Ptr& emitter)
	{
		m_emitters.push_back(emitter);
	}

	void GridEmitterSet3::OnUpdate(double currentTimeInSeconds, double timeIntervalInSeconds)
	{
		for (auto& emitter : m_emitters)
		{
			emitter->Update(currentTimeInSeconds, timeIntervalInSeconds);
		}
	}

	GridEmitterSet3::Builder GridEmitterSet3::GetBuilder()
	{
		return Builder();
	}
	
	GridEmitterSet3::Builder& GridEmitterSet3::Builder::WithEmitters(const std::vector<GridEmitter3Ptr>& emitters)
	{
		m_emitters = emitters;
		return *this;
	}

	GridEmitterSet3 GridEmitterSet3::Builder::Build() const
	{
		return GridEmitterSet3(m_emitters);
	}

	GridEmitterSet3Ptr GridEmitterSet3::Builder::MakeShared() const
	{
		return std::shared_ptr<GridEmitterSet3>(
			new GridEmitterSet3(m_emitters),
			[](GridEmitterSet3* obj)
		{
			delete obj;
		});
	}
}