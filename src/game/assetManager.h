#pragma once
#include <raylib.h>

struct AssetManager
{
	Texture2D board = {};
	Texture2D hit = {};
	Texture2D ships = {};
	Texture2D select = {};
	void loadAll();
};