#pragma once
#include <raylib.h>
#include <board.h>
#include <ui.h>

struct Ship;
struct AssetManager;

struct Graphics
{
	Camera2D camera = {};
	void drawHumanBoard(AssetManager& assetManager, Board& Humanboard, std::vector<Ship>& ships, Board::SquareState* aiShots);
	void drawAiBoard(AssetManager& assetManager, Board& board, Board::SquareState* humanShots);

	void drawGrid(float boardX, float boardY);

	bool init();

	Vector2 getMouse();

	bool update();
};