#include <graphics.h>
#include <assetManager.h>
#include <ship.h>

bool Graphics::init()
{
	camera.target = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f };  // world-space center of view
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;
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
		Rectangle source = ship.getShipSource(true);
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

void Graphics::drawAiBoard(AssetManager& assetManager, Board& aiBoard, Board::SquareState* humanShots)
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

