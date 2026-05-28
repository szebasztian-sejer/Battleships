#include <ship.h>
#include <board.h>
#include <iostream>

Ship::Ship(Alignment alignment, int size, Vector2 position)
{
	id = shipID;
	std::cout << "New ship created with ID: " << id << "\n";
	shipID++;
	this->alignment = alignment;
	this->size = size;
	this->position = position;

	auto inc = alignment == Alignment::HORIZONTAL ? Vector2{ 1,0 } : Vector2{ 0,1 };
	Vector2 next = { position.x, position.y };
	for (int i = 0; i < size; i++)
	{
		squares.push_back(next);
		next.x += inc.x;
		next.y += inc.y;
	}

}
bool Ship::isValid(Board& board) const
{
	for (auto& pos : squares)
	{
		if (pos.x < 0 || pos.x >= board.w || pos.y < 0 || pos.y >= board.h)
		{
			return false;
		}
		if (board.getSquare(pos) != Board::SquareState::EMPTY)
		{
			return false;
		}
	}
	return true;
}

bool Ship::isSunk(Board& board) const
{
	for (auto& square : squares)
	{
		if (board.getSquare(square) == Board::SquareState::SHIP)
		{
			return false;
		}
	}
	return true;
}
bool Ship::checkAndSink(Board& board, Board* oppBoard)
{
	if (isSunk(board))
	{
		for (auto& square : squares)
		{
			board.setSquare(square, Board::SquareState::SUNK);
			if (oppBoard)
			{
				oppBoard->setSquare(square, Board::SquareState::SUNK);
			}
		}
		state = ShipState::SUNK;
		return true;
	}
	return false;
}



Rectangle Ship::getShipSource() const
{
	float x = 0;
	float y = 0;
	float w = 0;
	float h = 32;

	switch (size)
	{
		case 2:
		{
			x = 0;
			w = 64;
			break;
		}
		case 3:
		{
			x = 64;
			w = 96;
			break;
		}
		case 4:
		{
			x = 160;
			w = 128;
			break;
		}
		case 5:
		{
			x = 288;
			w = 160;
			break;
		}

	}
	switch (state)
	{
		case ShipState::FLOATING:
		{
			y = 0;
			break;
		}
	
		case ShipState::SUNK:
		{
			y = 64;
			break;
		}
	}

	return { x,y,w,h };
}

