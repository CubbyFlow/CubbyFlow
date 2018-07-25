/*************************************************************************
> File Name: Serialization.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for any serializable class.
> Created Time: 2017/04/28
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_SERIALIZATION_H
#define CUBBYFLOW_SERIALIZATION_H

#include <Core/Array/Array1.h>

#include <vector>
#include <cstring>

namespace CubbyFlow
{
	//! Abstract base class for any serializable class.
	class Serializable
	{
	public:
		//! Serializes this instance into the flat buffer.
		virtual void Serialize(std::vector<uint8_t>* buffer) const = 0;

		//! Deserializes this instance from the flat buffer.
		virtual void Deserialize(const std::vector<uint8_t>& buffer) = 0;
	};

	//! Serializes serializable object.
	void Serialize(const Serializable* serializable, std::vector<uint8_t>* buffer);

	//! Serializes data chunk using common schema.
	void Serialize(const uint8_t* data, size_t size, std::vector<uint8_t>* buffer);

	//! Serializes data chunk using common schema.
	template <typename T>
	void Serialize(const ConstArrayAccessor1<T>& array, std::vector<uint8_t>* buffer);

	//! Deserializes buffer to serializable object.
	void Deserialize(const std::vector<uint8_t>& buffer, Serializable* serializable);

	//! Deserializes buffer to data chunk using common schema.
	void Deserialize(const std::vector<uint8_t>& buffer, std::vector<uint8_t>* data);

	//! Deserializes buffer to data chunk using common schema.
	template <typename T>
	void Deserialize(const std::vector<uint8_t>& buffer, Array1<T>* array);
}

#include <Core/Utils/Serialization-Impl.h>

#endif
