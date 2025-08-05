#include "Grid.h"

#include "Tile.h"
#include "Tower.h"
#include "Enemy.h"

Grid::Grid(Game* const pGame)
	: Actor(pGame)
	, mSelectedTile(nullptr)
	, mTiles()
	, mNextEnemy(0.f)
{
	mTiles.resize(NUM_ROWS);
	for (size_t i = 0; i < mTiles.size(); ++i)
	{
		mTiles[i].resize(NUM_COLS);
	}

	for (size_t i = 0; i < NUM_ROWS; ++i)
	{
		const float tileY = START_Y + i * TILE_SIZE;
		for (size_t j = 0; j < NUM_COLS; ++j)
		{
			mTiles[i][j] = new Tile(pGame);

			const float tileX = TILE_SIZE * 0.5f + j * TILE_SIZE;
			mTiles[i][j]->SetPosition(Vector2(tileX, tileY));
		}
	}

	Tile* const pStartTile = GetStartTile();
	pStartTile->SetTileState(Tile::ETileState::START);

	Tile* const pEndTile = GetEndTile();
	pEndTile->SetTileState(Tile::ETileState::BASE);

	for (size_t i = 0; i < NUM_ROWS; ++i)
	{
		for (size_t j = 0; j < NUM_COLS; ++j)
		{
			if (i > 0)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i - 1][j]);
			}

			if (i < NUM_ROWS - 1)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i + 1][j]);
			}

			if (j > 0)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i][j - 1]);
			}

			if (j < NUM_COLS - 1)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i][j + 1]);
			}
		}
	}

	FindPath(pEndTile, pStartTile);
	updatePathTiles(pStartTile);

	mNextEnemy = ENEMY_TIME;
}

void Grid::ProcessClick(const int x, const int y)
{
	const float adjustedY = y - (START_Y - TILE_SIZE * 0.5f);

	if (adjustedY >= 0)
	{
		const float tileX = x / TILE_SIZE;
		const float tileY = adjustedY / TILE_SIZE;

		if (tileX >= 0 && tileX < NUM_COLS && tileY >= 0 && tileY < NUM_ROWS)
		{
			selectTile(tileY, tileX);
		}
	}

}

bool Grid::FindPath(Tile* const pStart, Tile* const pGoal)
{
	for (size_t i = 0; i < NUM_ROWS; ++i)
	{
		for (size_t j = 0; j < NUM_COLS; ++j)
		{
			mTiles[i][j]->g = 0.f;
			mTiles[i][j]->mbOpenSet = false;
			mTiles[i][j]->mbClosedSet = false;
		}
	}

	std::vector<Tile*> openSet;
	openSet.reserve(NUM_ROWS * NUM_COLS);

	Tile* pCurrent = pStart;
	pCurrent->mbClosedSet = true;

	do
	{
		for (Tile* p : pCurrent->mAdjacent)
		{
			if (p->mbBlocked)
			{
				continue;
			}

			if (!p->mbClosedSet)
			{
				if (!p->mbOpenSet)
				{
					p->mpParent = pCurrent;

					const Vector2 goalToP = p->GetPosition() - pGoal->GetPosition();
					const float dist = goalToP.Length();

					p->h = dist;
					p->g = pCurrent->g + TILE_SIZE;
					p->f = p->g + p->h;

					openSet.push_back(p);
					p->mbOpenSet = true;
				}
			}
			else
			{
				const float newG = pCurrent->g + TILE_SIZE;
				if (newG < p->g)
				{
					p->mpParent = pCurrent;
					p->g = newG;
					p->f = p->g + p->h;
				}
			}
		}

		if (openSet.empty())
		{
			break;
		}

		std::vector<Tile*>::iterator iter = std::min_element(openSet.begin(), openSet.end(),
			[](Tile* const pA, Tile* const pB)
			{
				return pA->f < pB->f;
			});

		pCurrent = *iter;
		openSet.erase(iter);
		pCurrent->mbOpenSet = false;
		pCurrent->mbClosedSet = true;

	} while (pCurrent != pGoal);

	return pCurrent == pGoal;
}

void Grid::BuildTower()
{
	if (mSelectedTile != nullptr && !mSelectedTile->mbBlocked)
	{
		mSelectedTile->mbBlocked = true;

		Tile* const pStartTile = GetStartTile();
		Tile* const pEndTile = GetEndTile();

		if (FindPath(pEndTile, pStartTile))
		{
			Tower* const pTower = new Tower(GetGame());
			pTower->SetPosition(mSelectedTile->GetPosition());
		}
		else
		{
			mSelectedTile->mbBlocked = false;
			FindPath(pEndTile, pStartTile);
		}
		updatePathTiles(pStartTile);
	}
}

Tile* Grid::GetStartTile()
{
	return mTiles[3][0];
}

Tile* Grid::GetEndTile()
{
	return mTiles[3][15];
}

void Grid::UpdateActor(const float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	mNextEnemy -= deltaTime;
	if (mNextEnemy <= 0.f)
	{
		new Enemy(GetGame());
		mNextEnemy += ENEMY_TIME;
	}
}

void Grid::selectTile(const size_t row, const size_t col)
{
	Tile::ETileState tileState = mTiles[row][col]->GetTileState();
	if (tileState != Tile::ETileState::START && tileState != Tile::ETileState::BASE)
	{
		if (mSelectedTile != nullptr)
		{
			mSelectedTile->ToggleSelect();
		}

		mSelectedTile = mTiles[row][col];
		mSelectedTile->ToggleSelect();
	}
}

void Grid::updatePathTiles(Tile* const pStart)
{
	for (size_t i = 0; i < NUM_ROWS; ++i)
	{
		for (size_t j = 0; j < NUM_COLS; ++j)
		{
			if (i != 3 || j != 0 && j != 15)
			{
				mTiles[i][j]->SetTileState(Tile::ETileState::DEFAULT);
			}
		}
	}

	Tile* pTile = pStart->mpParent;
	const Tile* const pEndtile = GetEndTile();
	while (pTile != pEndtile)
	{
		pTile->SetTileState(Tile::ETileState::PATH);
		pTile = pTile->mpParent;
	}
}
