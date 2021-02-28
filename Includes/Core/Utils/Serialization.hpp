// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_SERIALIZATION_HPP
#define CUBBYFLOW_SERIALIZATION_HPP

#include <Core/Array/ArrayView.hpp>

#include <vector>

namespace CubbyFlow
{
//! Abstract base class for any serializable class.
class Serializable
{
 public:
    //! Default constructor.
    Serializable() = default;

    //! Default copy constructor.
    Serializable(const Serializable&) = default;

    //! Default move constructor.
    Serializable(Serializable&&) noexcept = default;

    //! Default virtual destructor.
    virtual ~Serializable() = default;

    //! Default copy assignment operator.
    Serializable& operator=(const Serializable&) = default;

    //! Default move assignment operator.
    Serializable& operator=(Serializable&&) noexcept = default;

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
void Serialize(const ConstArrayView1<T>& array, std::vector<uint8_t>* buffer);

//! Deserializes buffer to serializable object.
void Deserialize(const std::vector<uint8_t>& buffer,
                 Serializable* serializable);

//! Deserializes buffer to data chunk using common schema.
void Deserialize(const std::vector<uint8_t>& buffer,
                 std::vector<uint8_t>* data);

//! Deserializes buffer to data chunk using common schema.
template <typename T>
void Deserialize(const std::vector<uint8_t>& buffer, Array1<T>* array);
}  // namespace CubbyFlow

#include <Core/Utils/Serialization-Impl.hpp>

#endif
