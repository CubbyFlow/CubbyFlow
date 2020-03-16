/*************************************************************************
> File Name: GL3Application.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: GLFW application implementation
> Created Time: 2020/02/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_GL3APPLICATION_H
#define CUBBYFLOW_GL3APPLICATION_H

#ifdef CUBBYFLOW_USE_GL

#include <Framework/Application.h>
#include <memory>
#include <string>

struct GLFWwindow;
using GLFWWindowPtr = GLFWwindow*;

namespace CubbyFlow {
namespace CubbyRender {

    class GL3Window;
    using GL3WindowPtr = std::shared_ptr<GL3Window>;

    //!
    //! \brief implementation of the abstract application with GLFW Library
    //!
    class GL3Application final : public Application
    {
    public: 
        //! Default Constructor.
        GL3Application();

        //! Default Destructor.
        ~GL3Application() override;

        //! Initializes the application.
        int initialize() override;

        //! Run the application with limited frames and fps.
        int run(int numberOfFrames, double fps, ScreenRecorderPtr recorder = nullptr) override;
        
        //! Create window and return it.
        WindowPtr createWindow(const std::string& title, int width, int height) override;

        //! Get application window.
        GL3WindowPtr& getApplicationWindow();
        const GL3WindowPtr& getApplicationWindow() const;

        //! Destroy the application.
        void terminate() override;
        
    protected:
        //! Validate the application whether the app can run simulation or not.
        bool validateApplication() override;

    private:
        static void onWindowResized(GLFWwindow* glfwWindow, int width, int height);
        static void onWindowMoved(GLFWwindow* glfwWindow, int width, int height);
        static void onKey(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods);
        static void onMouseButton(GLFWwindow* glfwWindow, int button, int action, int mods);
        static void onMouseCursorEnter(GLFWwindow* glfwWindow, int entered);
        static void onMouseCursorPos(GLFWwindow* glfwWindow, double x, double y);
        static void onMouseScroll(GLFWwindow* glfwWindow, double deltaX, double deltaY);
        static void onChar(GLFWwindow* glfwWindow, unsigned int code);
        static void onCharMods(GLFWwindow* glfwWindow, unsigned int code, int mods);
        static void onDrop(GLFWwindow* glfwWindow, int numDroppedFiles, const char** pathNames);
        static void onErrorEvent(int error, const char* description);

        GL3WindowPtr _window;
    };

    using GL3ApplicationPtr = std::shared_ptr<GL3Application>;
} 
}

#endif

#endif 