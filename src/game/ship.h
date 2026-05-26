#pragma once
#include <raylib.h>
#include <vector>
#include <iostream>

struct Board;

static int shipID = 0;

struct Ship
{
	int id = 0;
	int size = 2;
	int hits = 0;
	Vector2 position = {};
	Rectangle drawRec = {};
	enum Alignment
	{
		HORIZONTAL = 0,
		VERTICAL,
	};
	enum ShipState
	{
		FLOATING = 0,
		SUNK,
	};

	Alignment alignment = Alignment::HORIZONTAL;
	std::vector<Vector2> squares;
	ShipState state;

	Ship(Alignment alignment, int size, Vector2 position);
	bool isSunk(Board& board) const;
	bool isValid(Board& board) const;
	Rectangle Ship::getShipSource() const;
	void checkAndSink(Board& board);

	void printShip()
	{
		for (auto& s : squares)
		{
			std::cout << "{" << s.x << "," << s.y << "," << size << "},";
		}
		std::cout << "\n";
	}
};