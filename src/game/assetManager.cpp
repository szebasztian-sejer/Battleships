#include <assetManager.h>

void AssetManager::loadAll()
{
	board = LoadTexture(RESOURCES_PATH "board.png");
	hit = LoadTexture(RESOURCES_PATH "hit.png");
	ships = LoadTexture(RESOURCES_PATH "ships.png");
	select = LoadTexture(RESOURCES_PATH "select.png");
	miss = LoadTexture(RESOURCES_PATH "miss.png");
	cursor = LoadTexture(RESOURCES_PATH "cursor.png");

	victoryFx = LoadSound(RESOURCES_PATH "victoryFx.ogg");
	defeatFx = LoadSound(RESOURCES_PATH "defeatFx.ogg");
	missFx = LoadSound(RESOURCES_PATH "missFx.ogg");
	hitFx = LoadSound(RESOURCES_PATH "hitFx.ogg");
	sinkFx = LoadSound(RESOURCES_PATH "sinkFx.ogg");

	SetSoundVolume(victoryFx, 1.0f);
	SetSoundVolume(defeatFx, 1.0f);
	SetSoundVolume(missFx, 0.5f);
	SetSoundVolume(hitFx, 0.4f);
	SetSoundVolume(sinkFx, 0.4f);
}