/*************************************************************************
> File Name: Matrix3x3.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D matrix class.
> Created Time: 2017/03/06
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_MATRIX3X3_H
#define CUBBYFLOW_MATRIX3X3_H

#include <Core/Matrix/Matrix.h>
#include <Core/Vector/Vector3.h>

#include <array>
#include <limits>

namespace CubbyFlow
{
	//!
	//! \brief 3-D matrix class.
	//!
	//! This class is a row-major 3-D matrix class, which means each element of
	//! the matrix is stored in order of (0, 0), (0, 1), (0, 2), (1, 0), (1, 1),
	//! (1, 2), (2, 0), (2, 1), and (2, 2).
	//!
	//! \tparam T - Type of the element.
	//!
	template <typename T>
	class Matrix<T, 3, 3>
	{
	public:
		static_assert(std::is_floating_point<T>::value, "Matrix only can be instantiated with floating point types");

		// MARK: Constructors
		//! Constructs identity matrix.
		Matrix();

		//! Constructs constant value matrix.
		explicit Matrix(T s);

		//! Constructs a matrix with input elements.
		//! \warning Ordering of the input elements is row-major.
		Matrix(
			T m00, T m01, T m02,
			T m10, T m11, T m12,
			T m20, T m21, T m22);

		//!
		//! \brief Constructs a matrix with given initializer list \p list.
		//!
		//! This constructor will build a matrix with given initializer list \p list
		//! such as
		//!
		//! \code{.cpp}
		//! Matrix<float, 3, 3> arr =
		//! {
		//!     {1.f, 2.f, 4.f},
		//!     {9.f, 3.f, 5.f},
		//!     {4.f, 8.f, 1.f}
		//! };
		//! \endcode
		//!
		//! Note the initializer also has 3x3 structure.
		//!
		//! \param list Initializer list that should be copy to the new matrix.
		//!
		template <typename U>
		Matrix(const std::initializer_list<std::initializer_list<U>>& list);

		//! Constructs a matrix with input matrix.
		Matrix(const Matrix& m);

		//! Constructs a matrix with input array.
		//! \warning Ordering of the input elements is row-major.
		explicit Matrix(const T* arr);

		// MARK: Basic setters
		//! Sets whole matrix with input scalar.
		void Set(T s);

		//! Sets this matrix with input elements.
		//! \warning Ordering of the input elements is row-major.
		void Set(
			T m00, T m01, T m02,
			T m10, T m11, T m12,
			T m20, T m21, T m22);

		//!
		//! \brief Sets a matrix with given initializer list \p list.
		//!
		//! This function will fill the matrix with given initializer list \p list
		//! such as
		//!
		//! \code{.cpp}
		//! Matrix<float, 3, 3> arr;
		//! arr.Set(
		//! {
		//!     {1.f, 2.f, 4.f},
		//!     {9.f, 3.f, 5.f},
		//!     {4.f, 8.f, 1.f}
		//! });
		//! \endcode
		//!
		//! Note the initializer also has 3x3 structure.
		//!
		//! \param list Initializer list that should be copy to the new matrix.
		//!
		template <typename U>
		void Set(const std::initializer_list<std::initializer_list<U>>& list);

		//! Copies from input matrix.
		void Set(const Matrix& m);

		//! Copies from input array.
		//! \warning Ordering of the input elements is row-major.
		void Set(const T* arr);

		//! Sets diagonal elements with input scalar.
		void SetDiagonal(T s);

		//! Sets off-diagonal elements with input scalar.
		void SetOffDiagonal(T s);

		//! Sets i-th row with input vector.
		void SetRow(size_t i, const Vector3<T>& row);

		//! Sets i-th column with input vector.
		void SetColumn(size_t i, const Vector3<T>& col);

		// MARK: Basic getters
		//! Returns true if this matrix is similar to the input matrix within the
		//! given tolerance.
		bool IsSimilar(const Matrix& m, double tol = std::numeric_limits<double>::epsilon()) const;

		//! Returns true if this matrix is a square matrix.
		bool IsSquare() const;

		//! Returns number of rows of this matrix.
		size_t Rows() const;

		//! Returns number of columns of this matrix.
		size_t Cols() const;

		//! Returns data pointer of this matrix.
		T* data();

		//! Returns constant pointer of this matrix.
		const T* data() const;

		// MARK: Binary operator methods - new instance = this instance (+) input
		//! Returns this matrix + input scalar.
		Matrix Add(T s) const;

		//! Returns this matrix + input matrix (element-wise).
		Matrix Add(const Matrix& m) const;

		//! Returns this matrix - input scalar.
		Matrix Sub(T s) const;

		//! Returns this matrix - input matrix (element-wise).
		Matrix Sub(const Matrix& m) const;

		//! Returns this matrix * input scalar.
		Matrix Mul(T s) const;

		//! Returns this matrix * input vector.
		Vector3<T> Mul(const Vector3<T>& v) const;

		//! Returns this matrix * input matrix.
		Matrix Mul(const Matrix& m) const;

		//! Returns this matrix / input scalar.
		Matrix Div(T s) const;

		// MARK: Binary operator methods - new instance = input (+) this instance
		//! Returns input scalar + this matrix.
		Matrix RAdd(T s) const;

		//! Returns input matrix + this matrix (element-wise).
		Matrix RAdd(const Matrix& m) const;

		//! Returns input scalar - this matrix.
		Matrix RSub(T s) const;

		//! Returns input matrix - this matrix (element-wise).
		Matrix RSub(const Matrix& m) const;

		//! Returns input scalar * this matrix.
		Matrix RMul(T s) const;

		//! Returns input matrix * this matrix.
		Matrix RMul(const Matrix& m) const;

		//! Returns input scalar / this matrix.
		Matrix RDiv(T s) const;

		// MARK: Augmented operator methods - this instance (+)= input
		//! Adds input scalar to this matrix.
		void IAdd(T s);

		//! Adds input matrix to this matrix (element-wise).
		void IAdd(const Matrix& m);

		//! Subtracts input scalar from this matrix.
		void ISub(T s);

		//! Subtracts input matrix from this matrix (element-wise).
		void ISub(const Matrix& m);

		//! Multiplies input scalar to this matrix.
		void IMul(T s);

		//! Multiplies input matrix to this matrix.
		void IMul(const Matrix& m);

		//! Divides this matrix with input scalar.
		void IDiv(T s);

		// MARK: Modifiers
		//! Transposes this matrix.
		void Transpose();

		//! Inverts this matrix.
		void Invert();

		// MARK: Complex getters
		//! Returns sum of all elements.
		T Sum() const;

		//! Returns average of all elements.
		T Avg() const;

		//! Returns minimum among all elements.
		T Min() const;

		//! Returns maximum among all elements.
		T Max() const;

		//! Returns absolute minimum among all elements.
		T AbsMin() const;

		//! Returns absolute maximum among all elements.
		T AbsMax() const;

		//! Returns sum of all diagonal elements.
		T Trace() const;

		//! Returns determinant of this matrix.
		T Determinant() const;

		//! Returns diagonal part of this matrix.
		Matrix Diagonal() const;

		//! Returns off-diagonal part of this matrix.
		Matrix OffDiagonal() const;

		//! Returns strictly lower triangle part of this matrix.
		Matrix StrictLowerTriangle() const;

		//! Returns strictly upper triangle part of this matrix.
		Matrix StrictUpperTriangle() const;

		//! Returns lower triangle part of this matrix (including the diagonal).
		Matrix LowerTriangle() const;

		//! Returns upper triangle part of this matrix (including the diagonal).
		Matrix UpperTriangle() const;

		//! Returns transposed matrix.
		Matrix Transposed() const;

		//! Returns inverse matrix.
		Matrix Inverse() const;

		template <typename U>
		Matrix<U, 3, 3> CastTo() const;

		// MARK: Setter operators
		//! Assigns input matrix.
		Matrix& operator=(const Matrix& m);

		//! Addition assignment with input scalar.
		Matrix& operator+=(T s);

		//! Addition assignment with input matrix (element-wise).
		Matrix& operator+=(const Matrix& m);

		//! Subtraction assignment with input scalar.
		Matrix& operator-=(T s);

		//! Subtraction assignment with input matrix (element-wise).
		Matrix& operator-=(const Matrix& m);

		//! Multiplication assignment with input scalar.
		Matrix& operator*=(T s);

		//! Multiplication assignment with input matrix.
		Matrix& operator*=(const Matrix& m);

		//! Division assignment with input scalar.
		Matrix& operator/=(T s);

		// MARK: Getter operators
		//! Returns reference of i-th element.
		T& operator[](size_t i);

		//! Returns constant reference of i-th element.
		const T& operator[](size_t i) const;

		//! Returns reference of (i,j) element.
		T& operator()(size_t i, size_t j);

		//! Returns constant reference of (i,j) element.
		const T& operator()(size_t i, size_t j) const;

		//! Returns true if is equal to m.
		bool operator==(const Matrix& m) const;

		//! Returns true if is not equal to m.
		bool operator!=(const Matrix& m) const;

		// MARK: Helpers
		//! Sets all matrix entries to zero.
		static Matrix MakeZero();

		//! Makes all diagonal elements to 1, and other elements to 0.
		static Matrix MakeIdentity();

		//! Makes scale matrix.
		static Matrix MakeScaleMatrix(T sx, T sy, T sz);

		//! Makes scale matrix.
		static Matrix MakeScaleMatrix(const Vector3<T>& s);

		//! Makes rotation matrix.
		//! \warning Input angle should be radian.
		static Matrix MakeRotationMatrix(const Vector3<T>& axis, T rad);

	private:
		std::array<T, 9> m_elements;
	};

	//! Type alias for 3x3 matrix.
	template <typename T> using Matrix3x3 = Matrix<T, 3, 3>;

	// Operator overloading
	//! Returns a matrix with opposite sign.
	template <typename T>
	Matrix<T, 3, 3> operator-(const Matrix<T, 3, 3>& a);

	//! Returns a + b (element-size).
	template <typename T>
	Matrix<T, 3, 3> operator+(const Matrix<T, 3, 3>& a, const Matrix<T, 3, 3>& b);

	//! Returns a + b', where every element of matrix b' is b.
	template <typename T>
	Matrix<T, 3, 3> operator+(const Matrix<T, 3, 3>& a, T b);

	//! Returns a' + b, where every element of matrix a' is a.
	template <typename T>
	Matrix<T, 3, 3> operator+(T a, const Matrix<T, 3, 3>& b);

	//! Returns a - b (element-size).
	template <typename T>
	Matrix<T, 3, 3> operator-(const Matrix<T, 3, 3>& a, const Matrix<T, 3, 3>& b);

	//! Returns a - b', where every element of matrix b' is b.
	template <typename T>
	Matrix<T, 3, 3> operator-(const Matrix<T, 3, 3>& a, T b);

	//! Returns a' - b, where every element of matrix a' is a.
	template <typename T>
	Matrix<T, 3, 3> operator-(T a, const Matrix<T, 3, 3>& b);

	//! Returns a * b', where every element of matrix b' is b.
	template <typename T>
	Matrix<T, 3, 3> operator*(const Matrix<T, 3, 3>& a, T b);

	//! Returns a' * b, where every element of matrix a' is a.
	template <typename T>
	Matrix<T, 3, 3> operator*(T a, const Matrix<T, 3, 3>& b);

	//! Returns a * b.
	template <typename T>
	Vector3<T> operator*(const Matrix<T, 3, 3>& a, const Vector3<T>& b);

	//! Returns a * b.
	template <typename T>
	Matrix<T, 3, 3> operator*(const Matrix<T, 3, 3>& a, const Matrix<T, 3, 3>& b);

	//! Returns a' / b, where every element of matrix a' is a.
	template <typename T>
	Matrix<T, 3, 3> operator/(const Matrix<T, 3, 3>& a, T b);

	//! Returns a / b', where every element of matrix b' is b.
	template <typename T>
	Matrix<T, 3, 3> operator/(T a, const Matrix<T, 3, 3>& b);

	//! Float-type 3x3 matrix.
	using Matrix3x3F = Matrix3x3<float>;

	//! Double-type 3x3 matrix.
	using Matrix3x3D = Matrix3x3<double>;
}

#include <Core/Matrix/Matrix3x3-Impl.h>

#endif