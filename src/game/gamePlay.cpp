#include <gamePlay.h>
#include <ui.h>
#include <ship.h>
#include <assetManager.h>

void GamePlay::drawHumanBoard(AssetManager& assetManager)
{
	for (auto& ship : human.ships)
	{
		Rectangle source = ship.getShipSource(true);
		Rectangle dest;
		dest.x = human.board.drawRec.x + 32 * ship.position.x;
		dest.y = human.board.drawRec.y + 32 * ship.position.y;
		dest.width = ship.size * 32;
		dest.height = 32;
		float rotation = 0.0f;
		if (ship.alignment == Ship::Alignment::VERTICAL)
		{
			rotation = 90.0f;
			dest.x += 32;
		}

		DrawTexturePro(
			assetManager.ships,
			source,
			dest,
			{ 0,0 },
			rotation,
			WHITE
		);
	}
	for (auto& pos : ai.misses)
	{
		Rectangle dest;
		dest.x = human.board.drawRec.x + 32 * pos.x;
		dest.y = human.board.drawRec.y + 32 * pos.y;
		dest.width = 32;
		dest.height = 32;
		DrawTexturePro(
			assetManager.miss,
			{ 0,0,32,32 },
			dest,
			{ 0,0 },
			0.0f,
			WHITE
		);
	}
	for (auto& pos : ai.hits)
	{
		Rectangle dest;
		dest.x = human.board.drawRec.x + 32 * pos.x;
		dest.y = human.board.drawRec.y + 32 * pos.y;
		dest.width = 32;
		dest.height = 32;
		DrawTexturePro(
			assetManager.hit,
			{ 0,0,32,32 },
			dest,
			{ 0,0 },
			0.0f,
			WHITE
		);
	}
}

void GamePlay::drawAiBoard(AssetManager& assetManager)
{
	for (auto& pos : human.misses)
	{
		Rectangle dest;
		dest.x = ai.board.drawRec.x + 32 * pos.x;
		dest.y = ai.board.drawRec.y + 32 * pos.y;
		dest.width = 32;
		dest.height = 32;
		DrawTexturePro(
			assetManager.miss,
			{ 0,0,32,32 },
			dest,
			{ 0,0 },
			0.0f,
			WHITE
		);
	}
	for (auto& pos : human.hits)
	{
		Rectangle dest;
		dest.x = ai.board.drawRec.x + 32 * pos.x;
		dest.y = ai.board.drawRec.y + 32 * pos.y;
		dest.width = 32;
		dest.height = 32;
		DrawTexturePro(
			assetManager.hit,
			{ 0,0,32,32 },
			dest,
			{ 0,0 },
			0.0f,
			WHITE
		);
	}
}

void GamePlay::drawGrid(float boardX, float boardY)
{
	for (int i = 0; i <= 10; i++)
	{
		DrawLine(
			boardX + i * 32,
			boardY,
			boardX + i * 32,
			boardY + 320,
			BLACK
		);

		DrawLine(
			boardX,
			boardY + i * 32,
			boardX + 320,
			boardY + i * 32,
			BLACK
		);
	}
}

bool GamePlay::init(Difficulty d)
{
	human.setBoard();
	ai.setBoard();
	camera.target = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f };  // world-space center of view
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;
	difficulty = d;
	state = State::PLACEMENT;

	return true;
}
bool GamePlay::update(AssetManager& assetManager)
{
	
	camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

	ClearBackground({ 75, 75, 150, 255 });

	BeginMode2D(camera);


	Vector2 topLeftView = GetScreenToWorld2D({ 0, 0 }, camera);
	Vector2 bottomRightView = GetScreenToWorld2D({ (float)GetScreenWidth(), (float)GetScreenHeight() }, camera);

	DrawTexturePro(
		assetManager.board,
		{ 0,0,320,320 },
		human.board.drawRec,
		{ 0,0 },
		{ 0.0f },
		WHITE
	);
	DrawTexturePro(
		assetManager.board,
		{ 0,0,320,320 },
		ai.board.drawRec,
		{ 0,0 },
		{ 0.0f },
		WHITE
	);

	drawHumanBoard(assetManager);
	drawAiBoard(assetManager);

	drawGrid(human.board.drawRec.x, human.board.drawRec.y);
	drawGrid(ai.board.drawRec.x, ai.board.drawRec.y);

	switch (state)
		{
		case State::PLACEMENT:
		{
			state = State::HUMAN_TURN;
			break;
		}
		case State::HUMAN_TURN:
		{
			Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), camera);

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
					if (checkHit(ai.board, selectedCell))
					{
						ai.board.setSquare(selectedCell, Board::SquareState::HIT);
						human.hits.push_back(selectedCell);
					}
					else
					{
						ai.board.setSquare(selectedCell, Board::SquareState::MISSED);
						human.misses.push_back(selectedCell);
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
			if (checkHit(human.board, target))
			{
				human.board.setSquare(target, Board::SquareState::HIT);
				ai.playerBoard.setSquare(target, Board::SquareState::HIT);
				ai.hits.push_back(target);
			}
			else
			{
				ai.playerBoard.setSquare(target, Board::SquareState::MISSED);
				ai.misses.push_back(target);
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

	Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), camera);

	int cellX = (int)floor((worldPos.x - board.drawRec.x) / CELL_SIZE);
	int cellY = (int)floor((worldPos.y - board.drawRec.y) / CELL_SIZE);

	return 
	{
		(float)cellX,
		(float)cellY
	};
}