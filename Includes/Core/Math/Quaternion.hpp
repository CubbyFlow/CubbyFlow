// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_QUATERNION_HPP
#define CUBBYFLOW_QUATERNION_HPP

#include <Core/Matrix/Matrix.hpp>

namespace CubbyFlow
{
//!
//! \brief Quaternion class defined as q = w + xi + yj + zk.
//!
template <typename T>
class Quaternion
{
 public:
    static_assert(
        std::is_floating_point<T>::value,
        "Quaternion only can be instantiated with floating point types");

    //! Make an identity quaternion.
    Quaternion();

    //! Constructs a quaternion with given elements.
    Quaternion(T newW, T newX, T newY, T newZ);

    //! Constructs a quaternion with given elements.
    Quaternion(const std::initializer_list<T>& list);

    //! Constructs a quaternion with given rotation axis and angle.
    Quaternion(const Vector3<T>& axis, T angle);

    //! Constructs a quaternion with from and to vectors.
    Quaternion(const Vector3<T>& from, const Vector3<T>& to);

    //! Constructs a quaternion with three basis vectors.
    Quaternion(const Vector3<T>& axis0, const Vector3<T>& axis1,
               const Vector3<T>& axis2);

    //! Constructs a quaternion with 3x3 rotational matrix.
    explicit Quaternion(const Matrix3x3<T>& m33);

    //! Copy constructor.
    Quaternion(const Quaternion& other);

    //! Default move constructor.
    Quaternion(Quaternion&& other) noexcept;

    //! Default destructor.
    ~Quaternion() = default;

    //! Copy assignment operator.
    Quaternion& operator=(const Quaternion& other);

    //! Default move assignment operator.
    Quaternion& operator=(Quaternion&& other) noexcept;

    //! Sets the quaternion with other quaternion.
    void Set(const Quaternion& other);

    //! Sets the quaternion with given elements.
    void Set(T newW, T newX, T newY, T newZ);

    //! Sets the quaternion with given elements.
    void Set(const std::initializer_list<T>& list);

    //! Sets the quaternion with given rotation axis and angle.
    void Set(const Vector3<T>& axis, T angle);

    //! Sets the quaternion with from and to vectors.
    void Set(const Vector3<T>& from, const Vector3<T>& to);

    //! Sets quaternion with three basis vectors.
    void Set(const Vector3<T>& rotationBasis0, const Vector3<T>& rotationBasis1,
             const Vector3<T>& rotationBasis2);

    //! Sets the quaternion with 3x3 rotational matrix.
    void Set(const Matrix3x3<T>& m);

    //! Returns quaternion with other base type.
    template <typename U>
    [[nodiscard]] Quaternion<U> CastTo() const;

    //! Returns normalized quaternion.
    [[nodiscard]] Quaternion Normalized() const;

    //! Returns this quaternion * vector.
    [[nodiscard]] Vector3<T> Mul(const Vector3<T>& v) const;

    //! Returns this quaternion * other quaternion.
    [[nodiscard]] Quaternion Mul(const Quaternion& other) const;

    //! Computes the dot product with other quaternion.
    [[nodiscard]] T Dot(const Quaternion<T>& other) const;

    //! Returns other quaternion * this quaternion.
    [[nodiscard]] Quaternion RMul(const Quaternion& other) const;

    //! Returns this quaternion *= other quaternion.
    void IMul(const Quaternion& other);

    //! Makes this quaternion identity.
    void SetIdentity();

    //! Rotate this quaternion with given angle in radians.
    void Rotate(T angleInRadians);

    //! Normalizes the quaternion.
    void Normalize();

    //! Returns the rotational axis.
    [[nodiscard]] Vector3<T> Axis() const;

    //! Returns the rotational angle.
    [[nodiscard]] T Angle() const;

    //! Returns the axis and angle.
    void GetAxisAngle(Vector3<T>* axis, T* angle) const;

    //! Returns the inverse quaternion.
    [[nodiscard]] Quaternion Inverse() const;

    //! Converts to the 3x3 rotation matrix.
    [[nodiscard]] Matrix3x3<T> Matrix3() const;

    //! Converts to the 4x4 rotation matrix.
    [[nodiscard]] Matrix4x4<T> Matrix4() const;

    //! Returns L2 norm of this quaternion.
    [[nodiscard]] T L2Norm() const;

    //! Returns this quaternion *= other quaternion.
    Quaternion& operator*=(const Quaternion& other);

    //! Returns the reference to the i-th element.
    T& operator[](size_t i);

    //! Returns the const reference to the i-th element.
    const T& operator[](size_t i) const;

    //! Returns true if equal.
    bool operator==(const Quaternion& other) const;

    //! Returns true if not equal.
    bool operator!=(const Quaternion& other) const;

    //! Returns identity matrix.
    [[nodiscard]] static Quaternion MakeIdentity();

    //! Real part.
    T w;

    //!< Imaginary part (i).
    T x;

    //!< Imaginary part (j).
    T y;

    //!< Imaginary part (k).
    T z;
};

//! Computes spherical linear interpolation.
template <typename T>
[[nodiscard]] Quaternion<T> Slerp(const Quaternion<T>& a,
                                  const Quaternion<T>& b, T t);

//! Returns quaternion q * vector v.
template <typename T>
[[nodiscard]] Vector<T, 3> operator*(const Quaternion<T>& q,
                                     const Vector<T, 3>& v);

//! Returns quaternion a times quaternion b.
template <typename T>
[[nodiscard]] Quaternion<T> operator*(const Quaternion<T>& a,
                                      const Quaternion<T>& b);

//! Float-type quaternion.
using QuaternionF = Quaternion<float>;

//! Double-type quaternion.
using QuaternionD = Quaternion<double>;
}  // namespace CubbyFlow

#include <Core/Math/Quaternion-Impl.hpp>

#endif