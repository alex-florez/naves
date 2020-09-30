#pragma once

#include "Actor.h"

class AmmoCollectable : public Actor
{
public:
	AmmoCollectable(int ammoValue, float x, float y, Game* game);

	int ammoValue; // Valor del recolectable
};

