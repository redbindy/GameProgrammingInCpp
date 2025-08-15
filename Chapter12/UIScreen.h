#pragma once

#include <string>
#include <functional>
#include <vector>

#include "Math.h"

class Game;
class Shader;
class Font;
class Texture;

class Button final
{
public:
	Button(
		const std::string& name,
		Font* const pFont,
		std::function<void()> onClick,
		const Vector2& pos,
		const Vector2& dims
	);
	virtual ~Button();

	void SetName(const std::string& name);

	bool ContainsPoint(const Vector2& pt) const;

	void OnClick();

	Texture* GetNameTex()
	{
		return mpNameTex;
	}

	const Vector2& GetPosition() const
	{
		return mPosition;
	}

	bool GetHighlighted() const
	{
		return mbHighlighted;
	}

	void SetHighlighted(const bool bSel)
	{
		mbHighlighted = bSel;
	}

private:
	std::function<void()> mOnClickFunc;
	std::string mName;
	Texture* mpNameTex;

	Font* mpFont;

	Vector2 mPosition;
	Vector2 mDimensions;

	bool mbHighlighted;
};

class UIScreen
{
public:
	enum class EUIState
	{
		ACTIVE,
		CLOSING
	};

public:
	UIScreen(Game* const pGame);
	virtual ~UIScreen();

	virtual void Update(const float deltaTime);
	virtual void Draw(Shader* const pShader);
	virtual void ProcessInput(const bool* const pbKeys);
	virtual void HandleKeyPress(const int key);

	void Close();

	void SetTitle(const std::string& text, const Vector3& color = Color::White, int pointSize = 40);

	void AddButton(const std::string& name, std::function<void()> onClick);

	EUIState GetState() const
	{
		return mState;
	}

protected:
	Game* mpGame;

	Font* mpFont;
	Texture* mpTitle;

	Vector2 mTitlePos;

	EUIState mState;

	Texture* mpButtonOn;
	Texture* mpButtonOff;

	Vector2 mNextButtonPos;

	std::vector<Button*> mButtons;

	Vector2 mBGPos;
	Texture* mpBackground;

protected:
	void DrawTexture(
		Shader* const pShader,
		Texture* const pTexture,
		const Vector2& offset = Vector2::Zero,
		const float scale = 1.f
	);

	void setRelativeMouseMode(const bool bRelative);
};