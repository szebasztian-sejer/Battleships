#pragma once
#include <human.h>
#include <raylib.h>
#include <ai.h>
#include <difficulty.h>
#include <graphics.h>

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

	Graphics graphics;

	bool init(Difficulty d);
	bool update(AssetManager& assetManager);

	bool checkHit(Board& board, Vector2 target);
	void updateShips(Player* player);
	Vector2 getSelectPosition(Board& board);
};