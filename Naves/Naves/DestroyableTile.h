#pragma once

#include "Tile.h"

class DestroyableTile : public Tile
{
public:
	DestroyableTile(string filename, float x, float y, Game* game);

	void update() override;
	bool willDestroy() override;
	bool isDestroyable() override;

};

