#pragma once
#include <raylib.h>
#include <board.h>
#include <ui.h>
#include <shipPlace.h>
#include <shipMask.h>
#include <memory>

struct Ship;
struct AssetManager;

struct Graphics
{
	Camera2D camera = {};

	Rectangle bottomRec = {};
	std::vector<ShipPlace> shipPlaces;
	std::vector<ShipPlace> remainingHumanShips;
	std::vector<ShipPlace> remainingAiShips;
	Rectangle quitButton = {};
	Rectangle currentTarget = {};

	void drawHumanBoard(AssetManager& assetManager, Board& humanBoard, std::vector<Ship>& ships, Board::SquareState* aiShots);
	void drawAiBoard(AssetManager& assetManager, Board& aiBoard, std::vector<Ship>& ships, Board::SquareState* humanShots);

	void drawGrid(float boardX, float boardY);

	void drawBottomRec();
	void drawPlacementsUi(AssetManager& assetManager, ShipMask* shipMask, Board& board);
	void drawGameUI();

	bool init();

	Vector2 getMouse();

	bool update();
};