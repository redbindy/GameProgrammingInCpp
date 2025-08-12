// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once

#include <cmath>
#include <memory.h>
#include <limits>

namespace Math
{
	constexpr float PI = 3.1415926535f;
	constexpr float TWO_PI = PI * 2.0f;
	constexpr float PI_OVER_2 = PI / 2.0f;
	constexpr float Infinity = std::numeric_limits<float>::infinity();
	constexpr float NegInfinity = -std::numeric_limits<float>::infinity();

	inline float ToRadians(float degrees)
	{
		return degrees * PI / 180.0f;
	}

	inline float ToDegrees(float radians)
	{
		return radians * 180.0f / PI;
	}

	inline bool IsNearZero(float val, float epsilon = 0.001f)
	{
		return fabs(val) <= epsilon;
	}

	template <typename T>
	T Max(const T& a, const T& b)
	{
		return (a < b ? b : a);
	}

	template <typename T>
	T Min(const T& a, const T& b)
	{
		return (a < b ? a : b);
	}

	template <typename T>
	T Clamp(const T& value, const T& lower, const T& upper)
	{
		return Min(upper, Max(lower, value));
	}

	inline float Abs(float value)
	{
		return fabs(value);
	}

	inline float Cos(float angle)
	{
		return cosf(angle);
	}

	inline float Sin(float angle)
	{
		return sinf(angle);
	}

	inline float Tan(float angle)
	{
		return tanf(angle);
	}

	inline float Acos(float value)
	{
		return acosf(value);
	}

	inline float Atan2(float y, float x)
	{
		return atan2f(y, x);
	}

	inline float Cot(float angle)
	{
		return 1.0f / Tan(angle);
	}

	inline float Lerp(float a, float b, float f)
	{
		return a + f * (b - a);
	}

	inline float Sqrt(float value)
	{
		return sqrtf(value);
	}

	inline float Fmod(float numer, float denom)
	{
		return fmod(numer, denom);
	}
}

// 2D Vector
class Vector2
{
public:
	float X;
	float Y;

	Vector2()
		:X(0.0f)
		, Y(0.0f)
	{
	}

	explicit Vector2(float inX, float inY)
		:X(inX)
		, Y(inY)
	{
	}

	// Set both components in one line
	void Set(float inX, float inY)
	{
		X = inX;
		Y = inY;
	}

	// Vector addition (a + b)
	friend Vector2 operator+(const Vector2& a, const Vector2& b)
	{
		return Vector2(a.X + b.X, a.Y + b.Y);
	}

	// Vector subtraction (a - b)
	friend Vector2 operator-(const Vector2& a, const Vector2& b)
	{
		return Vector2(a.X - b.X, a.Y - b.Y);
	}

	// Component-wise multiplication
	// (a.x * b.x, ...)
	friend Vector2 operator*(const Vector2& a, const Vector2& b)
	{
		return Vector2(a.X * b.X, a.Y * b.Y);
	}

	// Scalar multiplication
	friend Vector2 operator*(const Vector2& vec, float scalar)
	{
		return Vector2(vec.X * scalar, vec.Y * scalar);
	}

	// Scalar multiplication
	friend Vector2 operator*(float scalar, const Vector2& vec)
	{
		return Vector2(vec.X * scalar, vec.Y * scalar);
	}

	// Scalar *=
	Vector2& operator*=(float scalar)
	{
		X *= scalar;
		Y *= scalar;
		return *this;
	}

	// Vector +=
	Vector2& operator+=(const Vector2& right)
	{
		X += right.X;
		Y += right.Y;
		return *this;
	}

	// Vector -=
	Vector2& operator-=(const Vector2& right)
	{
		X -= right.X;
		Y -= right.Y;
		return *this;
	}

	// Length squared of vector
	float LengthSq() const
	{
		return (X * X + Y * Y);
	}

	// Length of vector
	float Length() const
	{
		return (Math::Sqrt(LengthSq()));
	}

	// Normalize this vector
	void Normalize()
	{
		float length = Length();
		X /= length;
		Y /= length;
	}

	// Normalize the provided vector
	static Vector2 Normalize(const Vector2& vec)
	{
		Vector2 temp = vec;
		temp.Normalize();
		return temp;
	}

