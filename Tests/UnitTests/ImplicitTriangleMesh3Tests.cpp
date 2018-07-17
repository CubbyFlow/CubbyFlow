#include "pch.h"
#include "UnitTestsUtils.h"

#include <Geometry/Box3.h>
#include <Geometry/ImplicitTriangleMesh3.h>
#include <Surface/SurfaceToImplicit3.h>

#include <fstream>

using namespace CubbyFlow;

TEST(ImplicitTriangleMesh3, SignedDistance)
{
	auto box = Box3::Builder()
		.WithLowerCorner({ 0, 0, 0 })
		.WithUpperCorner({ 1, 1, 1 })
		.MakeShared();
	SurfaceToImplicit3 refSurf(box);

	std::ifstream objFile(RESOURCES_DIR "cube.obj");
	auto mesh = TriangleMesh3::Builder().MakeShared();
	mesh->ReadObj(&objFile);

	auto imesh = ImplicitTriangleMesh3::Builder()
		.WithTriangleMesh(mesh)
		.WithResolutionX(20)
		.MakeShared();

	for (size_t i = 0; i < GetNumberOfSamplePoints3(); ++i)
	{
		auto sample = GetSamplePoints3()[i];
		auto refAns = refSurf.SignedDistance(sample);
		auto actAns = imesh->SignedDistance(sample);

		EXPECT_NEAR(refAns, actAns, 1.0 / 20);
	}
}