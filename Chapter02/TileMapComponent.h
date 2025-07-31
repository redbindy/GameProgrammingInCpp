#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <string>

#include "SpriteComponent.h"
#include "Math.h"

class Actor;

class TileMapComponent final : public SpriteComponent
{
public:
	TileMapComponent(Actor* const pOwner, const int drawOrder);
	TileMapComponent(Actor* const pOwner);
	virtual ~TileMapComponent() = default;

	void Draw(SDL_Renderer* const pRenderer) override;

private:
	struct TileInfo
	{
		Vector2 pos;
		int tileIndex;
	};

	enum
	{
		LAYER_COUNT = 3, 
		TILE_SIZE = 32, 
		N_ROW_TILE = 8
	};

private:
	std::vector<TileInfo> mTileInfos[LAYER_COUNT];
};