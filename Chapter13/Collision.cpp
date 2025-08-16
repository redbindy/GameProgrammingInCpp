#include "Collision.h"

LineSegment::LineSegment(const Vector3& start, const Vector3& end)
	: Start(start)
	, End(end)
{

}

Vector3 LineSegment::PointOnSegment(const float t) const
{
	return Start + (End - Start) * t;
}

float LineSegment::MinDistSq(const Vector3& pos) const
{
	const Vector3 ab = End - Start;
	const Vector3 ac = pos - Start;
	if (Vector3::Dot(ab, ac) < 0.f)
	{
		return ac.LengthSq();
	}

	const Vector3 ba = ab * -1.f;
	const Vector3 bc = pos - End;
	if (Vector3::Dot(ba, bc) < 0.f)
	{
		return bc.LengthSq();
	}

	const Vector3 p = Vector3::Dot(ac, ab) / Vector3::Dot(ab, ab) * ab;

	const Vector3 d = ac - p;

	return d.LengthSq();
}

float LineSegment::MinDistSq(const LineSegment& s1, const LineSegment& s2)
{
	const Vector3 u = s1.End - s1.Start;
	const Vector3 v = s2.End - s2.Start;

	const Vector3 w = s1.Start - s2.Start;

	const float a = Vector3::Dot(u, u);
	const float b = Vector3::Dot(u, v);
	const float c = Vector3::Dot(v, v);
	const float d = Vector3::Dot(u, w);
	const float e = Vector3::Dot(v, w);

	const float D = a * c - b * b; // 항상 >= 0

	float sc = D; // sc = sN / sD, 기본 sD = D >= 0
	float sN = D;
	float sD = D;

	float tc = D; // tc = tN / tD 기본 tD = D >= 0
	float tN = D;
	float tD = D;

	// 두 개의 최근접 점의 직선 매개변수를 계산
	if (Math::IsNearZero(D)) // 직선이 거의 평행이면
	{
		sN = 0.f; // s1에 있는 P0를 사용하게 강제
		sD = 1.f; // 0 나누기 방지용
		tN = e;
		tD = c;
	}
	else // 무한 직선에서 최근접점 얻기
	{
		sN = (b * e - c * d);
		tN = (a * e - b * d);
		if (sN < 0.f) // sc < 0 => s = 0 엣지가 보임
		{
			sN = 0.f;
			tN = e;
			tD = c;
		}
		else if (sN > sD) // sc > 1 => s = 1 엣지가 보임
		{
			sN = sD;
			tN = e + b;
			tD = c;
		}
	}

	if (tN < 0.f) // tc < 0 => t = 0 엣지가 보임
	{
		tN = 0.f;

		// 이 엣지에 대해서 sc를 다시 계산
		if (-d < 0.f)
		{
			sN = 0.f;
		}
		else if (-d > a)
		{
			sN = sD;
		}
		else
		{
			sN = -d;
			sD = a;
		}
	}
	else if (tN > tD) // tc > 1 => t = 1 엣지가 보임
	{
		tN = tD;

		// 이 엣지에 대해서 sc를 재계산
		if ((-d + b) < 0.f)
		{
			sN = 0.f;
		}
		else
		{
			sN = (-d + b);
			sD = a;
		}
	}

	// sc와 tc를 얻기 위해 나누기
	sc = (Math::IsNearZero(sN) ? 0.f : sN / sD);
	tc = (Math::IsNearZero(tN) ? 0.f : tN / tD);

	// 두 최근접 점과 차 얻기
	const Vector3 dp = w + (sc * u) - (tc * v);

	return dp.LengthSq(); // 최단 거리 반환
}

Plane::Plane(const Vector3& normal, const float d)
	: Normal(normal)
	, D(d)
{
}

Plane::Plane(const Vector3& a, const Vector3 b, const Vector3& c)
	: Normal()
	, D(0.f)
{
	const Vector3 ab = b - a;
	const Vector3 ac = c - a;

	Normal = Vector3::Cross(ab, ac);
	Normal.Normalize();

	D = -Vector3::Dot(a, Normal);
}

float Plane::GetSignedDist(const Vector3& point) const
{
	return Vector3::Dot(point, Normal) - D;
}

Sphere::Sphere(const Vector3& center, const float radius)
	: Center(center)
	, Radius(radius)
{
}

