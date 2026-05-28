#pragma once
#include <player.h>

struct AI : Player
{
	AI();
	void setBoard();
	Board playerBoard;
	std::vector<int> remaining;
	std::vector<int> remainingShips = { 5,4,3,3,2 };
	std::vector<Vector2> currentTargets;

	Vector2 seek();
	Vector2 destroy();
	Vector2 easyAiTarget();
	Vector2 mediumAiTarget();
};