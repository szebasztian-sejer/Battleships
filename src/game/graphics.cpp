#include <graphics.h>
#include <assetManager.h>
#include <ship.h>
#include <shipPlace.h>
#include <string>

bool Graphics::init()
{
	camera.target = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f };  // world-space center of view
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;
	int ships[] = { 5,4,3,3,2 };
	bottomRec = { 16,(float)GetScreenHeight() - 224, (float)GetScreenWidth() - 32, 210 };
	float xOffSet = 32.0f;
	int off = xOffSet;

	for (int i = 0; i < 5; i++)
	{
		ShipPlace s(ships[i], true);
		if (i != 0)
		{
			off += (ships[i - 1] + 1) * xOffSet;
		}
		s.dstRec.x = bottomRec.x + off;
		s.dstRec.y = bottomRec.y + 96;
		s.dstRec.width = ships[i] * 32;
		s.dstRec.height = 32;
		shipPlaces.push_back(s);
		s.active = false;
		aiShips.push_back(s);

	}
	quitButton = { bottomRec.x + bottomRec.width - 100, bottomRec.y+bottomRec.height - 64, 80, 50 };
	gameOverScreen = { GetScreenWidth() * 0.2f, GetScreenHeight() * 0.2f, GetScreenWidth() * 0.6f, GetScreenHeight() * 0.3f };
	

	gameOverQuit.width = quitTextSize * 2;
	gameOverQuit.height = 24 * 1.5f;
	gameOverRestart.width = restartTextSize * 2;
	gameOverRestart.height = 24 * 1.5f;

	gameOverQuit.x = gameOverScreen.x + (gameOverScreen.width / 2 - gameOverQuit.width) / 2;
	gameOverRestart.x = (gameOverScreen.x + gameOverScreen.width/2) + (gameOverScreen.width / 2 - gameOverRestart.width) / 2;
	gameOverQuit.y = gameOverScreen.y + gameOverScreen.height - gameOverQuit.height * 2;
	gameOverRestart.y = gameOverScreen.y + gameOverScreen.height - gameOverRestart.height * 2;

	return true;
}

Vector2 Graphics::getMouse()
{
	
	return GetScreenToWorld2D(GetMousePosition(), camera);
}

bool Graphics::update()
{
	camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

	ClearBackground({ 75, 75, 150, 255 });

	BeginMode2D(camera);


	Vector2 topLeftView = GetScreenToWorld2D({ 0, 0 }, camera);
	Vector2 bottomRightView = GetScreenToWorld2D({ (float)GetScreenWidth(), (float)GetScreenHeight() }, camera);
	return true;
}

