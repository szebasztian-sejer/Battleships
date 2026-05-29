#include <ai.h>
#include <helpers.h>
#include <ship.h>
#include <raylib.h>
#include <algorithm>
#include <shipMask.h>

AI::AI()
{
	board.drawRec = { 500,100,320,320 };
	std::fill(shots, shots + 100, Board::SquareState::EMPTY);
	remaining.resize(100);
	for (int i = 0; i < remaining.size(); i++)
	{
		remaining[i] = i;
	}
}

void AI::setBoard()
{
	int startingShips[] = { 5,4,3,3,2 };
	for (auto& i : startingShips)
	{
		bool placed = false;
		do
		{
			placed = false;
			int x = getRandomInt(0, board.w - 1);
			int y = getRandomInt(0, board.h - 1);
			Vector2 position = { (float)x, (float)y };
			int a = getRandomInt(0, 1);
			Ship::Alignment alignment = a == 0 ? Ship::Alignment::HORIZONTAL : Ship::Alignment::VERTICAL;

			ShipMask shipMask(alignment, i, position);
			if(shipMask.isValid(board))
			{
				placed = true;
				Ship ship(shipMask.alignment, shipMask.size, shipMask.position);
				ships.push_back(ship);
				for (auto& pos : ship.squares)
				{
					board.setSquare(pos, Board::SquareState::SHIP);
				}
			}
			
		} while (!placed);
	}
}

Vector2 AI::easyAiTarget()
{
	while(true)
	{
		int i = getRandomInt(0, remaining.size()-1);
		int pos = remaining[i];
		float x = pos % playerBoard.w;
		float y = pos / playerBoard.w;
		Vector2 target = { x,y };
		if (playerBoard.getSquare(target) == Board::SquareState::EMPTY)
		{
			remaining[i] = remaining.back();
			remaining.pop_back();
			return target;
		}
	}
}

Vector2 AI::seek()
{
	return easyAiTarget();
}
Vector2 AI::destroy()
{
	if (remainingShips.empty()) { std::cout << "PLAYER SHIPS EMPTY ERROR!!\n";  return seek(); }
	int intBoard[10][10] = {};
	for (const auto& t : currentTargets)
	{
		int tx = (int)t.x;
		int ty = (int)t.y;
		int absMax = *std::max_element(remainingShips.begin(), remainingShips.end());
		int rowMin = tx;
		int colMin = ty;

		for (int x = std::max(0, tx - 1); x >= std::max(0, tx - absMax); x--)
		{
			auto sq = playerBoard.getSquare(Vector2{ (float)x, t.y });
			if (sq == Board::SquareState::MISSED || sq == Board::SquareState::SUNK) { break; }
			rowMin--;
		}
		rowMin = std::max(0, rowMin);
		for (int x = rowMin; x <= tx; x++)
		{
			for (auto size : remainingShips)
			{
				auto sMask = ShipMask(Ship::Alignment::HORIZONTAL, size, Vector2{ (float)x,t.y});
				if (sMask.isValid(playerBoard))
				{
					for (const auto& square : sMask.squares)
					{
						if (playerBoard.getSquare(square) == Board::SquareState::EMPTY)
						{
							intBoard[(int)square.y][(int)square.x]++;
						}
						else
						{
							intBoard[(int)square.y][(int)square.x] = -2;
						}
					}
				}
			}
		}

		for (int y = std::max(0, ty - 1); y >= std::max(0, ty - absMax); y--)
		{
			auto sq = playerBoard.getSquare(Vector2{ t.x, (float)y });
			if (sq == Board::SquareState::MISSED || sq == Board::SquareState::SUNK) { break; }
			colMin--;
		}
		colMin = std::max(0, colMin);
		for (int y = colMin; y <= ty; y++)
		{
			for (auto size : remainingShips)
			{
				auto sMask = ShipMask(Ship::Alignment::VERTICAL, size, Vector2{ t.x, (float)y });
				if (sMask.isValid(playerBoard))
				{
					for (const auto& square : sMask.squares)
					{
						if (playerBoard.getSquare(square) == Board::SquareState::EMPTY)
						{
							intBoard[(int)square.y][(int)square.x]++;
						}
					}
				}
			}
		}
	}

	int max = -1;
	Vector2 best = { -1,0 };
	for (int y = 0; y < playerBoard.h; y++)
	{
		for (int x = 0; x < playerBoard.w; x++)
		{
			if (intBoard[y][x] > max)
			{
				max = intBoard[y][x];
				best = Vector2{ float(x), float(y) };
			}
			std::cout << intBoard[y][x] << "  ";
		}
		std::cout << "\n";
	}

	if (best.x == -1) { std::cout << "No good candidae found, defaulting to seeking\n";  return seek(); }
	std::cout << "Shooting target: {" << best.x << "," << best.y << "} with score: " << max << "\n";
	return best;
}

Vector2 AI::mediumAiTarget()
{
	currentTargets.clear();
	for (int y = 0; y < playerBoard.h; y++)
	{
		for (int x = 0; x < playerBoard.w; x++)
		{
			Vector2 target = { float(x),float(y) };
			if (playerBoard.getSquare(target) == Board::SquareState::HIT)
			{
				currentTargets.push_back(target);
			}
		}
	}
	if (currentTargets.empty())
	{
		std::cout << "Current targets empty, seeking...\n";
		return seek();
	}
	return destroy();
}
