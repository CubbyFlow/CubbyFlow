/*************************************************************************
> File Name: GridEmitterSet2.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D grid-based emitter set.
> Created Time: 2017/08/06
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Emitter/GridEmitterSet2.h>

namespace CubbyFlow
{
	GridEmitterSet2::GridEmitterSet2()
	{
		// Do nothing
	}

	GridEmitterSet2::GridEmitterSet2(const std::vector<GridEmitter2Ptr>& emitters)
	{
		for (const auto& e : emitters)
		{
			AddEmitter(e);
		}
	}

	GridEmitterSet2::~GridEmitterSet2()
	{
		// Do nothing
	}

	void GridEmitterSet2::AddEmitter(const GridEmitter2Ptr& emitter)
	{
		m_emitters.push_back(emitter);
	}

	void GridEmitterSet2::OnUpdate(double currentTimeInSeconds, double timeIntervalInSeconds)
	{
		for (auto& emitter : m_emitters)
		{
			emitter->Update(currentTimeInSeconds, timeIntervalInSeconds);
		}
	}

	GridEmitterSet2::Builder GridEmitterSet2::GetBuilder()
	{
		return Builder();
	}
	
	GridEmitterSet2::Builder& GridEmitterSet2::Builder::WithEmitters(const std::vector<GridEmitter2Ptr>& emitters)
	{
		m_emitters = emitters;
		return *this;
	}

	GridEmitterSet2 GridEmitterSet2::Builder::Build() const
	{
		return GridEmitterSet2(m_emitters);
	}

	GridEmitterSet2Ptr GridEmitterSet2::Builder::MakeShared() const
	{
		return std::shared_ptr<GridEmitterSet2>(
			new GridEmitterSet2(m_emitters),
			[](GridEmitterSet2* obj)
		{
			delete obj;
		});
	}
}