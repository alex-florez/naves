#include "AmmoCollectable.h"

AmmoCollectable::AmmoCollectable(int ammoValue, float x, float y, Game* game)
	: Actor("res/ammo.png", x, y, 35, 36, game) {

	this->ammoValue = ammoValue;
}
