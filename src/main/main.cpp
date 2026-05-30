#include <raylib.h>
#include <gameMain.h>

int main(void)
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1024, 860, "Battleships");

	InitAudioDevice();
	SetExitKey(KEY_NULL);
	SetTargetFPS(240);



	if (!initGame())
	{
		return 0;
	}

	bool running = true;

	while (running && !WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);

		running = updateGame();

		EndDrawing();
	}

	CloseWindow();
	CloseAudioDevice();
	closeGame();


	return 0;
}