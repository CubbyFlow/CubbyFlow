#pragma once

#include <Vox/App.hpp>
#include <Vox/GLTypes.hpp>

namespace Vox {
};

class TestApp : public Vox::App
{
public:
    //! Default Constructor
    TestApp();
    //! Default Destructor
    ~TestApp();
    
    //! Intiailize sample application.
    bool Initialize(const Vox::Path& scenePath) override;
    void DrawFrame() override;
    const char* GetWindowTitle() override { return "Test Application"; };

protected:
private:
};