#pragma once

struct AssetManager;

struct SoundEngine
{
	void playHitSound(AssetManager& assetManager);
	void playMissSound(AssetManager& assetManager);
	void playSinkSound(AssetManager& assetManager);
	void playVictorySound(AssetManager& assetManager);
	void playDefeatSound(AssetManager& assetManager);
};