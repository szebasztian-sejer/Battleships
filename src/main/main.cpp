#include <raylib.h>
#include <gameMain.h>

int main(void)
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1024, 860, "Battleships");
	SetExitKey(KEY_NULL);
	SetTargetFPS(240);



	if (!initGame())
	{
		return 0;
	}

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);

		if (!updateGame())
		{
			CloseWindow();
		}

		EndDrawing();
	}

	CloseWindow();
	closeGame();


	return 0;
}