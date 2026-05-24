#include <human.h>
#include <helpers.h>
#include <ship.h>
#include <raylib.h>

Human::Human()
{
	board.drawRec = { 100,100,320,320 };
}

void Human::setBoard()
{
	int startingShips[] = { 5,4,3,3,2 };
	for (auto& i : startingShips)
	{
		bool placed = false;
		do
		{	
			placed = true;
			int x = getRandomInt(0, board.w - 1);
			int y = getRandomInt(0, board.h - 1);
			Vector2 position = { (float)x, (float)y };
			int a = getRandomInt(0, 1);
			Ship::Alignment alignment = a == 0 ? Ship::Alignment::HORIZONTAL : Ship::Alignment::VERTICAL;

			Ship ship(alignment, i, position);
			for (auto& pos : ship.squares)
			{
				if (!ship.isValid(board))
				{
					placed = false;
					break;
				}
				
			}
			if (placed)
			{
				ships.push_back(ship);
				for (auto& pos : ship.squares)
				{
					board.setSquare(pos, Board::SquareState::SHIP);
				}
			}
			else
			{
				shipID--;
			}
		} while (!placed);
	}
}
