/*************************************************************************
> File Name: GridEmitterSet2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D grid-based emitter set.
> Created Time: 2017/08/06
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_GRID_EMITTER_SET2_H
#define CUBBYFLOW_GRID_EMITTER_SET2_H

#include <Emitter/GridEmitter2.h>

#include <vector>

namespace CubbyFlow
{
	//!
	//! \brief 2-D grid-based emitter set.
	//!
	class GridEmitterSet2 final : public GridEmitter2
	{
	public:
		class Builder;

		//! Constructs an emitter.
		GridEmitterSet2();

		//! Constructs an emitter with sub-emitters.
		explicit GridEmitterSet2(const std::vector<GridEmitter2Ptr>& emitters);

		//! Destructor.
		virtual ~GridEmitterSet2();

		//! Adds sub-emitter.
		void AddEmitter(const GridEmitter2Ptr& emitter);

		//! Returns builder fox GridEmitterSet2.
		static Builder GetBuilder();

	private:
		std::vector<GridEmitter2Ptr> m_emitters;

		void OnUpdate(double currentTimeInSeconds, double timeIntervalInSeconds) override;
	};

	//! Shared pointer type for the GridEmitterSet2.
	using GridEmitterSet2Ptr = std::shared_ptr<GridEmitterSet2>;
	
	//!
	//! \brief Front-end to create GridEmitterSet2 objects step by step.
	//!
	class GridEmitterSet2::Builder final
	{
	public:
		//! Returns builder with list of sub-emitters.
		Builder& WithEmitters(const std::vector<GridEmitter2Ptr>& emitters);

		//! Builds GridEmitterSet2.
		GridEmitterSet2 Build() const;

		//! Builds shared pointer of GridEmitterSet2 instance.
		GridEmitterSet2Ptr MakeShared() const;

	private:
		std::vector<GridEmitter2Ptr> m_emitters;
	};
}

#endif