	// Dot product between two vectors (a dot b)
	static float Dot(const Vector2& a, const Vector2& b)
	{
		return (a.X * b.X + a.Y * b.Y);
	}

	// Lerp from A to B by f
	static Vector2 Lerp(const Vector2& a, const Vector2& b, float f)
	{
		return Vector2(a + f * (b - a));
	}

	// Reflect V about (normalized) N
	static Vector2 Reflect(const Vector2& v, const Vector2& n)
	{
		return v - 2.0f * Vector2::Dot(v, n) * n;
	}

	// Transform vector by matrix
	static Vector2 Transform(const Vector2& vec, const class Matrix3& mat, float w = 1.0f);

	static const Vector2 Zero;
	static const Vector2 UnitX;
	static const Vector2 UnitY;
	static const Vector2 NegUnitX;
	static const Vector2 NegUnitY;
};

// 3D Vector
class Vector3
{
public:
	float X;
	float Y;
	float Z;

	Vector3()
		:X(0.0f)
		, Y(0.0f)
		, Z(0.0f)
	{
	}

	explicit Vector3(float inX, float inY, float inZ)
		:X(inX)
		, Y(inY)
		, Z(inZ)
	{
	}

	// Cast to a const float pointer
	const float* GetAsFloatPtr() const
	{
		return reinterpret_cast<const float*>(&X);
	}

	// Set all three components in one line
	void Set(float inX, float inY, float inZ)
	{
		X = inX;
		Y = inY;
		Z = inZ;
	}

	// Vector addition (a + b)
	friend Vector3 operator+(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
	}

	// Vector subtraction (a - b)
	friend Vector3 operator-(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
	}

	// Component-wise multiplication
	friend Vector3 operator*(const Vector3& left, const Vector3& right)
	{
		return Vector3(left.X * right.X, left.Y * right.Y, left.Z * right.Z);
	}

	// Scalar multiplication
	friend Vector3 operator*(const Vector3& vec, float scalar)
	{
		return Vector3(vec.X * scalar, vec.Y * scalar, vec.Z * scalar);
	}

	// Scalar multiplication
	friend Vector3 operator*(float scalar, const Vector3& vec)
	{
		return Vector3(vec.X * scalar, vec.Y * scalar, vec.Z * scalar);
	}

	// Scalar *=
	Vector3& operator*=(float scalar)
	{
		X *= scalar;
		Y *= scalar;
		Z *= scalar;
		return *this;
	}

	// Vector +=
	Vector3& operator+=(const Vector3& right)
	{
		X += right.X;
		Y += right.Y;
		Z += right.Z;
		return *this;
	}

	// Vector -=
	Vector3& operator-=(const Vector3& right)
	{
		X -= right.X;
		Y -= right.Y;
		Z -= right.Z;
		return *this;
	}

	// Length squared of vector
	float LengthSq() const
	{
		return (X * X + Y * Y + Z * Z);
	}

	// Length of vector
	float Length() const
	{
		return (Math::Sqrt(LengthSq()));
	}

	// Normalize this vector
	void Normalize()
	{
		float length = Length();
		X /= length;
		Y /= length;
		Z /= length;
	}

	// Normalize the provided vector
	static Vector3 Normalize(const Vector3& vec)
	{
		Vector3 temp = vec;
		temp.Normalize();
		return temp;
	}

	// Dot product between two vectors (a dot b)
	static float Dot(const Vector3& a, const Vector3& b)
	{
		return (a.X * b.X + a.Y * b.Y + a.Z * b.Z);
	}

	// Cross product between two vectors (a cross b)
	static Vector3 Cross(const Vector3& a, const Vector3& b)
	{
		Vector3 temp;
		temp.X = a.Y * b.Z - a.Z * b.Y;
		temp.Y = a.Z * b.X - a.X * b.Z;
		temp.Z = a.X * b.Y - a.Y * b.X;
		return temp;
	}

	// Lerp from A to B by f
	static Vector3 Lerp(const Vector3& a, const Vector3& b, float f)
	{
		return Vector3(a + f * (b - a));
	}

