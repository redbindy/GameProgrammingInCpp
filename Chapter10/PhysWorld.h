#pragma once

#include <vector>
#include <functional>

#include "Math.h"
#include "Collision.h"

class Game;
class BoxComponent;
class Actor;

class PhysWorld final
{
public:
	struct CollisionInfo
	{
		Vector3 point;
		Vector3 normal;

		BoxComponent* pBox;

		Actor* pActor;
	};

public:
	PhysWorld(Game* const pGame);

	bool SegmentCast(const LineSegment& l, CollisionInfo& outColl);

	void TestPairwise(std::function<void(Actor* const, Actor* const)> f);

	void TestSweepAndPrune(std::function<void(Actor* const, Actor* const)> f);

	void AddBox(BoxComponent* const pBox);
	void RemoveBox(BoxComponent* const pBox);

private:
	Game* mpGame;
	std::vector<BoxComponent*> mBoxes;
};