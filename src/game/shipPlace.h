#pragma once
#include <raylib.h>

struct ShipPlace
{
	ShipPlace(int size, bool active);
	int size = 2;
	bool active = false;
	bool isHovered = false;
	Rectangle srcRec = {};
	Rectangle dstRec = {};
};