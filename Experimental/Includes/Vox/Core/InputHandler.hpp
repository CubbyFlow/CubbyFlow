/*************************************************************************
> File Name: InputHandler.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Input handle interfaces
> Created Time: 2020/12/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_INPUT_HANDLER_HPP
#define CUBBYFLOW_VOX_INPUT_HANDLER_HPP

#include <Core/Utils/Macros.hpp>

namespace Vox
{
    /**
     * Input handle interfaces
     */
    class InputHandler
    {
    public:
        InputHandler() = default;
        virtual ~InputHandler() {};

        virtual void SetWindowResized(int width, int height)
        {
            UNUSED_VARIABLE(width);
            UNUSED_VARIABLE(height);
        }
        virtual void SetKey(int key, int scancode, int action, int mods)
        {
            UNUSED_VARIABLE(key);
            UNUSED_VARIABLE(scancode);
            UNUSED_VARIABLE(action);
            UNUSED_VARIABLE(mods);
        }
        virtual void SetMouseButton(int button, int action, int mods)
        {
            UNUSED_VARIABLE(button);
            UNUSED_VARIABLE(action);
            UNUSED_VARIABLE(mods);
        }
        virtual void SetMouseCursorPos(double x, double y)
        {
            UNUSED_VARIABLE(x);
            UNUSED_VARIABLE(y);
        }
        virtual void SetMouseScroll(double deltaX, double deltaY)
        {
            UNUSED_VARIABLE(deltaX);
            UNUSED_VARIABLE(deltaY);
        }
    };

};  // namespace Vox

#endif