	// Reflect V about (normalized) N
	static Vector3 Reflect(const Vector3& v, const Vector3& n)
	{
		return v - 2.0f * Vector3::Dot(v, n) * n;
	}

	static Vector3 Transform(const Vector3& vec, const class Matrix4& mat, float w = 1.0f);
	// This will transform the vector and renormalize the w component
	static Vector3 TransformWithPerspDiv(const Vector3& vec, const class Matrix4& mat, float w = 1.0f);

	// Transform a Vector3 by a quaternion
	static Vector3 Transform(const Vector3& v, const class Quaternion& q);

	static const Vector3 Zero;
	static const Vector3 UnitX;
	static const Vector3 UnitY;
	static const Vector3 UnitZ;
	static const Vector3 NegUnitX;
	static const Vector3 NegUnitY;
	static const Vector3 NegUnitZ;
	static const Vector3 Infinity;
	static const Vector3 NegInfinity;
};

// 3x3 Matrix
class Matrix3
{
public:
	float Mat[3][3];

	Matrix3()
	{
		*this = Matrix3::Identity;
	}

	explicit Matrix3(float inMat[3][3])
	{
		memcpy(Mat, inMat, 9 * sizeof(float));
	}

	// Cast to a const float pointer
	const float* GetAsFloatPtr() const
	{
		return reinterpret_cast<const float*>(&Mat[0][0]);
	}

	// Matrix multiplication
	friend Matrix3 operator*(const Matrix3& left, const Matrix3& right)
	{
		Matrix3 retVal;
		// row 0
		retVal.Mat[0][0] =
			left.Mat[0][0] * right.Mat[0][0] +
			left.Mat[0][1] * right.Mat[1][0] +
			left.Mat[0][2] * right.Mat[2][0];

		retVal.Mat[0][1] =
			left.Mat[0][0] * right.Mat[0][1] +
			left.Mat[0][1] * right.Mat[1][1] +
			left.Mat[0][2] * right.Mat[2][1];

		retVal.Mat[0][2] =
			left.Mat[0][0] * right.Mat[0][2] +
			left.Mat[0][1] * right.Mat[1][2] +
			left.Mat[0][2] * right.Mat[2][2];

		// row 1
		retVal.Mat[1][0] =
			left.Mat[1][0] * right.Mat[0][0] +
			left.Mat[1][1] * right.Mat[1][0] +
			left.Mat[1][2] * right.Mat[2][0];

		retVal.Mat[1][1] =
			left.Mat[1][0] * right.Mat[0][1] +
			left.Mat[1][1] * right.Mat[1][1] +
			left.Mat[1][2] * right.Mat[2][1];

		retVal.Mat[1][2] =
			left.Mat[1][0] * right.Mat[0][2] +
			left.Mat[1][1] * right.Mat[1][2] +
			left.Mat[1][2] * right.Mat[2][2];

		// row 2
		retVal.Mat[2][0] =
			left.Mat[2][0] * right.Mat[0][0] +
			left.Mat[2][1] * right.Mat[1][0] +
			left.Mat[2][2] * right.Mat[2][0];

		retVal.Mat[2][1] =
			left.Mat[2][0] * right.Mat[0][1] +
			left.Mat[2][1] * right.Mat[1][1] +
			left.Mat[2][2] * right.Mat[2][1];

		retVal.Mat[2][2] =
			left.Mat[2][0] * right.Mat[0][2] +
			left.Mat[2][1] * right.Mat[1][2] +
			left.Mat[2][2] * right.Mat[2][2];

		return retVal;
	}

	Matrix3& operator*=(const Matrix3& right)
	{
		*this = *this * right;
		return *this;
	}

	// Create a scale matrix with x and y scales
	static Matrix3 CreateScale(float xScale, float yScale)
	{
		float temp[3][3] =
		{
			{ xScale, 0.0f, 0.0f },
			{ 0.0f, yScale, 0.0f },
			{ 0.0f, 0.0f, 1.0f },
		};
		return Matrix3(temp);
	}

	static Matrix3 CreateScale(const Vector2& scaleVector)
	{
		return CreateScale(scaleVector.X, scaleVector.Y);
	}

