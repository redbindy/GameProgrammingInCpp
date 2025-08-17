#pragma once

#include <vector>
#include <array>
#include <algorithm>

#include "Math.h"

class LineSegment final
{
public:
	Vector3 Start;
	Vector3 End;

public:
	LineSegment(const Vector3& start, const Vector3& end);
	Vector3 PointOnSegment(const float t) const;
	float MinDistSq(const Vector3& pos) const;

	static float MinDistSq(const LineSegment& s1, const LineSegment& s2);
};

class Plane final
{
public:
	Vector3 Normal;
	float D;

public:
	Plane(const Vector3& normal, const float d);
	Plane(const Vector3& a, const Vector3 b, const Vector3& c);
	float GetSignedDist(const Vector3& point) const;
};

class Sphere final
{
public:
	Vector3 Center;
	float Radius;

public:
	Sphere(const Vector3& center, const float radius);

	bool Contains(const Vector3& point) const;
};

class AABB final
{
public:
	Vector3 Min;
	Vector3 Max;

public:
	AABB(const Vector3& min, const Vector3& max);

	void UpdateMinMax(const Vector3& point);

	void Rotate(const Quaternion& q);

	bool Contains(const Vector3& point) const;

	float MinDistSq(const Vector3& point) const;
};

class OBB final
{
public:
	Vector3 Center;
	Quaternion Rotation;
	Vector3 Extents;
};

class Capsule final
{
public:
	LineSegment Segment;
	float Radius;

public:
	Capsule(const Vector3& start, const Vector3& end, const float radius);

	bool Contains(const Vector3& point) const;
};

class ConvexPolygon final
{
public:
	std::vector<Vector2> Vertices;

public:
	bool Contains(const Vector2& point) const;
};

bool Intersect(const Sphere& a, const Sphere& b);
bool Intersect(const AABB& a, const AABB& b);
bool Intersect(const Sphere& s, const AABB& box);
bool Intersect(const Capsule& a, const Capsule& b);

bool Intersect(const LineSegment& l, const Plane& p, float& outT);
bool Intersect(const LineSegment& l, const Sphere& s, float& outT);
bool Intersect(const LineSegment& l, const AABB& b, float& outT, Vector3& outNorm);
bool SweptSphere(
	const Sphere& p0, const Sphere& p1,
	const Sphere& q0, const Sphere& q1,
	float& outT
);