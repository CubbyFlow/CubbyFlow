#include "gtest/gtest.h"
#include <Vox/VoxScene.hpp>
#include <Vox/FileSystem.hpp>
#include <Vox/PerspectiveCamera.hpp>
#include "RenderTestsUtils.hpp"

using namespace CubbyFlow;

TEST(VoxScene, ParsingTest)
{
    const auto& scene = Vox::VoxScene::CreateScene("TestScene.json");
    const auto& camera = scene->GetSceneObject<Vox::PerspectiveCamera>("Camera");

    EXPECT_VECTOR3_NEAR(CubbyFlow::Vector3F(0.0f, 0.0f, 0.0f), camera->GetCameraOrigin(), 1e-3f);
    //! As camera target point in testscene.json is (0, 0, -1), camera direction must be negated.
    EXPECT_VECTOR3_NEAR(CubbyFlow::Vector3F(0.0f, 0.0f, 1.0f), camera->GetCameraDirection(), 1e-3f);
}