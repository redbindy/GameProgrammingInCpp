#include "TileMapComponent.h"

#include "Game.h"

TileMapComponent::TileMapComponent(Actor* const pOwner, const int drawOrder)
	: SpriteComponent(pOwner, drawOrder)
	, mTileInfos{}
{
	const char* TILE_SET_PATH = "Assets/Tiles.png";

	Game* const pGame = pOwner->GetGame();
	SDL_Texture* const pTileSet = pGame->GetTexture(TILE_SET_PATH);

	SetTexture(pTileSet);

	for (int i = 0; i < LAYER_COUNT; ++i)
	{
		mTileInfos[i].reserve(SCREEN_WIDTH / TILE_SIZE);

		const char* layerPathFormat = "Assets/MapLayer%d.csv";
		char buffer[128];

		sprintf(buffer, layerPathFormat, i + 1);
		std::ifstream fin;
		fin.open(buffer);
		{
			TileInfo info;
			info.pos = Vector2();
			info.tileIndex = 0;

			while (true)
			{
				fin.getline(buffer, INT32_MAX);
				if (fin.eof())
				{
					break;
				}

				info.pos.X = 0;

				const char* token = strtok(buffer, ",");
				info.tileIndex = atoi(token);

				mTileInfos[i].push_back(info);

				while (true)
				{
					token = strtok(nullptr, ",");
					if (token == nullptr)
					{
						break;
					}

					info.tileIndex = atoi(token);
					++info.pos.X;

					if (info.tileIndex == -1)
					{
						continue;
					}

					mTileInfos[i].push_back(info);
				}
				++info.pos.Y;
			}
		}
		fin.close();
	}
}

TileMapComponent::TileMapComponent(Actor* const pOwner)
	: TileMapComponent(pOwner, 10)
{

}

void TileMapComponent::Draw(SDL_Renderer* const pRenderer)
{
	for (int i = LAYER_COUNT - 1; i >= 0; --i)
	{
		const std::vector<TileInfo>& infos = mTileInfos[i];
		{
			for (const TileInfo& info : infos)
			{
				SDL_FRect srcRect;
				srcRect.x = (info.tileIndex % N_ROW_TILE) * TILE_SIZE;
				srcRect.y = (info.tileIndex / N_ROW_TILE) * TILE_SIZE;
				srcRect.w = TILE_SIZE;
				srcRect.h = TILE_SIZE;

				SDL_FRect dstRect;
				dstRect.x = info.pos.X * TILE_SIZE;
				dstRect.y = info.pos.Y * TILE_SIZE;
				dstRect.w = TILE_SIZE;
				dstRect.h = TILE_SIZE;

				SDL_RenderTexture(
					pRenderer,
					mpTexture,
					&srcRect,
					&dstRect
				);
			}
		}
	}
}
