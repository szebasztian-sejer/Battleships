#include <shipPlace.h>

ShipPlace::ShipPlace(int size, bool active)
{
	this->size = size;
	this->active = active;

	float x = 0;
	float y = 0;
	float w = 0;
	float h = 32;

	switch (size)
		{
		case 2:
		{
			x = 0;
			w = 64;
			break;
		}
		case 3:
		{
			x = 64;
			w = 96;
			break;
		}
		case 4:
		{
			x = 160;
			w = 128;
			break;
		}
		case 5:
		{
			x = 288;
			w = 160;
			break;
		}

	}
	
	srcRec = { x,y,w,h };
}