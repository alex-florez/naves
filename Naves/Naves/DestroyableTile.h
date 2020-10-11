#pragma once

#include "Tile.h"

class DestroyableTile : public Tile
{
public:

	DestroyableTile(string filename, int destroyTime, float x, float y, Game* game);
	void update() override;
	bool willDestroy() override;

	int timeSincePlayerIsOver = 0; // ticks que han pasado desde que el jugador está encima del tile
	int destroyTime; // ticks que tarda en destruirse el tile
};

