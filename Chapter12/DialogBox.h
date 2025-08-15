#pragma once

#include "UIScreen.h"

class DialogBox final : public UIScreen
{
public:
	DialogBox(Game* const pGame, const std::string& text, std::function<void()> onOk);
	virtual ~DialogBox();

private:
};