	// Create a scale matrix with a uniform factor
	static Matrix3 CreateScale(float scale)
	{
		return CreateScale(scale, scale);
	}

	// Create a rotation matrix about the Z axis
	// theta is in radians
	static Matrix3 CreateRotation(float theta)
	{
		float temp[3][3] =
		{
			{ Math::Cos(theta), Math::Sin(theta), 0.0f },
			{ -Math::Sin(theta), Math::Cos(theta), 0.0f },
			{ 0.0f, 0.0f, 1.0f },
		};
		return Matrix3(temp);
	}

	// Create a translation matrix (on the xy-plane)
	static Matrix3 CreateTranslation(const Vector2& trans)
	{
		float temp[3][3] =
		{
			{ 1.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
			{ trans.X, trans.Y, 1.0f },
		};
		return Matrix3(temp);
	}

	static const Matrix3 Identity;
};

// 4x4 Matrix
class Matrix4
{
public:
	float Mat[4][4];

	Matrix4()
	{
		*this = Matrix4::Identity;
	}

	explicit Matrix4(float inMat[4][4])
	{
		memcpy(Mat, inMat, 16 * sizeof(float));
	}

	// Cast to a const float pointer
	const float* GetAsFloatPtr() const
	{
		return reinterpret_cast<const float*>(&Mat[0][0]);
	}

	// Matrix multiplication (a * b)
	friend Matrix4 operator*(const Matrix4& a, const Matrix4& b)
	{
		Matrix4 retVal;
		// row 0
		retVal.Mat[0][0] =
			a.Mat[0][0] * b.Mat[0][0] +
			a.Mat[0][1] * b.Mat[1][0] +
			a.Mat[0][2] * b.Mat[2][0] +
			a.Mat[0][3] * b.Mat[3][0];

		retVal.Mat[0][1] =
			a.Mat[0][0] * b.Mat[0][1] +
			a.Mat[0][1] * b.Mat[1][1] +
			a.Mat[0][2] * b.Mat[2][1] +
			a.Mat[0][3] * b.Mat[3][1];

		retVal.Mat[0][2] =
			a.Mat[0][0] * b.Mat[0][2] +
			a.Mat[0][1] * b.Mat[1][2] +
			a.Mat[0][2] * b.Mat[2][2] +
			a.Mat[0][3] * b.Mat[3][2];

		retVal.Mat[0][3] =
			a.Mat[0][0] * b.Mat[0][3] +
			a.Mat[0][1] * b.Mat[1][3] +
			a.Mat[0][2] * b.Mat[2][3] +
			a.Mat[0][3] * b.Mat[3][3];

		// row 1
		retVal.Mat[1][0] =
			a.Mat[1][0] * b.Mat[0][0] +
			a.Mat[1][1] * b.Mat[1][0] +
			a.Mat[1][2] * b.Mat[2][0] +
			a.Mat[1][3] * b.Mat[3][0];

		retVal.Mat[1][1] =
			a.Mat[1][0] * b.Mat[0][1] +
			a.Mat[1][1] * b.Mat[1][1] +
			a.Mat[1][2] * b.Mat[2][1] +
			a.Mat[1][3] * b.Mat[3][1];

		retVal.Mat[1][2] =
			a.Mat[1][0] * b.Mat[0][2] +
			a.Mat[1][1] * b.Mat[1][2] +
			a.Mat[1][2] * b.Mat[2][2] +
			a.Mat[1][3] * b.Mat[3][2];

		retVal.Mat[1][3] =
			a.Mat[1][0] * b.Mat[0][3] +
			a.Mat[1][1] * b.Mat[1][3] +
			a.Mat[1][2] * b.Mat[2][3] +
			a.Mat[1][3] * b.Mat[3][3];

		// row 2
		retVal.Mat[2][0] =
			a.Mat[2][0] * b.Mat[0][0] +
			a.Mat[2][1] * b.Mat[1][0] +
			a.Mat[2][2] * b.Mat[2][0] +
			a.Mat[2][3] * b.Mat[3][0];

		retVal.Mat[2][1] =
			a.Mat[2][0] * b.Mat[0][1] +
			a.Mat[2][1] * b.Mat[1][1] +
			a.Mat[2][2] * b.Mat[2][1] +
			a.Mat[2][3] * b.Mat[3][1];

		retVal.Mat[2][2] =
			a.Mat[2][0] * b.Mat[0][2] +
			a.Mat[2][1] * b.Mat[1][2] +
			a.Mat[2][2] * b.Mat[2][2] +
			a.Mat[2][3] * b.Mat[3][2];

		retVal.Mat[2][3] =
			a.Mat[2][0] * b.Mat[0][3] +
			a.Mat[2][1] * b.Mat[1][3] +
			a.Mat[2][2] * b.Mat[2][3] +
			a.Mat[2][3] * b.Mat[3][3];

		// row 3
		retVal.Mat[3][0] =
			a.Mat[3][0] * b.Mat[0][0] +
			a.Mat[3][1] * b.Mat[1][0] +
			a.Mat[3][2] * b.Mat[2][0] +
			a.Mat[3][3] * b.Mat[3][0];

		retVal.Mat[3][1] =
			a.Mat[3][0] * b.Mat[0][1] +
			a.Mat[3][1] * b.Mat[1][1] +
			a.Mat[3][2] * b.Mat[2][1] +
			a.Mat[3][3] * b.Mat[3][1];

		retVal.Mat[3][2] =
			a.Mat[3][0] * b.Mat[0][2] +
			a.Mat[3][1] * b.Mat[1][2] +
			a.Mat[3][2] * b.Mat[2][2] +
			a.Mat[3][3] * b.Mat[3][2];

		retVal.Mat[3][3] =
			a.Mat[3][0] * b.Mat[0][3] +
			a.Mat[3][1] * b.Mat[1][3] +
			a.Mat[3][2] * b.Mat[2][3] +
			a.Mat[3][3] * b.Mat[3][3];

		return retVal;
	}

