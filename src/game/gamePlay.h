#pragma once
#include <human.h>
#include <raylib.h>
#include <ai.h>
#include <difficulty.h>
#include <graphics.h>
#include <soundEngine.h>
#include <shipMask.h>
#include <memory>
#include <unordered_set>
#include <ship.h>

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

	int selectedShip;
	std::unordered_set<Ship> alreadySunk;
	std::unique_ptr<ShipMask> shipMask;

	Graphics graphics;
	SoundEngine soundEngine;

	bool init(Difficulty d);
	bool update(AssetManager& assetManager);

	bool checkHit(Board& board, Vector2 target);
	bool updateShips(Player* player, Board* oppBoard = nullptr);
	Vector2 getSelectPosition(Board& board);
};