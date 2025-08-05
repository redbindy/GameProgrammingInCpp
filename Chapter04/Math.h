#pragma once

#include <cmath>
#include <limits>

namespace Math
{
	constexpr float PI = 3.14159f;
	constexpr float TWO_PI = 2.f * PI;
	constexpr float PI_OVER_2 = PI / 2.f;
	constexpr float Infinity = std::numeric_limits<float>::infinity();
	constexpr float NegInfinity = -Infinity;

	inline float ToRadians(const float degrees)
	{
		return degrees * PI / 180.f;
	}

	inline float ToDegrees(const float radians)
	{
		return radians * 180.f / PI;
	}

	inline bool IsNearZero(const float val, const float epsilon)
	{
		return fabs(val) <= epsilon;
	}

	inline bool IsNearZero(const float val)
	{
		return IsNearZero(val, 0.001f);
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

	inline float Abs(const float value)
	{
		return fabs(value);
	}

	inline float Cos(const float angle)
	{
		return cosf(angle);
	}

	inline float Sin(const float angle)
	{
		return sinf(angle);
	}

	inline float Tan(const float angle)
	{
		return tanf(angle);
	}

	inline float Acos(const float value)
	{
		return acosf(value);
	}

	inline float Atan2(const float y, const float x)
	{
		return atan2f(y, x);
	}

	inline float Cot(const float angle)
	{
		return 1.f / Tan(angle);
	}

	inline float Lerp(const float a, const float b, const float f)
	{
		return a + f * (b - a);
	}

	inline float Sqrt(const float value)
	{
		return sqrtf(value);
	}

	inline float Fmod(const float numer, const float denom)
	{
		return fmod(numer, denom);
	}
}

class Vector2 final
{
public:
	float X;
	float Y;

public:
	Vector2()
		: X(0.f)
		, Y(0.f)
	{

	}

	Vector2(const float x, const float y)
		: X(x)
		, Y(y)
	{

	}

	Vector2& operator+=(const Vector2& other)
	{
		X += other.X;
		Y += other.Y;

		return *this;
	}

	Vector2& operator-=(const Vector2& other)
	{
		X -= other.X;
		Y -= other.Y;

		return *this;
	}

	Vector2& operator*=(const float scaler)
	{
		X *= scaler;
		Y *= scaler;

		return *this;
	}

	friend Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
	{
		return Vector2(lhs.X + rhs.X, lhs.Y + rhs.Y);
	}

	friend Vector2 operator-(const Vector2& lhs, const Vector2& rhs)
	{
		return Vector2(lhs.X - rhs.X, lhs.Y - rhs.Y);
	}

	friend Vector2 operator*(const Vector2& lhs, const Vector2& rhs)
	{
		return Vector2(lhs.X * rhs.X, lhs.Y * rhs.Y);
	}

	friend Vector2 operator*(const Vector2& v, const float scalar)
	{
		return Vector2(v.X * scalar, v.Y * scalar);
	}

	friend Vector2 operator*(const float scaler, const Vector2& v)
	{
		return operator*(v, scaler);
	}

	float LengthSq() const
	{
		return (X * X + Y * Y);
	}

	float Length() const
	{
		return Math::Sqrt(LengthSq());
	}

	void Normalize()
	{
		const float length = Length();

		X /= length;
		Y /= length;
	}

	static Vector2 Normalize(const Vector2& v)
	{
		Vector2 temp = v;
		temp.Normalize();

		return temp;
	}

	static float Dot(const Vector2& lhs, const Vector2& rhs)
	{
		return (lhs.X * rhs.X + lhs.Y * rhs.Y);
	}

	static Vector2 Lerp(const Vector2& lhs, const Vector2& rhs, const float f)
	{
		return Vector2(lhs + f * (rhs - lhs));
	}

	static Vector2 Reflect(const Vector2& v, const Vector2& n)
	{
		return v - 2.f * Vector2::Dot(v, n) * n;
	}

	static Vector2 Transform(const Vector2& vec, const class Matrix& mat, const float w);

	static const Vector2 Zero;
	static const Vector2 UnitX;
	static const Vector2 UnitY;
	static const Vector2 NegUnitX;
	static const Vector2 NegUnitY;
};