void Graphics::drawHumanBoard(AssetManager& assetManager, Board& humanBoard, std::vector<Ship>& ships, Board::SquareState* aiShots)
{
	DrawTexturePro(
		assetManager.board,
		{ 0,0,320,320 },
		humanBoard.drawRec,
		{ 0,0 },
		{ 0.0f },
		WHITE
	);

	for (auto& ship : ships)
	{
		Rectangle source = ship.getShipSource();
		Rectangle dest;
		dest.x = humanBoard.drawRec.x + 32 * ship.position.x;
		dest.y = humanBoard.drawRec.y + 32 * ship.position.y;
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
	for (int y = 0; y < humanBoard.h; y++)
	{
		for (int x = 0; x < humanBoard.w; x++)
		{
			const auto& sq = humanBoard.getSquare(Vector2{ (float)x,(float)y });
			if ( sq != Board::SquareState::HIT && sq != Board::SquareState::MISSED) 
			{
				continue;
			}
			Rectangle dest;
			dest.x = humanBoard.drawRec.x + 32 * x;
			dest.y = humanBoard.drawRec.y + 32 * y;
			dest.width = 32;
			dest.height = 32;
			Texture2D tex = sq == Board::SquareState::HIT ? assetManager.hit : assetManager.miss;
			DrawTexturePro(
				tex,
				{ 0,0,32,32 },
				dest,
				{ 0,0 },
				0.0f,
				WHITE
			);
		}
	}
}

void Graphics::drawAiBoard(AssetManager& assetManager, Board& aiBoard, std::vector<Ship>& ships, Board::SquareState* humanShots)
{
	DrawTexturePro(
		assetManager.board,
		{ 0,0,320,320 },
		aiBoard.drawRec,
		{ 0,0 },
		{ 0.0f },
		WHITE
	);

	for (int i = 0; i < aiBoard.w * aiBoard.h; i++)
	{
		if (humanShots[i] == Board::SquareState::EMPTY) { continue; }
		Rectangle dest;
		dest.x = aiBoard.drawRec.x + 32 * (i % aiBoard.w);
		dest.y = aiBoard.drawRec.y + 32 * (i / aiBoard.w);
		dest.width = 32;
		dest.height = 32;
		Texture2D tex = humanShots[i] == Board::SquareState::HIT ? assetManager.hit : assetManager.miss;
		DrawTexturePro(
			tex,
			{ 0,0,32,32 },
			dest,
			{ 0,0 },
			0.0f,
			WHITE
		);
	}

	for (auto& ship : ships)
	{
		if (ship.state != Ship::ShipState::SUNK)
		{
			continue;
		}
		Rectangle source = ship.getShipSource();
		Rectangle dest;
		dest.x = aiBoard.drawRec.x + 32 * ship.position.x;
		dest.y = aiBoard.drawRec.y + 32 * ship.position.y;
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
}

void Graphics::drawGrid(float boardX, float boardY)
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

void Graphics::drawCoords(float boardX, float boardY)
{
	for (int x = 0; x < 10; x++)
	{
		std::string letter;
		letter += 'A' + x;
		auto textsize = MeasureText(letter.c_str(), 32);
		float textX = (boardX + x * 32 + boardX + x * 32 + textsize) / 2.0f;
		DrawText(letter.c_str(), textX, boardY + 11 * 32, 32, BLACK);
	}
	for (int y = 1; y <= 10; y++)
	{
		std::string number;
		number += std::to_string(y);
		DrawText(number.c_str(), boardX - 32, boardY + (y - 1) * 32, 32, BLACK);
	}
}

void Graphics::drawBottomRec()
{
	Color c = { 10,10,10, 255 };

	DrawRectangle(bottomRec.x, bottomRec.y, bottomRec.width, bottomRec.height, c);

	DrawLineEx({ bottomRec.x + 5, bottomRec.y + 10 }, { bottomRec.x + bottomRec.width - 5, bottomRec.y + 10 }, 10, WHITE);
	DrawLineEx({ bottomRec.x + 5, bottomRec.y + 10 }, { bottomRec.x + 5, bottomRec.y + bottomRec.height - 10 }, 10, WHITE);
	DrawLineEx({ bottomRec.x + 5, bottomRec.y + bottomRec.height - 10 }, { bottomRec.x + bottomRec.width - 5, bottomRec.y + bottomRec.height - 10 }, 10, WHITE);
	DrawLineEx({ bottomRec.x + bottomRec.width - 5, bottomRec.y + 10 }, { bottomRec.x + bottomRec.width - 5, bottomRec.y + bottomRec.height - 10 }, 10, WHITE);

}
void Graphics::drawPlacementsUi(AssetManager& assetManager, ShipMask* shipMask, Board& board)
{
	float pulse = 0.5f + 0.5f * sinf(GetTime() * 5.0f);

	unsigned char brightness = (unsigned char)(190 + pulse * 65);
	unsigned char alpha = (unsigned char)(160 + pulse * 95);

	Color tint = 
	{
		brightness,
		brightness,
		brightness,
		alpha
	};
	std::string helpText1 = "Left click to select and place ships. Right click to rotate.\n";
	std::string helpText2 = "Backspace to cancel selection.";
	int textSize1 = MeasureText(helpText1.c_str(), 24);
	int textSize2 = MeasureText(helpText2.c_str(), 24);

	int text1X = (bottomRec.x + bottomRec.width - textSize1) / 2;
	int text2X = (bottomRec.x + bottomRec.width - textSize2) / 2;
	DrawText(helpText1.c_str(), text1X, bottomRec.y + 24, 24, WHITE);
	DrawText(helpText2.c_str(), text2X, bottomRec.y + 48, 24, WHITE);
	
	for (int i = 0; i < shipPlaces.size(); i++)
	{
		const auto& s = shipPlaces[i];
		DrawTexturePro(
			assetManager.ships,
			s.srcRec,
			s.dstRec,
			{ 0,0 },
			0.0f,
			WHITE
		);

		if (s.isHovered)
		{
			DrawRectangle(s.dstRec.x - 4, s.dstRec.y - 4, 4, s.dstRec.height + 8, RED);
			DrawRectangle(s.dstRec.x, s.dstRec.y - 4, s.dstRec.width, 4, RED);
			DrawRectangle(s.dstRec.x + s.dstRec.width, s.dstRec.y - 4, 4, s.dstRec.height + 8, RED);
			DrawRectangle(s.dstRec.x - 4, s.dstRec.y + s.dstRec.height, s.dstRec.width + 8, 4, RED);
		}
	}
	if (shipMask)
	{
		Rectangle source = shipMask->getShipMaskSource(shipMask->isValid(board));
		Rectangle dest;
		dest.x = getMouse().x;
		dest.y = getMouse().y;
		dest.width = shipMask->size * 32;
		dest.height = 32;
		float rotation = 0.0f;
		if (shipMask->alignment == Ship::Alignment::VERTICAL)
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
			tint
		);

	}
}

void Graphics::drawCurrentTargetText(Vector2 position)
{
	std::string target = "Now targetting: ";
	if (position.x < 0 || position.x >= 10 || position.y < 0 || position.y >= 10)
	{
		target += "NONE";
	}
	else
	{
		target += 'A' + position.x;
		target += std::to_string((int)position.y + 1);
	}
	int x = (bottomRec.x + bottomRec.width) / 30 * 17;
	int y = bottomRec.y + 22;

	DrawText(target.c_str(), x, y, 32, WHITE);


}

void Graphics::drawGameUI(AssetManager& assetManager, Vector2 position, const std::vector<Ship>& remainingAiShips)
{
	int x = bottomRec.x + 20;
	int y = bottomRec.y + 20;
	DrawText("Remaining targets: ", x, y, 32, WHITE);
	for (int i = 0; i < remainingAiShips.size(); i++)
	{
		if (remainingAiShips[i].state != Ship::SUNK)
		{
			auto toDraw = aiShips[i];
			DrawTexturePro(
				assetManager.ships,
				toDraw.srcRec,
				toDraw.dstRec,
				{ 0.0f, 0.0f },
				0.0f,
				WHITE
			);
		}
	}

	drawCurrentTargetText(position);
}

bool Graphics::drawQuitButton(Vector2 mouse)
{
	Color c{ 132, 132, 132, 255 };
	DrawRectangle(quitButton.x, quitButton.y, quitButton.width, quitButton.height, c);
	DrawText("Quit", quitButton.x + 5, quitButton.y + 5, 32, WHITE);

	if (CheckCollisionPointRec(mouse, quitButton))
	{
		DrawRectangle(quitButton.x - 4, quitButton.y - 4, 4, quitButton.height + 8, RED);
		DrawRectangle(quitButton.x, quitButton.y - 4, quitButton.width, 4, RED);
		DrawRectangle(quitButton.x + quitButton.width, quitButton.y - 4, 4, quitButton.height + 8, RED);
		DrawRectangle(quitButton.x - 4, quitButton.y + quitButton.height, quitButton.width + 8, 4, RED);
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			return true;
		}
	}
	return false;
}

