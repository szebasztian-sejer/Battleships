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
}

void GamePlay::drawAiBoard(AssetManager& assetManager)
{
	for (auto& ship : ai.ships)
	{
		Rectangle source = ship.getShipSource(true);
		Rectangle dest;
		dest.x = ai.board.drawRec.x + 32 * ship.position.x;
		dest.y = ai.board.drawRec.y + 32 * ship.position.y;
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

	EndMode2D();

	return true;
}