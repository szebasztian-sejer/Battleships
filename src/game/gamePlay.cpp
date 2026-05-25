#include <gamePlay.h>
#include <ui.h>
#include <ship.h>
#include <assetManager.h>


bool GamePlay::init(Difficulty d)
{
	human.setBoard();
	ai.setBoard();
	
	difficulty = d;
	state = State::PLACEMENT;

	return graphics.init();
}
bool GamePlay::update(AssetManager& assetManager)
{
	
	graphics.update();

	graphics.drawHumanBoard(assetManager, human.board, human.ships, ai.shots);
	graphics.drawAiBoard(assetManager, ai.board, human.shots);

	graphics.drawGrid(human.board.drawRec.x, human.board.drawRec.y);
	graphics.drawGrid(ai.board.drawRec.x, ai.board.drawRec.y);

	switch (state)
		{
		case State::PLACEMENT:
		{
			state = State::HUMAN_TURN;
			break;
		}
		case State::HUMAN_TURN:
		{
			Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), graphics.camera);

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
					if (checkHit(ai.board, selectedCell))
					{
						ai.board.setSquare(selectedCell, Board::SquareState::HIT);	
						human.shots[pos] = Board::SquareState::HIT;
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
			break;
		}
		case State::AI_TURN:
		{
			Vector2 target = ai.easyAiTarget();
			int pos = (int)target.y * human.board.w + (int)target.x;
			if (checkHit(human.board, target))
			{
				human.board.setSquare(target, Board::SquareState::HIT);
				ai.playerBoard.setSquare(target, Board::SquareState::HIT);
				ai.shots[pos] = Board::SquareState::HIT;
			}
			else
			{
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

	Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), graphics.camera);

	int cellX = (int)floor((worldPos.x - board.drawRec.x) / CELL_SIZE);
	int cellY = (int)floor((worldPos.y - board.drawRec.y) / CELL_SIZE);

	return 
	{
		(float)cellX,
		(float)cellY
	};
}