/*************************************************************************
> File Name: Animation.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for animation-related class.
> Created Time: 2017/04/19
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_ANIMATION_H
#define CUBBYFLOW_ANIMATION_H

#include <Core/Animation/Frame.h>

#include <memory>

namespace CubbyFlow
{
	//!
	//! \brief Abstract base class for animation-related class.
	//!
	//! This class represents the animation logic in very abstract level.
	//! Generally animation is a function of time and/or its previous state.
	//! This base class provides a virtual function update() which can be
	//! overriden by its sub-classes to implement their own state update logic.
	//!
	class Animation
	{
	public:
		Animation();

		virtual ~Animation();

		//!
		//! \brief Updates animation state for given \p frame.
		//!
		//! This function updates animation state by calling Animation::onUpdate
		//! function.
		//!
		void Update(const Frame& frame);

	protected:
		//!
		//! \brief The implementation of this function should update the animation
		//!     state for given Frame instance \p frame.
		//!
		//! This function is called from Animation::update when state of this class
		//! instance needs to be updated. Thus, the inherited class should override
		//! this function and implement its logic for updating the animation state.
		//!
		virtual void OnUpdate(const Frame& frame) = 0;
	};

	//! Shared pointer for the Animation type.
	using AnimationPtr = std::shared_ptr<Animation>;
}

#endif