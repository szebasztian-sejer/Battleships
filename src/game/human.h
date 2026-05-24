#pragma once
#include <player.h>

struct Human : Player
{
	Human();
	void setBoard();
	Board aiBoard;
};