#pragma once

#include "Actor.h"

class Tile : public Actor
{
public:
	Tile(string filename, bool breakable, float x, float y, Game* game);
};

