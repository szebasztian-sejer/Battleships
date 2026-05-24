#pragma once
#include <vector>
#include <board.h>
#include <raylib.h>

struct Ship;

struct Player
{
	Player();
	Board board;
	std::vector<Ship> ships;
	std::vector<Vector2> hits;
	std::vector<Vector2> misses;
	bool isLost();

	virtual void setBoard() = 0;
};