	Matrix4& operator*=(const Matrix4& right)
	{
		*this = *this * right;
		return *this;
	}

	// Invert the matrix - super slow
	void Invert();

	// Get the translation component of the matrix
	Vector3 GetTranslation() const
	{
		return Vector3(Mat[3][0], Mat[3][1], Mat[3][2]);
	}

	// Get the X axis of the matrix (forward)
	Vector3 GetXAxis() const
	{
		return Vector3::Normalize(Vector3(Mat[0][0], Mat[0][1], Mat[0][2]));
	}

	// Get the Y axis of the matrix (left)
	Vector3 GetYAxis() const
	{
		return Vector3::Normalize(Vector3(Mat[1][0], Mat[1][1], Mat[1][2]));
	}

	// Get the Z axis of the matrix (up)
	Vector3 GetZAxis() const
	{
		return Vector3::Normalize(Vector3(Mat[2][0], Mat[2][1], Mat[2][2]));
	}

	// Extract the scale component from the matrix
	Vector3 GetScale() const
	{
		Vector3 retVal;
		retVal.X = Vector3(Mat[0][0], Mat[0][1], Mat[0][2]).Length();
		retVal.Y = Vector3(Mat[1][0], Mat[1][1], Mat[1][2]).Length();
		retVal.Z = Vector3(Mat[2][0], Mat[2][1], Mat[2][2]).Length();
		return retVal;
	}

