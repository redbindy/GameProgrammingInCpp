#pragma once

#include <random>
#include "Math.h"

class Random final
{
public:
	static void Init();

	static void Seed(const unsigned int seed);

	static float GetFloat();

	static float GetFloatRange(const float min, const float max);

	static int GetIntRange(const int min, const int max);

	static Vector2 GetVector(const Vector2& min, const Vector2& max);

private:
	static std::mt19937 sGenerator;
};