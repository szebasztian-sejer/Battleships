#include <gameMain.h>
#include <assetManager.h>
#include <human.h>
#include <ai.h>
#include <helpers.h>
#include <ship.h>
#include <raymath.h>
#include <ui.h>
#include <gamePlay.h>
#include <difficulty.h>
#include <raylib.h>

UIEngine mainMenuUI;
GamePlay gamePlay;
Difficulty difficulty;

AssetManager assetManager;
bool inGame = false;

bool initGame()
{
	HideCursor();
	assetManager.loadAll();
	inGame = false;
	return true;
}
bool updateGame()
{
	Vector2 mouse = GetMousePosition();
	ClearBackground({ 0, 0, 0, 255 });
	if (!inGame)
	{
		mainMenuUI.addTitle("Battleships");
		if (mainMenuUI.addButton("Start game"))
		{
			inGame = true;
			gamePlay = {};
			gamePlay.init(difficulty);
		}
		if (mainMenuUI.addButton("Change difficulty"))
		{
			switch (difficulty)
			{
			case Difficulty::EASY:
			{
				difficulty = Difficulty::MEDIUM;
				break;
			}
			case Difficulty::MEDIUM:
			{
				difficulty = Difficulty::HARD;
				break;
			}
			case Difficulty::HARD:
			{
				difficulty = Difficulty::EASY;
				break;
			}
			}
		}
		std::string difficultyText = "Difficulty: ";
		switch (difficulty)
		{
		case Difficulty::EASY:
		{
			difficultyText += "Easy";
			break;
		}
		case Difficulty::MEDIUM:
		{
			difficultyText += "Medium";
			break;
		}
		case Difficulty::HARD:
		{
			difficultyText += "Hard";
			break;
		}

		}
		mainMenuUI.addTitle(difficultyText);
		if (mainMenuUI.addButton("Exit"))
		{
			return false;
		}

		mainMenuUI.updateAndRender();
		DrawTexturePro(
			assetManager.cursor,
			{ 0,0,(float)assetManager.cursor.width, (float)assetManager.cursor.height },
			{ mouse.x, mouse.y, (float)assetManager.cursor.width, (float)assetManager.cursor.height },
			{ assetManager.cursor.width / 2.0f, assetManager.cursor.height / 2.0f },
			0.0f,
			WHITE);

		return true;
	}
	else
	{
		bool ret = gamePlay.update(assetManager);
		DrawTexturePro(
			assetManager.cursor,
			{ 0,0,(float)assetManager.cursor.width, (float)assetManager.cursor.height },
			{ mouse.x, mouse.y, (float)assetManager.cursor.width, (float)assetManager.cursor.height },
			{ assetManager.cursor.width / 2.0f, assetManager.cursor.height / 2.0f },
			0.0f,
			WHITE);
		return ret;
	}
}
bool closeGame()
{
	return true;
}
