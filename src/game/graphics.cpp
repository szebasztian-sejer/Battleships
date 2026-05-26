#include <graphics.h>
#include <assetManager.h>
#include <ship.h>
#include <shipPlace.h>

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
		remainingHumanShips.push_back(s);
		remainingAiShips.push_back(s);

	}
	quitButton = { bottomRec.x + bottomRec.width - 100, bottomRec.y+bottomRec.height - 64, 80, 50 };
	currentTarget = { bottomRec.x + bottomRec.width - 200, bottomRec.y + 20, 64, 50 };
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
	for (int i = 0; i < humanBoard.w * humanBoard.h; i++)
	{
		if (aiShots[i] == Board::SquareState::EMPTY) { continue; }
		Rectangle dest;
		dest.x = humanBoard.drawRec.x + 32 * (i % humanBoard.w);
		dest.y = humanBoard.drawRec.y + 32 * (i / humanBoard.w);
		dest.width = 32;
		dest.height = 32;
		Texture2D tex = aiShots[i] == Board::SquareState::HIT ? assetManager.hit : assetManager.miss;
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
	Color c = { 155,155,155, 255 };

	DrawRectangle(bottomRec.x, bottomRec.y, bottomRec.width, bottomRec.height, c);

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
			DrawRectangle(s.dstRec.x - 4, s.dstRec.y - 4, 4, s.dstRec.height + 8, BLACK);
			DrawRectangle(s.dstRec.x, s.dstRec.y - 4, s.dstRec.width, 4, BLACK);
			DrawRectangle(s.dstRec.x + s.dstRec.width, s.dstRec.y - 4, 4, s.dstRec.height + 8, BLACK);
			DrawRectangle(s.dstRec.x - 4, s.dstRec.y + s.dstRec.height, s.dstRec.width + 8, 4, BLACK);
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
void drawGameUI()
{}

