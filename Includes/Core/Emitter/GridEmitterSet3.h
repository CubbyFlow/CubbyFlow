/*************************************************************************
> File Name: GridEmitterSet3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D grid-based emitter set.
> Created Time: 2017/08/06
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_GRID_EMITTER_SET3_H
#define CUBBYFLOW_GRID_EMITTER_SET3_H

#include <Core/Emitter/GridEmitter3.h>

#include <vector>

namespace CubbyFlow
{
	//!
	//! \brief 3-D grid-based emitter set.
	//!
	class GridEmitterSet3 final : public GridEmitter3
	{
	public:
		class Builder;

		//! Constructs an emitter.
		GridEmitterSet3();

		//! Constructs an emitter with sub-emitters.
		explicit GridEmitterSet3(const std::vector<GridEmitter3Ptr>& emitters);

		//! Destructor.
		virtual ~GridEmitterSet3();

		//! Adds sub-emitter.
		void AddEmitter(const GridEmitter3Ptr& emitter);

		//! Returns builder fox GridEmitterSet3.
		static Builder GetBuilder();

	private:
		std::vector<GridEmitter3Ptr> m_emitters;

		void OnUpdate(double currentTimeInSeconds, double timeIntervalInSeconds) override;
	};

	//! Shared pointer type for the GridEmitterSet3.
	using GridEmitterSet3Ptr = std::shared_ptr<GridEmitterSet3>;
	
	//!
	//! \brief Front-end to create GridEmitterSet3 objects step by step.
	//!
	class GridEmitterSet3::Builder final
	{
	public:
		//! Returns builder with list of sub-emitters.
		Builder& WithEmitters(const std::vector<GridEmitter3Ptr>& emitters);

		//! Builds GridEmitterSet3.
		GridEmitterSet3 Build() const;

		//! Builds shared pointer of GridEmitterSet3 instance.
		GridEmitterSet3Ptr MakeShared() const;

	private:
		std::vector<GridEmitter3Ptr> m_emitters;
	};
}

#endif