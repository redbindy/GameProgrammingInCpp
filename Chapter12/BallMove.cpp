#include "BallMove.h"

#include "Actor.h"
#include "PhysWorld.h"
#include "Game.h"
#include "TargetActor.h"
#include "BallActor.h"

BallMove::BallMove(Actor* const pOwner)
	: MoveComponent(pOwner)
	, mpPlayer(nullptr)
{
}

void BallMove::Update(const float deltaTime)
{
	constexpr float segmentLength = 30.f;

	const Vector3 start = mpOwner->GetPosition();
	Vector3 dir = mpOwner->GetForward();
	const Vector3 end = start + dir * segmentLength;

	LineSegment l(start, end);

	Game* const pGame = mpOwner->GetGame();
	PhysWorld* const phys = pGame->GetPhysWorld();

	PhysWorld::CollisionInfo info;
	if (phys->SegmentCast(l, info) && info.pActor != mpPlayer)
	{
		dir = Vector3::Reflect(dir, info.normal);
		mpOwner->RotateToNewForward(dir);

		TargetActor* const pTarget = dynamic_cast<TargetActor*>(info.pActor);
		if (pTarget != nullptr)
		{
			static_cast<BallActor*>(mpOwner)->HitTarget();
		}
	}

	MoveComponent::Update(deltaTime);
}
