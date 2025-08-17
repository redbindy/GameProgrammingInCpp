#pragma once

#include <string>
#include <unordered_map>
#include <SDL3_ttf/SDL_ttf.h>

#include "Math.h"

class Game;
class Texture;

class Font final
{
public:
	Font(Game* const pGame);
	virtual ~Font();

	bool Load(const std::string& fileName);

	void Unload();

	Texture* RenderText(const std::string& text, const Vector3& color = Color::White, int pointSize = 30);

private:
	std::unordered_map<int, TTF_Font*> mFontData;

	Game* mpGame;
};