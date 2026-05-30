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

	//Hard AI
	std::vector<Vector2> carrierPositions;
	std::vector<Vector2> battleshipPositions;
	std::vector<Vector2> cruiserPositions;
	std::vector<Vector2> destroyerPositions;
	std::vector<std::vector<Vector2>> positionVectors = { destroyerPositions, cruiserPositions, battleshipPositions, carrierPositions };

	Vector2 seek(bool rnd = true);
	Vector2 destroy();
	Vector2 easyAiTarget();
	Vector2 mediumHardAiTarget(bool rng = true);
};