#pragma once

#include <Vox/Core/App.hpp>
#include <Vox/Utils/GLTypes.hpp>

namespace Vox {
};

class TestApp : public Vox::App
{
public:
    //! Default Constructor
    TestApp(size_t shutdownTimer);
    //! Default Destructor
    ~TestApp();
    
    //! Intiailize sample application.
    bool Initialize(const Vox::Path& scenePath) override;
    void DrawFrame() override;
    const char* GetWindowTitle() override { return "Test Application"; };

protected:
private:
    size_t _shutdownTimer { 0 };
};