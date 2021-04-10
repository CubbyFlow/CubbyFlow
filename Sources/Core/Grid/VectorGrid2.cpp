// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Grid/VectorGrid2.hpp>
#include <Core/Utils/FlatbuffersHelper.hpp>

#include <Flatbuffers/generated/VectorGrid2_generated.h>

namespace CubbyFlow
{
void VectorGrid2::Clear()
{
    Resize(Vector2UZ{}, GridSpacing(), Origin(), Vector2D{});
}

void VectorGrid2::Resize(size_t resolutionX, size_t resolutionY,
                         double gridSpacingX, double gridSpacingY,
                         double originX, double originY, double initialValueX,
                         double initialValueY)
{
    Resize(Vector2UZ{ resolutionX, resolutionY },
           Vector2D{ gridSpacingX, gridSpacingY }, Vector2D{ originX, originY },
           Vector2D{ initialValueX, initialValueY });
}

void VectorGrid2::Resize(const Vector2UZ& resolution,
                         const Vector2D& gridSpacing, const Vector2D& origin,
                         const Vector2D& initialValue)
{
    SetSizeParameters(resolution, gridSpacing, origin);

    OnResize(resolution, gridSpacing, origin, initialValue);
}

void VectorGrid2::Resize(double gridSpacingX, double gridSpacingY,
                         double originX, double originY)
{
    Resize(Vector2D{ gridSpacingX, gridSpacingY },
           Vector2D{ originX, originY });
}

void VectorGrid2::Resize(const Vector2D& gridSpacing, const Vector2D& origin)
{
    Resize(Resolution(), gridSpacing, origin);
}

void VectorGrid2::Serialize(std::vector<uint8_t>* buffer) const
{
    flatbuffers::FlatBufferBuilder builder{ 1024 };

    fbs::Vector2UZ fbsResolution = CubbyFlowToFlatbuffers(Resolution());
    fbs::Vector2D fbsGridSpacing = CubbyFlowToFlatbuffers(GridSpacing());
    fbs::Vector2D fbsOrigin = CubbyFlowToFlatbuffers(Origin());

    Array1<double> gridData;
    GetData(gridData);
    const flatbuffers::Offset<flatbuffers::Vector<double>> data =
        builder.CreateVector(gridData.data(), gridData.Length());

    const flatbuffers::Offset<fbs::VectorGrid2> fbsGrid = CreateVectorGrid2(
        builder, &fbsResolution, &fbsGridSpacing, &fbsOrigin, data);

    builder.Finish(fbsGrid);

    uint8_t* buf = builder.GetBufferPointer();
    const size_t size = builder.GetSize();

    buffer->resize(size);
    memcpy(buffer->data(), buf, size);
}

void VectorGrid2::Deserialize(const std::vector<uint8_t>& buffer)
{
    const fbs::VectorGrid2* fbsGrid = fbs::GetVectorGrid2(buffer.data());

    Resize(FlatbuffersToCubbyFlow(*fbsGrid->resolution()),
           FlatbuffersToCubbyFlow(*fbsGrid->gridSpacing()),
           FlatbuffersToCubbyFlow(*fbsGrid->origin()));

    const flatbuffers::Vector<double>* data = fbsGrid->data();
    Array1<double> gridData(data->size());
    std::copy(data->begin(), data->end(), gridData.begin());

    SetData(gridData);
}
}  // namespace CubbyFlow