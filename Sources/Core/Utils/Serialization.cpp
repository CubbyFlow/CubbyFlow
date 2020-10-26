// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Utils/Serialization.hpp>

#include <Flatbuffers/generated/FlatData_generated.h>

#include <vector>

namespace CubbyFlow
{
void Serialize(const Serializable* serializable, std::vector<uint8_t>* buffer)
{
    serializable->Serialize(buffer);
}

void Serialize(const uint8_t* data, size_t size, std::vector<uint8_t>* buffer)
{
    flatbuffers::FlatBufferBuilder builder(1024);

    const auto fbsData =
        fbs::CreateFlatData(builder, builder.CreateVector(data, size));

    builder.Finish(fbsData);

    uint8_t* buf = builder.GetBufferPointer();
    const size_t sz = builder.GetSize();

    buffer->resize(sz);
    memcpy(buffer->data(), buf, sz);
}

void Deserialize(const std::vector<uint8_t>& buffer, Serializable* serializable)
{
    serializable->Deserialize(buffer);
}

void Deserialize(const std::vector<uint8_t>& buffer, std::vector<uint8_t>* data)
{
    const auto fbsData = fbs::GetFlatData(buffer.data());
    data->resize(fbsData->data()->size());
    std::copy(fbsData->data()->begin(), fbsData->data()->end(), data->begin());
}
}  // namespace CubbyFlow