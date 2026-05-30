#include <board.h>

Board::Board()
{
	squares.resize(h, std::vector<SquareState>(w, SquareState::EMPTY));
}

void Board::setSquare(Vector2 position, SquareState state)
{
	squares[position.y][position.x] = state;
}
Board::SquareState Board::getSquare(Vector2 position)
{
	return squares[position.y][position.x];
}