#pragma once

#include "Actor.h"

class Tile : public Actor
{
public:
	Tile(string filename, float x, float y, Game* game);

	virtual void update();
	virtual bool willDestroy();
	virtual bool isDestroyable();
	
	bool playerIsOver = false; // Indica si el jugador está o no encima del tile.
};

