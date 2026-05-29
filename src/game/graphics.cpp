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
		s.dstRec.y = bottomRec.y + 64;
		s.dstRec.width = ships[i] * 32;
		s.dstRec.height = 32;
		shipPlaces.push_back(s);
		s.active = false;
		aiShips.push_back(s);

	}
	quitButton = { bottomRec.x + bottomRec.width - 100, bottomRec.y+bottomRec.height - 64, 80, 50 };
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
			WHITE
		);

	}
	else
	{
		//std::cout << "ShipMask destroyed\n";
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

