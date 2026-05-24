#pragma once
#include <player.h>

struct AI : Player
{
	AI();
	void setBoard();
	Board playerBoard;

	Vector2 easyAiTarget();
};