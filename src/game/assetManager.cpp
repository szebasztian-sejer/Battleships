#include <assetManager.h>

void AssetManager::loadAll()
{
	board = LoadTexture(RESOURCES_PATH "board.png");
	hit = LoadTexture(RESOURCES_PATH "hit.png");
	ships = LoadTexture(RESOURCES_PATH "ships.png");
	select = LoadTexture(RESOURCES_PATH "select.png");
}