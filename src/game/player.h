#pragma once
#include <vector>
#include <board.h>

struct Ship;

struct Player
{
	Player();
	Board board;
	std::vector<Ship> ships;
	bool isLost();

	virtual void setBoard() = 0;
};