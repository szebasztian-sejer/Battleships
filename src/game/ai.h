#pragma once
#include <player.h>

struct AI : Player
{
	AI();
	void setBoard();
	Board playerBoard;
	std::vector<int> remaining;

	Vector2 easyAiTarget();
};