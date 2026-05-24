#include <player.h>
#include <ship.h>

Player::Player()
{

}

bool Player::isLost()
{
	for (int i = 0; i < board.h; i++)
	{
		for (int j = 0; j < board.w; j++)
		{
			if (board.getSquare(Vector2{ (float)i,(float)j }) == Board::SquareState::SHIP)
			{
				return false;
			}
		}
	}
	return true;
}