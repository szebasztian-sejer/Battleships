#include <soundEngine.h>
#include <raylib.h>
#include <assetManager.h>

void SoundEngine::playHitSound(AssetManager& assetManager)
{
	PlaySound(assetManager.hitFx);
}
void SoundEngine::playMissSound(AssetManager& assetManager)
{
	PlaySound(assetManager.missFx);
}
void SoundEngine::playSinkSound(AssetManager& assetManager)
{
	PlaySound(assetManager.sinkFx);
}
void SoundEngine::playVictorySound(AssetManager& assetManager)
{
	PlaySound(assetManager.victoryFx);
}
void SoundEngine::playDefeatSound(AssetManager& assetManager)
{
	PlaySound(assetManager.defeatFx);
}