#pragma once

#include "Actor.h"

#include <vector>
#include <algorithm>

class Game;
class Tile;

enum
{
	NUM_ROWS = 7,
	NUM_COLS = 16
};

constexpr float START_Y = 192.f;
constexpr float TILE_SIZE = 64.f;
constexpr float ENEMY_TIME = 1.5f;

class Grid final : public Actor
{
public:
	Grid(Game* const pGame);
	virtual ~Grid() = default;

	void ProcessClick(const int x, const int y);

	bool FindPath(Tile* const pStart, Tile* const pGoal);

	void BuildTower();

	Tile* GetStartTile();
	Tile* GetEndTile();

	void UpdateActor(const float deltaTime) override;

private:
	Tile* mSelectedTile;

	std::vector<std::vector<Tile*>> mTiles;

	float mNextEnemy;

private:
	void selectTile(const size_t row, const size_t col);

	void updatePathTiles(Tile* const pStart);
};