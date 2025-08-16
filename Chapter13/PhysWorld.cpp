#include "PhysWorld.h"

#include "BoxComponent.h"

PhysWorld::PhysWorld(Game* const pGame)
	: mpGame(pGame)
	, mBoxes()
{
}

bool PhysWorld::SegmentCast(const LineSegment& l, CollisionInfo& outColl)
{
	bool collided = false;

	float closestT = Math::Infinity;
	Vector3 norm;

	for (BoxComponent* const p : mBoxes)
	{
		float t;
		if (Intersect(l, p->GetWorldBox(), t, norm))
		{
			if (t < closestT)
			{
				closestT = t;
				outColl.point = l.PointOnSegment(t);
				outColl.normal = norm;
				outColl.pBox = p;
				outColl.pActor = p->GetOwner();

				collided = true;
			}
		}
	}

	return collided;
}

void PhysWorld::TestPairwise(std::function<void(Actor* const, Actor* const)> f)
{
	for (size_t i = 0; i < mBoxes.size(); ++i)
	{
		for (size_t j = i + 1; j < mBoxes.size(); ++j)
		{
			BoxComponent* const pA = mBoxes[i];
			BoxComponent* const pB = mBoxes[j];
			if (Intersect(pA->GetWorldBox(), pB->GetWorldBox()))
			{
				f(pA->GetOwner(), pB->GetOwner());
			}
		}
	}
}

void PhysWorld::TestSweepAndPrune(std::function<void(Actor* const, Actor* const)> f)
{
	std::sort(mBoxes.begin(), mBoxes.end(),
		[](BoxComponent* const pA, BoxComponent* const pB)
		{
			return pA->GetWorldBox().Min.X < pB->GetWorldBox().Min.X;
		});

	for (size_t i = 0; i < mBoxes.size(); ++i)
	{
		BoxComponent* const pA = mBoxes[i];

		const float max = pA->GetWorldBox().Max.X;
		for (size_t j = i + 1; j < mBoxes.size(); ++j)
		{
			BoxComponent* const pB = mBoxes[j];
			if (pB->GetWorldBox().Min.X > max)
			{
				break;
			}

			if (Intersect(pA->GetWorldBox(), pB->GetWorldBox()))
			{
				f(pA->GetOwner(), pB->GetOwner());
			}
		}
	}
}

void PhysWorld::AddBox(BoxComponent* const pBox)
{
	mBoxes.push_back(pBox);
}

void PhysWorld::RemoveBox(BoxComponent* const pBox)
{
	std::vector<BoxComponent*>::iterator iter = std::find(mBoxes.begin(), mBoxes.end(), pBox);

	if (iter != mBoxes.end())
	{
		std::iter_swap(iter, mBoxes.end() - 1);
		mBoxes.pop_back();
	}
}
