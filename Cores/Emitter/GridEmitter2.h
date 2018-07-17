/*************************************************************************
> File Name: GridEmitter2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for 2-D grid-based emitters.
> Created Time: 2017/08/06
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_GRID_EMITTER2_H
#define CUBBYFLOW_GRID_EMITTER2_H

#include <functional>
#include <memory>

namespace CubbyFlow
{
	//!
	//! \brief Abstract base class for 2-D grid-based emitters.
	//!
	class GridEmitter2
	{
	public:
		//!
		//! \brief Callback function type for update calls.
		//!
		//! This type of callback function will take the emitter pointer, current
		//! time, and time interval in seconds.
		//!
		using OnBeginUpdateCallback = std::function<void(GridEmitter2*, double, double)>;

		//! Constructs an emitter.
		GridEmitter2();

		//! Destructor.
		virtual ~GridEmitter2();

		//! Updates the emitter state from \p currentTimeInSeconds to the following
		//! time-step.
		void Update(double currentTimeInSeconds, double timeIntervalInSeconds);

		//!
		//! \brief      Sets the callback function to be called when
		//!             GridEmitter2::Update function is invoked.
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

	//! Shared pointer type for the GridEmitter2.
	using GridEmitter2Ptr = std::shared_ptr<GridEmitter2>;
}

#endif