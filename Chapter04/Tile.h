#pragma once

#include <vector>

#include "Actor.h"

class Game;
class SpriteComponent;

class Tile final : public Actor
{
	friend class Grid;

public:
	enum class ETileState
	{
		DEFAULT,
		PATH,
		START,
		BASE
	};

public:
	Tile(Game* const pGame);
	virtual ~Tile() = default;

	void ToggleSelect();

	inline ETileState GetTileState() const;
	void SetTileState(const ETileState state);

	inline const Tile* GetParent() const;

private:
	std::vector<Tile*> mAdjacent;

	ETileState mTileState;
	Tile* mpParent;

	float f;
	float g;
	float h;

	bool mbOpenSet;
	bool mbClosedSet;
	bool mbBlocked;

	bool mbSelected;

	SpriteComponent* mpSprite;

private:
	void updateTexture();
};

inline Tile::ETileState Tile::GetTileState() const
{
	return mTileState;
}

inline const Tile* Tile::GetParent() const
{
	return mpParent;
}
