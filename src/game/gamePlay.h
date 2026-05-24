#pragma once
#include <human.h>
#include <raylib.h>
#include <ai.h>
#include <difficulty.h>

struct AssetManager;

struct GamePlay
{
	enum State
	{
		PLACEMENT = 0,
		HUMAN_TURN,
		AI_TURN,
		HUMAN_WIN,
		AI_WIN
	};

	State state;
	Difficulty difficulty;
	Human human;
	AI ai;
	Camera2D camera = {};

	bool init(Difficulty d);
	bool update(AssetManager& assetManager);

	void drawHumanBoard(AssetManager& assetManager);
	void drawAiBoard(AssetManager& assetManager);

	void drawGrid(float boardX, float boardY);
};