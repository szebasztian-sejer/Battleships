#include <shipMask.h>
#include <board.h>

ShipMask::ShipMask(Ship::Alignment alignment, int size, Vector2 position)
{
	this->alignment = alignment;
	this->size = size;
	this->position = position;

	updateSquares();

}

bool ShipMask::isValid(Board& board) const
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

Rectangle ShipMask::getShipMaskSource(bool valid) const
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

	y = valid ? 0 : 32;
	return { x,y,w,h };
}

void ShipMask::updateSquares()
{
	squares.clear();
	auto inc = alignment == Ship::Alignment::HORIZONTAL ? Vector2{ 1,0 } : Vector2{ 0,1 };
	Vector2 next = { position.x, position.y };
	for (int i = 0; i < size; i++)
	{
		squares.push_back(next);
		next.x += inc.x;
		next.y += inc.y;
	}
}