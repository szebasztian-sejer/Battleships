#pragma once

#include <raylib.h>
#include <vector>
#include <ship.h>
#include <iostream>

struct Board;

struct ShipMask
{
	ShipMask(Ship::Alignment alignment, int size, Vector2 position);
	int size = 2;
	Vector2 position = {};
	Rectangle drawRec = {};

	Ship::Alignment alignment = Ship::Alignment::HORIZONTAL;
	std::vector<Vector2> squares;
	void updateSquares();

	bool isValid(Board& board) const;
	Rectangle getShipMaskSource(bool valid = true) const;
	~ShipMask()
	{
	}
};