bool Sphere::Contains(const Vector3& point) const
{
	const Vector3 pointToCenter = Center - point;

	const float distSq = pointToCenter.LengthSq();

	return distSq <= Radius;
}

AABB::AABB(const Vector3& min, const Vector3& max)
	: Min(min)
	, Max(max)
{

}

void AABB::UpdateMinMax(const Vector3& point)
{
	Min.X = Math::Min(Min.X, point.X);
	Min.Y = Math::Min(Min.Y, point.Y);
	Min.Z = Math::Min(Min.Z, point.Z);

	Max.X = Math::Max(Max.X, point.X);
	Max.Y = Math::Max(Max.Y, point.Y);
	Max.Z = Math::Max(Max.Z, point.Z);
}

void AABB::Rotate(const Quaternion& q)
{
	std::array<Vector3, 8> points;

	points[0] = Min;

	// 2개의 min, 1 max 순열로 구석점 생성
	points[1] = Vector3(Max.X, Min.Y, Min.Z);
	points[2] = Vector3(Min.X, Max.Y, Min.Z);
	points[3] = Vector3(Min.X, Min.Y, Max.Z);

	// 2개의 max, 1 min 순열로 구석점 생성
	points[4] = Vector3(Min.X, Max.Y, Max.Z);
	points[5] = Vector3(Max.X, Min.Y, Max.Z);
	points[6] = Vector3(Max.X, Max.Y, Min.Z);

	points[7] = Max;

	Vector3 p = Vector3::Transform(points[0], q);

	Min = p;
	Max = p;
	for (size_t i = 1; i < points.size(); ++i)
	{
		p = Vector3::Transform(points[i], q);

		UpdateMinMax(p);
	}
}

bool AABB::Contains(const Vector3& point) const
{
	const bool bOutside = point.X < Min.X
		|| point.Y < Min.Y
		|| point.Z < Min.Z
		|| point.X > Max.X
		|| point.Y > Max.Y
		|| point.Z > Max.Z;

	return !bOutside;
}

float AABB::MinDistSq(const Vector3& point) const
{
	float dx = Math::Max(Min.X - point.X, 0.f);
	dx = Math::Max(dx, point.X - Max.X);

	float dy = Math::Max(Min.Y - point.Y, 0.f);
	dy = Math::Max(dy, point.Y - Max.Y);

	float dz = Math::Max(Min.Z - point.Z, 0.f);
	dz = Math::Max(dz, point.Z - Max.Z);

	return dx * dx + dy * dy + dz * dz;
}

Capsule::Capsule(const Vector3& start, const Vector3& end, const float radius)
	: Segment(start, end)
	, Radius(radius)
{

}

bool Capsule::Contains(const Vector3& point) const
{
	const float distSq = Segment.MinDistSq(point);

	return distSq <= Radius * Radius;
}

bool ConvexPolygon::Contains(const Vector2& point) const
{
	float sum = 0.f;

	Vector2 a;
	Vector2 b;
	for (int i = 0; i < Vertices.size(); ++i)
	{
		a = Vertices[i] - point;
		a.Normalize();

		b = Vertices[i + 1] - point;
		b.Normalize();

		const float cos = Vector2::Dot(a, b);

		sum += Math::Acos(cos);
	}

	a = Vertices.back() - point;
	a.Normalize();

	b = Vertices.front() - point;
	b.Normalize();

	const float cos = Vector2::Dot(a, b);

	sum += Math::Acos(cos);

	return Math::IsNearZero(sum - Math::TWO_PI);
}

bool Intersect(const Sphere& a, const Sphere& b)
{
	const Vector3 aToB = b.Center - a.Center;

	const float distSq = aToB.LengthSq();

	float sumRadSq = a.Radius + b.Radius;
	sumRadSq *= sumRadSq;

	return distSq <= sumRadSq;
}

bool Intersect(const AABB& a, const AABB& b)
{
	const bool bNo = a.Max.X < b.Min.X
		|| a.Max.Y < b.Min.Y
		|| a.Max.Z < b.Min.Z
		|| b.Max.X < a.Min.X
		|| b.Max.Y < a.Min.Y
		|| b.Max.Z < a.Min.Z;

	return !bNo;
}

bool Intersect(const Sphere& s, const AABB& box)
{
	const float distSq = box.MinDistSq(s.Center);

	return distSq <= s.Radius * s.Radius;
}

