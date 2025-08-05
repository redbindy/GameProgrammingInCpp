#include "Tile.h"

#include "SpriteComponent.h"
#include "Game.h"

Tile::Tile(Game* const pGame)
	: Actor(pGame)
	, mAdjacent()
	, mTileState(ETileState::DEFAULT)
	, mpParent(nullptr)
	, f(0.f)
	, g(0.f)
	, h(0.f)
	, mbOpenSet(false)
	, mbClosedSet(false)
	, mbBlocked(false)
	, mbSelected(false)
	, mpSprite(nullptr)
{
	mpSprite = new SpriteComponent(this);
	updateTexture();
}

void Tile::SetTileState(const ETileState state)
{
	mTileState = state;
	updateTexture();
}

void Tile::ToggleSelect()
{
	mbSelected = !mbSelected;
	updateTexture();
}

void Tile::updateTexture()
{
	std::string text;
	switch (mTileState)
	{
	case ETileState::START:
		text = "Assets/TileTan.png";
		break;

	case ETileState::BASE:
		text = "Assets/TileGreen.png";
		break;

	case ETileState::PATH:
		if (mbSelected)
		{
			text = "Assets/TileGreySelected.png";
		}
		else
		{
			text = "Assets/TileGrey.png";
		}
		break;

	default:
		if (mbSelected)
		{
			text = "Assets/TileBrownSelected.png";
		}
		else
		{
			text = "Assets/TileBrown.png";
		}
		break;
	}

	Game* const pGame = GetGame();

	SDL_Texture* const pTexture = pGame->GetTexture(text);
	mpSprite->SetTexture(pTexture);
}