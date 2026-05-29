#include <gamePlay.h>
#include <ui.h>
#include <ship.h>
#include <assetManager.h>
#include <iostream>

bool GamePlay::init(Difficulty d)
{
	//human.setBoard();
	ai.setBoard();
	
	difficulty = d;
	state = State::PLACEMENT;

	return graphics.init();
}

bool GamePlay::updateShips(Player* player, Board* oppBoard)
{
	bool result = false;
	for (auto& ship : player->ships)
	{
		if (ship.checkAndSink(player->board, oppBoard))
		{
			bool newSink = alreadySunk.insert(ship).second;
			if (newSink) { result = true; }
		}
	}
	return result;
}
bool GamePlay::update(AssetManager& assetManager)
{

	if (!graphics.update()) { return false; }

	graphics.drawHumanBoard(assetManager, human.board, human.ships, ai.shots);
	graphics.drawAiBoard(assetManager, ai.board, ai.ships, human.shots);

	graphics.drawGrid(human.board.drawRec.x, human.board.drawRec.y);
	graphics.drawGrid(ai.board.drawRec.x, ai.board.drawRec.y);
	graphics.drawBottomRec();
	


	Vector2 mouseWorld = graphics.getMouse();
	if (graphics.drawQuitButton(mouseWorld)) { return false; }
	auto targetPos = getSelectPosition(ai.board);

	switch (state)
	{
		case State::PLACEMENT:
		{
			if (graphics.shipPlaces.empty()) { state = State::HUMAN_TURN; }
			for (int i = 0; i < graphics.shipPlaces.size(); i++)
			{
				auto& shipPlace = graphics.shipPlaces[i];
				shipPlace.isHovered = CheckCollisionPointRec(mouseWorld, shipPlace.dstRec);
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && shipPlace.isHovered && shipPlace.active)
				{
					shipPlace.active = false;
					selectedShip = i;
					shipMask = std::make_unique<ShipMask>(
						Ship::HORIZONTAL,
						graphics.shipPlaces[i].size,
						getSelectPosition(human.board)
						);
				}

			}
			if (shipMask)
			{
				shipMask->position = getSelectPosition(human.board);
				shipMask->updateSquares();
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
				{
					if (shipMask->isValid(human.board))
					{
						Ship s(shipMask->alignment, shipMask->size, shipMask->position);
						human.ships.push_back(s);
						for (auto& pos : s.squares)
						{
							human.board.setSquare(pos, Board::SquareState::SHIP);
						}
						graphics.shipPlaces[selectedShip] = graphics.shipPlaces.back();
						graphics.shipPlaces.pop_back();
						shipMask = nullptr;
					}
				}
				if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
				{
					shipMask->alignment = shipMask->alignment == Ship::HORIZONTAL ? Ship::VERTICAL : Ship::HORIZONTAL;
					shipMask->updateSquares();
				}
				if (IsKeyPressed(KEY_BACKSPACE))
				{
					graphics.shipPlaces[selectedShip].active = true;
					shipMask = nullptr;
				}
			}

			graphics.drawPlacementsUi(assetManager, shipMask.get(), human.board);
			break;
		}
		case State::HUMAN_TURN:
		{

			if (CheckCollisionPointRec(mouseWorld, ai.board.drawRec))
			{
				Vector2 selectedCell = getSelectPosition(ai.board);
				Vector2 drawSelectPos = { ai.board.drawRec.x + selectedCell.x * 32, ai.board.drawRec.y + selectedCell.y * 32 };

				DrawTexturePro(
					assetManager.select,
					{ 0, 0, (float)assetManager.select.width, (float)assetManager.select.height },
					{ drawSelectPos.x, drawSelectPos.y, 32, 32 },
					{ 0, 0 },
					0.0f,
					WHITE
				);

				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
				{
					int pos = (int)selectedCell.y * ai.board.w + (int)selectedCell.x;
					if (human.shots[pos] == Board::SquareState::EMPTY)
					{
						if (checkHit(ai.board, selectedCell))
						{
							ai.board.setSquare(selectedCell, Board::SquareState::HIT);
							human.shots[pos] = Board::SquareState::HIT;
							updateShips(&ai);
						}
						else
						{
							ai.board.setSquare(selectedCell, Board::SquareState::MISSED);
							human.shots[pos] = Board::SquareState::MISSED;
						}
						if (ai.isLost())
						{
							state = State::HUMAN_WIN;
						}
						else
						{
							state = State::AI_TURN;
						}
					}
				}
			}

			graphics.drawGameUI(assetManager, targetPos, ai.ships);
			break;
		}
		case State::AI_TURN:
		{
			Vector2 target = ai.mediumAiTarget();
			std::cout << "Targetting: {" << target.y << "," << target.x << "}\n";
			int pos = (int)target.y * human.board.w + (int)target.x;
			if (checkHit(human.board, target))
			{
				human.board.setSquare(target, Board::SquareState::HIT);
				ai.playerBoard.setSquare(target, Board::SquareState::HIT);
				ai.shots[pos] = Board::SquareState::HIT;
				if (updateShips(&human, &(ai.playerBoard)))
				{
					ai.currentTargets.clear();
					
					int cruisersSunk = 0;
					for (const auto& s : human.ships)
					{
						if (s.isSunk(human.board) && s.size == 3)
						{
							cruisersSunk++;
						}
					}
					if (cruisersSunk == 1)
					{
						ai.remainingShips = { 5,4,3,2 };
					}
					else if (cruisersSunk == 2)
					{
						ai.remainingShips = { 5,4,2 };
					}
					

					for (const auto& s : human.ships)
					{
						if (s.isSunk(human.board) && s.size != 3 ) 
						{
							for (int i = 0; i < ai.remainingShips.size(); i++)
							{
								if (ai.remainingShips[i] == s.size)
								{
									ai.remainingShips[i] = ai.remainingShips.back();
									ai.remainingShips.pop_back();
								}
							}
						}
					}

				}
			}
			else
			{
				human.board.setSquare(target, Board::SquareState::MISSED);
				ai.playerBoard.setSquare(target, Board::SquareState::MISSED);
				ai.shots[pos] = Board::SquareState::MISSED;
			}
			if (human.isLost())
			{
				state = State::AI_WIN;
			}
			else
			{
				state = State::HUMAN_TURN;
			}
			break;
		}
		case State::AI_WIN:
		{
			return false;
		}
		case State::HUMAN_WIN:
		{
			return false;
		}

	}

	EndMode2D();

	return true;
}

bool GamePlay::checkHit(Board& board, Vector2 target)
{
	return board.getSquare(target) == Board::SquareState::SHIP;
}

Vector2 GamePlay::getSelectPosition(Board& board)
{
	constexpr int CELL_SIZE = 32;

	Vector2 worldPos = graphics.getMouse();

	int cellX = (int)floor((worldPos.x - board.drawRec.x) / CELL_SIZE);
	int cellY = (int)floor((worldPos.y - board.drawRec.y) / CELL_SIZE);

	return 
	{
		(float)cellX,
		(float)cellY
	};
}