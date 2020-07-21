/*************************************************************************
> File Name: DeviceTimer.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The timer for measure performance of gpu computation such as DMA.
> Created Time: 2020/07/21
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_DEVICE_TIMER_H
#define CUBBYFLOW_DEVICE_TIMER_H

#if defined(CUBBYFLOW_USE_GL)
    #include <GL3/Utils/GL3Timer.h>
#elif defined(CUBBYFLOW_USE_VULKAN)
    #include <Vulkan/Utils/VulkanTimer.h>
#elif defined(CUBBYFLOW_USE_DIRECTX)
    #include <DirectX/Utils/DxTimer.h>
#endif

namespace CubbyFlow {
namespace CubbyRender {
    
#if defined(CUBBYFLOW_USE_GL)
    using DeviceTimer = GL3Timer;
#elif defined(CUBBYFLOW_USE_VULKAN)
    using DeviceTimer = VulkanTimer;
#elif defined(CUBBYFLOW_USE_DIRECTX)
    using DeviceTimer = DxTimer;
#else
    class DeviceTimer
	{
	public:
        static_assert("Device Timer without any graphics api can't exist");
		DeviceTimer() {};
		double DurationInSeconds() const { return 0.0 };
		void Reset() {};
	};
#endif

}
}

#endif