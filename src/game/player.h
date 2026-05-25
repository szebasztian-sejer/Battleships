#pragma once
#include <vector>
#include <board.h>
#include <raylib.h>
#include <algorithm>

struct Ship;

struct Player
{
	Player();
	Board board;
	std::vector<Ship> ships;
	Board::SquareState shots[100];
	bool isLost();

	virtual void setBoard() = 0;
};