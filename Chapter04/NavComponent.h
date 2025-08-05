#pragma once

#include "MoveComponent.h"
#include "Math.h"

class Tile;

class NavComponent final : public MoveComponent
{
public:
	NavComponent(Actor* const pOwner, const int updateOrder);
	NavComponent(Actor* const pOwner);
	virtual ~NavComponent() = default;

	void Update(const float deltaTime) override;
	void StartPath(const Tile* const pStart);
	void TurnTo(const Vector2& pos);

private:
	const Tile* mNextNode;
};