Rectangle* Graphics::drawGameOver(bool humanWon)
{
	std::string gameOver = "Game Over!";
	std::string winner = humanWon ? "Player won!" : "AI won!";

	int gameOverTextSize = MeasureText(gameOver.c_str(), 48);
	int gameOverTextX = gameOverScreen.x + (gameOverScreen.width - gameOverTextSize) / 2;
	int gameOverTextY = floor(gameOverScreen.y + 48 * 1.35f);
	int winnerTextSize = MeasureText(winner.c_str(), 36);
	int winnerTextX = gameOverScreen.x + (gameOverScreen.width - winnerTextSize) / 2;
	int winnerTextY = floor(gameOverTextY + 36 * 1.25f);

	DrawRectangle(gameOverScreen.x, gameOverScreen.y, gameOverScreen.width, gameOverScreen.height, { 40, 40, 40, 100 });
	DrawText(gameOver.c_str(), gameOverTextX, gameOverTextY, 48, WHITE);
	DrawText(winner.c_str(), winnerTextX, winnerTextY, 36, WHITE);
	DrawRectangle(gameOverQuit.x, gameOverQuit.y, gameOverQuit.width, gameOverQuit.height, { 135, 135, 135, 180 });
	DrawRectangle(gameOverRestart.x, gameOverRestart.y, gameOverRestart.width, gameOverRestart.height, { 135, 135, 135, 180 });
	int gameOverQuitButtonX = gameOverQuit.x + (gameOverQuit.width - quitTextSize) / 2;
	int gameOverQuitButtonY = gameOverQuit.y + (gameOverQuit.height - 32) / 2;
	int gameOverRestartButtonX = gameOverRestart.x + (gameOverRestart.width - restartTextSize) / 2;
	int gameOverRestartButtonY = gameOverRestart.y + (gameOverRestart.height - 32) / 2;
	DrawText(quitText.c_str(), gameOverQuitButtonX, gameOverQuitButtonY, 32, WHITE);
	DrawText(restartText.c_str(), gameOverRestartButtonX, gameOverRestartButtonY, 32, WHITE);

	if (CheckCollisionPointRec(getMouse(), gameOverQuit))
	{
		DrawRectangle(gameOverQuit.x - 4, gameOverQuit.y - 4, 4, gameOverQuit.height + 8, RED);
		DrawRectangle(gameOverQuit.x, gameOverQuit.y - 4, gameOverQuit.width, 4, RED);
		DrawRectangle(gameOverQuit.x + gameOverQuit.width, gameOverQuit.y - 4, 4, gameOverQuit.height + 8, RED);
		DrawRectangle(gameOverQuit.x - 4, gameOverQuit.y + gameOverQuit.height, gameOverQuit.width + 8, 4, RED);
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			return &gameOverQuit;
		}
	}
	if (CheckCollisionPointRec(getMouse(), gameOverRestart))
	{
		DrawRectangle(gameOverRestart.x - 4, gameOverRestart.y - 4, 4, gameOverRestart.height + 8, RED);
		DrawRectangle(gameOverRestart.x, gameOverRestart.y - 4, gameOverRestart.width, 4, RED);
		DrawRectangle(gameOverRestart.x + gameOverRestart.width, gameOverRestart.y - 4, 4, gameOverRestart.height + 8, RED);
		DrawRectangle(gameOverRestart.x - 4, gameOverRestart.y + gameOverRestart.height, gameOverRestart.width + 8, 4, RED);
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			return &gameOverRestart;
		}
	}


	return nullptr;

}

