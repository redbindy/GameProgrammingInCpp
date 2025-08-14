#include "Font.h"

#include "Texture.h"
#include "Game.h"

Font::Font(Game* const pGame)
	: mFontData()
	, mpGame(pGame)
{
}

Font::~Font()
{
}

bool Font::Load(const std::string& fileName)
{
	std::vector<int> fontSizes = {
		8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28,
		30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 52, 56, 60, 64, 68, 72
	};

	for (const int size : fontSizes)
	{
		TTF_Font* const pFont = TTF_OpenFont(fileName.c_str(), size);
		if (pFont == nullptr)
		{
			SDL_Log("Failed to load font %s in size %d", fileName.c_str(), size);

			return false;
		}

		mFontData.insert({ size, pFont });
	}

	return true;
}

void Font::Unload()
{
	for (std::pair<const int, TTF_Font*>& pair : mFontData)
	{
		TTF_CloseFont(pair.second);
	}
}

Texture* Font::RenderText(const std::string& textKey, const Vector3& color, int pointSize)
{
	Texture* pTexture = nullptr;

	SDL_Color sdlColor;
	sdlColor.r = static_cast<uint8_t>(color.X * 255.f);
	sdlColor.g = static_cast<uint8_t>(color.Y * 255.f);
	sdlColor.b = static_cast<uint8_t>(color.Z * 255.f);
	sdlColor.a = 255;

	std::unordered_map<int, TTF_Font*>::iterator iter = mFontData.find(pointSize);
	if (iter != mFontData.end())
	{
		TTF_Font* const pFont = iter->second;

		const std::string& actualTeext = mpGame->GetText(textKey);

		SDL_Surface* const pSurf = TTF_RenderText_Blended(pFont, actualTeext.c_str(), actualTeext.size(), sdlColor);

		if (pSurf != nullptr)
		{
			SDL_Surface* pRGBA = SDL_ConvertSurface(pSurf, SDL_PIXELFORMAT_RGBA32);
			SDL_DestroySurface(pSurf);

			pTexture = new Texture();
			pTexture->CreateFromSurface(pRGBA);
		}
	}
	else
	{
		SDL_Log("Point size %d is unsupported", pointSize);
	}

	return pTexture;
}