	// Create a scale matrix with x, y, and z scales
	static Matrix4 CreateScale(float xScale, float yScale, float zScale)
	{
		float temp[4][4] =
		{
			{ xScale, 0.0f, 0.0f, 0.0f },
			{ 0.0f, yScale, 0.0f, 0.0f },
			{ 0.0f, 0.0f, zScale, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		};
		return Matrix4(temp);
	}

	static Matrix4 CreateScale(const Vector3& scaleVector)
	{
		return CreateScale(scaleVector.X, scaleVector.Y, scaleVector.Z);
	}

	// Create a scale matrix with a uniform factor
	static Matrix4 CreateScale(float scale)
	{
		return CreateScale(scale, scale, scale);
	}

	// Rotation about x-axis
	static Matrix4 CreateRotationX(float theta)
	{
		float temp[4][4] =
		{
			{ 1.0f, 0.0f, 0.0f , 0.0f },
			{ 0.0f, Math::Cos(theta), Math::Sin(theta), 0.0f },
			{ 0.0f, -Math::Sin(theta), Math::Cos(theta), 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
		};
		return Matrix4(temp);
	}

	// Rotation about y-axis
	static Matrix4 CreateRotationY(float theta)
	{
		float temp[4][4] =
		{
			{ Math::Cos(theta), 0.0f, -Math::Sin(theta), 0.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f },
			{ Math::Sin(theta), 0.0f, Math::Cos(theta), 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
		};
		return Matrix4(temp);
	}

	// Rotation about z-axis
	static Matrix4 CreateRotationZ(float theta)
	{
		float temp[4][4] =
		{
			{ Math::Cos(theta), Math::Sin(theta), 0.0f, 0.0f },
			{ -Math::Sin(theta), Math::Cos(theta), 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
		};
		return Matrix4(temp);
	}

	// Create a rotation matrix from a quaternion
	static Matrix4 CreateFromQuaternion(const class Quaternion& q);

	static Matrix4 CreateTranslation(const Vector3& trans)
	{
		float temp[4][4] =
		{
			{ 1.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ trans.X, trans.Y, trans.Z, 1.0f }
		};
		return Matrix4(temp);
	}

	static Matrix4 CreateLookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
	{
		Vector3 zaxis = Vector3::Normalize(target - eye);
		Vector3 xaxis = Vector3::Normalize(Vector3::Cross(up, zaxis));
		Vector3 yaxis = Vector3::Normalize(Vector3::Cross(zaxis, xaxis));
		Vector3 trans;
		trans.X = -Vector3::Dot(xaxis, eye);
		trans.Y = -Vector3::Dot(yaxis, eye);
		trans.Z = -Vector3::Dot(zaxis, eye);

		float temp[4][4] =
		{
			{ xaxis.X, yaxis.X, zaxis.X, 0.0f },
			{ xaxis.Y, yaxis.Y, zaxis.Y, 0.0f },
			{ xaxis.Z, yaxis.Z, zaxis.Z, 0.0f },
			{ trans.X, trans.Y, trans.Z, 1.0f }
		};
		return Matrix4(temp);
	}

	static Matrix4 CreateOrtho(float width, float height, float near, float far)
	{
		float temp[4][4] =
		{
			{ 2.0f / width, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 2.0f / height, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f / (far - near), 0.0f },
			{ 0.0f, 0.0f, near / (near - far), 1.0f }
		};
		return Matrix4(temp);
	}

	static Matrix4 CreatePerspectiveFOV(float fovY, float width, float height, float near, float far)
	{
		float yScale = Math::Cot(fovY / 2.0f);
		float xScale = yScale * height / width;
		float temp[4][4] =
		{
			{ xScale, 0.0f, 0.0f, 0.0f },
			{ 0.0f, yScale, 0.0f, 0.0f },
			{ 0.0f, 0.0f, far / (far - near), 1.0f },
			{ 0.0f, 0.0f, -near * far / (far - near), 0.0f }
		};
		return Matrix4(temp);
	}

	// Create "Simple" View-Projection Matrix from Chapter 6
	static Matrix4 CreateSimpleViewProj(float width, float height)
	{
		float temp[4][4] =
		{
			{ 2.0f / width, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 2.0f / height, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 1.0f }
		};
		return Matrix4(temp);
	}

	static const Matrix4 Identity;
};

// (Unit) Quaternion
class Quaternion
{
public:
	float X;
	float Y;
	float Z;
	float W;

	Quaternion()
	{
		*this = Quaternion::Identity;
	}

	// This directly sets the quaternion components --
	// don't use for axis/angle
	explicit Quaternion(float inX, float inY, float inZ, float inW)
	{
		Set(inX, inY, inZ, inW);
	}

	// Construct the quaternion from an axis and angle
	// It is assumed that axis is already normalized,
	// and the angle is in radians
	explicit Quaternion(const Vector3& axis, float angle)
	{
		float scalar = Math::Sin(angle / 2.0f);
		X = axis.X * scalar;
		Y = axis.Y * scalar;
		Z = axis.Z * scalar;
		W = Math::Cos(angle / 2.0f);
	}

	// Directly set the internal components
	void Set(float inX, float inY, float inZ, float inW)
	{
		X = inX;
		Y = inY;
		Z = inZ;
		W = inW;
	}

	void Conjugate()
	{
		X *= -1.0f;
		Y *= -1.0f;
		Z *= -1.0f;
	}

	float LengthSq() const
	{
		return (X * X + Y * Y + Z * Z + W * W);
	}

	float Length() const
	{
		return Math::Sqrt(LengthSq());
	}

	void Normalize()
	{
		float length = Length();
		X /= length;
		Y /= length;
		Z /= length;
		W /= length;
	}

	// Normalize the provided quaternion
	static Quaternion Normalize(const Quaternion& q)
	{
		Quaternion retVal = q;
		retVal.Normalize();
		return retVal;
	}

	// Linear interpolation
	static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float f)
	{
		Quaternion retVal;
		retVal.X = Math::Lerp(a.X, b.X, f);
		retVal.Y = Math::Lerp(a.Y, b.Y, f);
		retVal.Z = Math::Lerp(a.Z, b.Z, f);
		retVal.W = Math::Lerp(a.W, b.W, f);
		retVal.Normalize();
		return retVal;
	}

	static float Dot(const Quaternion& a, const Quaternion& b)
	{
		return a.X * b.X + a.Y * b.Y + a.Z * b.Z + a.W * b.W;
	}

	// Spherical Linear Interpolation
	static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float f)
	{
		float rawCosm = Quaternion::Dot(a, b);

		float cosom = -rawCosm;
		if (rawCosm >= 0.0f)
		{
			cosom = rawCosm;
		}

		float scale0, scale1;

		if (cosom < 0.9999f)
		{
			const float omega = Math::Acos(cosom);
			const float invSin = 1.f / Math::Sin(omega);
			scale0 = Math::Sin((1.f - f) * omega) * invSin;
			scale1 = Math::Sin(f * omega) * invSin;
		}
		else
		{
			// Use linear interpolation if the quaternions
			// are collinear
			scale0 = 1.0f - f;
			scale1 = f;
		}

		if (rawCosm < 0.0f)
		{
			scale1 = -scale1;
		}

		Quaternion retVal;
		retVal.X = scale0 * a.X + scale1 * b.X;
		retVal.Y = scale0 * a.Y + scale1 * b.Y;
		retVal.Z = scale0 * a.Z + scale1 * b.Z;
		retVal.W = scale0 * a.W + scale1 * b.W;
		retVal.Normalize();
		return retVal;
	}

	// Concatenate
	// Rotate by q FOLLOWED BY p
	static Quaternion Concatenate(const Quaternion& q, const Quaternion& p)
	{
		Quaternion retVal;

		// Vector component is:
		// ps * qv + qs * pv + pv x qv
		Vector3 qv(q.X, q.Y, q.Z);
		Vector3 pv(p.X, p.Y, p.Z);
		Vector3 newVec = p.W * qv + q.W * pv + Vector3::Cross(pv, qv);
		retVal.X = newVec.X;
		retVal.Y = newVec.Y;
		retVal.Z = newVec.Z;

		// Scalar component is:
		// ps * qs - pv . qv
		retVal.W = p.W * q.W - Vector3::Dot(pv, qv);

		return retVal;
	}

	static const Quaternion Identity;
};

namespace Color
{
	static const Vector3 Black(0.0f, 0.0f, 0.0f);
	static const Vector3 White(1.0f, 1.0f, 1.0f);
	static const Vector3 Red(1.0f, 0.0f, 0.0f);
	static const Vector3 Green(0.0f, 1.0f, 0.0f);
	static const Vector3 Blue(0.0f, 0.0f, 1.0f);
	static const Vector3 Yellow(1.0f, 1.0f, 0.0f);
	static const Vector3 LightYellow(1.0f, 1.0f, 0.88f);
	static const Vector3 LightBlue(0.68f, 0.85f, 0.9f);
	static const Vector3 LightPink(1.0f, 0.71f, 0.76f);
	static const Vector3 LightGreen(0.56f, 0.93f, 0.56f);
}
