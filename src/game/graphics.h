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
	std::vector<ShipPlace> aiShips;
	Rectangle quitButton = {};

	Rectangle gameOverScreen = {};
	Rectangle gameOverQuit = {};
	Rectangle gameOverRestart = {};

	std::string quitText = "Quit";
	std::string restartText = "Restart";

	int quitTextSize = MeasureText(quitText.c_str(), 24);
	int restartTextSize = MeasureText(restartText.c_str(), 24);

	bool init();

	void drawHumanBoard(AssetManager& assetManager, Board& humanBoard, std::vector<Ship>& ships, Board::SquareState* aiShots);
	void drawAiBoard(AssetManager& assetManager, Board& aiBoard, std::vector<Ship>& ships, Board::SquareState* humanShots);

	void drawGrid(float boardX, float boardY);
	void drawCoords(float boardX, float boardY);

	void drawBottomRec();
	void drawPlacementsUi(AssetManager& assetManager, ShipMask* shipMask, Board& board);
	void drawCurrentTargetText(Vector2 position);

	void drawGameUI(AssetManager& assetManager, Vector2 position, const std::vector<Ship>& remainingAiShips);
	bool drawQuitButton(Vector2 mouse);

	Rectangle* drawGameOver(bool humanWon);
	

	Vector2 getMouse();

	bool update();
};