bool Intersect(const Capsule& a, const Capsule& b)
{
	const float distSq = LineSegment::MinDistSq(a.Segment, b.Segment);

	float sumRadSq = a.Radius + b.Radius;
	sumRadSq *= sumRadSq;

	return distSq <= sumRadSq;
}

bool Intersect(const LineSegment& l, const Plane& p, float& outT)
{
	const float denom = Vector3::Dot(l.End - l.Start, p.Normal);
	if (Math::IsNearZero(denom))
	{
		if (Math::IsNearZero(Vector3::Dot(l.Start, p.Normal) - p.D))
		{
			outT = 0.f;

			return true;
		}
	}
	else
	{
		const float numer = -Vector3::Dot(l.Start, p.Normal) - p.D;
		outT = numer / denom;

		return outT >= 0.f && outT <= 1.f;
	}

	return false;
}

bool Intersect(const LineSegment& l, const Sphere& s, float& outT)
{
	const Vector3 X = l.Start - s.Center;
	const Vector3 Y = l.End - l.Start;

	const float a = Vector3::Dot(Y, Y);
	const float b = 2.f * Vector3::Dot(X, Y);
	const float c = Vector3::Dot(X, X) - s.Radius * s.Radius;

	float disc = b * b - 4.f * a * c;
	if (disc < 0.f)
	{
		return false;
	}

	disc = Math::Sqrt(disc);

	const float tMin = (-b - disc) / (2.f * a);
	const float tMax = (-b + disc) / (2.f * a);

	if (tMin >= 0.f && tMin <= 1.f)
	{
		outT = tMin;

		return true;
	}

	if (tMax >= 0.f && tMax <= 1.f)
	{
		outT = tMax;

		return true;
	}

	return false;
}

bool TestSidePlane(
	const float start,
	const float end,
	const float negD,
	const Vector3& norm,
	std::vector<std::pair<float, Vector3>>& out
)
{
	const float denom = end - start;

	if (Math::IsNearZero(denom))
	{
		return false;
	}

	const float numer = -start + negD;
	const float t = numer / denom;

	if (t >= 0.f && t <= 1.f)
	{
		out.push_back({ t, norm });

		return true;
	}

	return false;
}

bool Intersect(const LineSegment& l, const AABB& b, float& outT, Vector3& outNorm)
{
	std::vector<std::pair<float, Vector3>> tValues;

	TestSidePlane(l.Start.X, l.End.X, b.Min.X, Vector3::NegUnitX, tValues);
	TestSidePlane(l.Start.X, l.End.X, b.Max.X, Vector3::UnitX, tValues);

	TestSidePlane(l.Start.Y, l.End.Y, b.Min.Y, Vector3::NegUnitY, tValues);
	TestSidePlane(l.Start.Y, l.End.Y, b.Max.Y, Vector3::UnitY, tValues);

	TestSidePlane(l.Start.Z, l.End.Z, b.Min.Z, Vector3::NegUnitZ, tValues);
	TestSidePlane(l.Start.Z, l.End.Z, b.Max.Z, Vector3::UnitZ, tValues);

	std::sort(tValues.begin(), tValues.end(),
		[](const std::pair<float, Vector3>& a, const std::pair<float, Vector3>& b)
		{
			return a.first < b.first;
		});

	Vector3 point;
	for (std::pair<float, Vector3>& t : tValues)
	{
		point = l.PointOnSegment(t.first);
		if (b.Contains(point))
		{
			outT = t.first;
			outNorm = t.second;

			return true;
		}
	}

	return false;
}

bool SweptSphere(const Sphere& p0, const Sphere& p1, const Sphere& q0, const Sphere& q1, float& outT)
{
	const Vector3 X = p0.Center - q0.Center;
	const Vector3 Y = p1.Center - p0.Center - (q1.Center - q0.Center);

	const float a = Vector3::Dot(Y, Y);
	const float b = 2.f * Vector3::Dot(X, Y);
	const float sumRadii = p0.Radius + q0.Radius;
	const float c = Vector3::Dot(X, X) - sumRadii * sumRadii;

	float disc = b * b - 4.f * a * c;

	if (disc < 0.f)
	{
		return false;
	}

	disc = Math::Sqrt(disc);

	outT = (-b - disc) / (2.f * a);

	return outT >= 0.f && outT <= 1.f;
}
