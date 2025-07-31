#pragma once

namespace Math
{
	constexpr float PI = 3.14159f;

	inline float ToRadian(const float degree)
	{
		return degree * PI / 180.f;
	}

	inline float ToDegree(const float radian)
	{
		return radian * 180.f / PI;
	}
};

class Vector2 final
{
public:
	float X;
	float Y;

public:
	Vector2();
	Vector2(const float x, const float y);
	virtual ~Vector2() = default;
};

inline Vector2::Vector2()
	: Vector2(0.f, 0.f)
{

}

inline Vector2::Vector2(const float x, const float y)
	: X(x)
	, Y(y)
{

}
