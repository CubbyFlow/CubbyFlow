/*************************************************************************
> File Name: Serialization.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Abstract base class for any serializable class.
> Created Time: 2017/05/27
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Utils/Serialization.h>

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

		auto fbsData = fbs::CreateFlatData(builder, builder.CreateVector(data, size));

		builder.Finish(fbsData);

		uint8_t* buf = builder.GetBufferPointer();
		size_t sz = builder.GetSize();

		buffer->resize(sz);
		memcpy(buffer->data(), buf, sz);
	}

	void Deserialize(const std::vector<uint8_t>& buffer, Serializable* serializable)
	{
		serializable->Deserialize(buffer);
	}

	void Deserialize(const std::vector<uint8_t>& buffer, std::vector<uint8_t>* data)
	{
		auto fbsData = fbs::GetFlatData(buffer.data());
		data->resize(fbsData->data()->size());
		std::copy(fbsData->data()->begin(), fbsData->data()->end(), data->begin());
	}
}