// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Grid/VectorGrid3.hpp>
#include <Core/Utils/FlatbuffersHelper.hpp>

#include <Flatbuffers/generated/VectorGrid3_generated.h>

namespace CubbyFlow
{
VectorGrid3::VectorGrid3()
{
    // Do nothing
}

VectorGrid3::~VectorGrid3()
{
    // Do nothing
}

void VectorGrid3::Clear()
{
    Resize(Size3(), GridSpacing(), Origin(), Vector3D());
}

void VectorGrid3::Resize(size_t resolutionX, size_t resolutionY,
                         size_t resolutionZ, double gridSpacingX,
                         double gridSpacingY, double gridSpacingZ,
                         double originX, double originY, double originZ,
                         double initialValueX, double initialValueY,
                         double initialValueZ)
{
    Resize(Size3(resolutionX, resolutionY, resolutionZ),
           Vector3D(gridSpacingX, gridSpacingY, gridSpacingZ),
           Vector3D(originX, originY, originZ),
           Vector3D(initialValueX, initialValueY, initialValueZ));
}

void VectorGrid3::Resize(const Size3& resolution, const Vector3D& gridSpacing,
                         const Vector3D& origin, const Vector3D& initialValue)
{
    SetSizeParameters(resolution, gridSpacing, origin);

    OnResize(resolution, gridSpacing, origin, initialValue);
}

void VectorGrid3::Resize(double gridSpacingX, double gridSpacingY,
                         double gridSpacingZ, double originX, double originY,
                         double originZ)
{
    Resize(Vector3D(gridSpacingX, gridSpacingY, gridSpacingZ),
           Vector3D(originX, originY, originZ));
}

void VectorGrid3::Resize(const Vector3D& gridSpacing, const Vector3D& origin)
{
    Resize(Resolution(), gridSpacing, origin);
}

void VectorGrid3::Serialize(std::vector<uint8_t>* buffer) const
{
    flatbuffers::FlatBufferBuilder builder(1024);

    auto fbsResolution = CubbyFlowToFlatbuffers(Resolution());
    auto fbsGridSpacing = CubbyFlowToFlatbuffers(GridSpacing());
    auto fbsOrigin = CubbyFlowToFlatbuffers(Origin());

    std::vector<double> gridData;
    GetData(&gridData);
    auto data = builder.CreateVector(gridData.data(), gridData.size());

    auto fbsGrid = fbs::CreateVectorGrid3(builder, &fbsResolution,
                                          &fbsGridSpacing, &fbsOrigin, data);

    builder.Finish(fbsGrid);

    uint8_t* buf = builder.GetBufferPointer();
    size_t size = builder.GetSize();

    buffer->resize(size);
    memcpy(buffer->data(), buf, size);
}

void VectorGrid3::Deserialize(const std::vector<uint8_t>& buffer)
{
    auto fbsGrid = fbs::GetVectorGrid3(buffer.data());

    Resize(FlatbuffersToCubbyFlow(*fbsGrid->resolution()),
           FlatbuffersToCubbyFlow(*fbsGrid->gridSpacing()),
           FlatbuffersToCubbyFlow(*fbsGrid->origin()));

    auto data = fbsGrid->data();
    std::vector<double> gridData(data->size());
    std::copy(data->begin(), data->end(), gridData.begin());

    SetData(gridData);
}

VectorGridBuilder3::VectorGridBuilder3()
{
    // Do nothing
}

VectorGridBuilder3::~VectorGridBuilder3()
{
    // Do nothing
}
}  // namespace CubbyFlow