#include <board.h>
#include <iostream>

Board::Board()
{
	squares.resize(h, std::vector<SquareState>(w, SquareState::EMPTY));
}

void Board::setSquare(Vector2 position, SquareState state)
{
	squares[position.y][position.x] = state;
	std::cout << "Successfully set square at x: " << position.x << " y: " << position.y << "\n";
}
Board::SquareState Board::getSquare(Vector2 position)
{
	return squares[position.y][position.x];
}