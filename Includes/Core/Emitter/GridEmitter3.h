/*************************************************************************
> File Name: GridEmitter3.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Abstract base class for 3-D grid-based emitters.
> Created Time: 2017/08/06
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_GRID_EMITTER3_H
#define CUBBYFLOW_GRID_EMITTER3_H

#include <functional>
#include <memory>

namespace CubbyFlow
{
	//!
	//! \brief Abstract base class for 3-D grid-based emitters.
	//!
	class GridEmitter3
	{
	public:
		//!
		//! \brief Callback function type for update calls.
		//!
		//! This type of callback function will take the emitter pointer, current
		//! time, and time interval in seconds.
		//!
		using OnBeginUpdateCallback = std::function<void(GridEmitter3*, double, double)>;

		//! Constructs an emitter.
		GridEmitter3();

		//! Destructor.
		virtual ~GridEmitter3();

		//! Updates the emitter state from \p currentTimeInSeconds to the following
		//! time-step.
		void Update(double currentTimeInSeconds, double timeIntervalInSeconds);

		//!
		//! \brief      Sets the callback function to be called when
		//!             GridEmitter3::Update function is invoked.
		//!
		//! The callback function takes current simulation time in seconds unit. Use
		//! this callback to track any motion or state changes related to this
		//! emitter.
		//!
		//! \param[in]  callback The callback function.
		//!
		void SetOnBeginUpdateCallback(const OnBeginUpdateCallback& callback);

	protected:
		virtual void OnUpdate(double currentTimeInSeconds, double timeIntervalInSeconds) = 0;

		void CallOnBeginUpdateCallback(double currentTimeInSeconds, double timeIntervalInSeconds);

	private:
		OnBeginUpdateCallback m_onBeginUpdateCallback;
	};

	//! Shared pointer type for the GridEmitter3.
	using GridEmitter3Ptr = std::shared_ptr<GridEmitter3>;
}

#endif