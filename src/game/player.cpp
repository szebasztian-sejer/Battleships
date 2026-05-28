#include <player.h>
#include <ship.h>

Player::Player()
{

}

bool Player::isLost()
{
	for (int y = 0; y < board.h; y++)
	{
		for (int x = 0; x < board.w; x++)
		{
			if (board.getSquare(Vector2{ (float)x,(float)y }) == Board::SquareState::SHIP)
			{
				return false;
			}
		}
	}
	return true;
}