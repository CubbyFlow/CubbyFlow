/*************************************************************************
> File Name: Serializable.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Serializable functions for CubbyFlow Python API.
> Created Time: 2018/02/01
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Utils/Serializable.h>
#include <Core/Utils/Serialization.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <fstream>

using namespace CubbyFlow;

void AddSerializable(pybind11::module& m)
{
	pybind11::class_<Serializable, std::shared_ptr<Serializable>>(m, "Serializable",
		R"pbdoc(
			Abstract base class for any serializable class.
		)pbdoc")
	.def("Serialize", [](const Serializable& instance)
	{
		std::vector<uint8_t> buffer;
		instance.Serialize(&buffer);
		return buffer;
	},
		R"pbdoc(
			Serializes this instance into the flat buffer.
		)pbdoc")
	.def("SerializeToFile", [](const Serializable& instance, const std::string& fileName)
	{
		std::vector<uint8_t> buffer;
		instance.Serialize(&buffer);
		std::ofstream file(fileName.c_str(), std::ios::binary);
		
		if (file)
		{
			file.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
			file.close();
		}
		else
		{
			throw std::invalid_argument("Cannot write to file.");
		}
	},
		R"pbdoc(
			Serializes this instance into the file.
		)pbdoc",
		pybind11::arg("fileName"))
	.def("Deserialize", &Serializable::Deserialize,
		R"pbdoc(
			Deserializes this instance from the flat buffer.
		)pbdoc",
		pybind11::arg("buffer"))
	.def("DeserializeFromFile", [](Serializable& instance, const std::string& fileName)
	{
		std::ifstream file(fileName.c_str(), std::ios::binary);
		if (file)
		{
			std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
			instance.Deserialize(buffer);
			file.close();
		}
		else
		{
			throw std::invalid_argument("Cannot write to file.");
		}
	},
		R"pbdoc(
			Deserializes this instance from the flat buffer.
		)pbdoc",
		pybind11::arg("fileName"));
}