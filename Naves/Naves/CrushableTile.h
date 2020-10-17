#pragma once

#include "Tile.h"

class CrushableTile : public Tile
{
public:

	CrushableTile(string filename, int destroyTime, float x, float y, Game* game);
	void update() override;
	bool willDestroy() override;
	bool isDestroyable() override;

	int timeSincePlayerIsOver = 0; // ticks que han pasado desde que el jugador está encima del tile
	int destroyTime; // ticks que tarda en destruirse el tile
};

