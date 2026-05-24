#pragma once
#include <raylib.h>
#include <vector>

struct Board
{
	int w = 10;
	int h = 10;
	Rectangle drawRec = {};
	enum SquareState
	{
		EMPTY = 0,
		HIT,
		MISSED,
		SHIP,
		SUNK,
	};

	std::vector<std::vector<SquareState>> squares;

	Board();
	SquareState getSquare(Vector2 position);
	void setSquare(Vector2 position, SquareState state);
	

};