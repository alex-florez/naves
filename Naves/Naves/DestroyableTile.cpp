#include "DestroyableTile.h"


DestroyableTile::DestroyableTile(string filename, int destroyTime, float x, float y, Game* game)
	: Tile(filename, x, y, game) {

	this->destroyTime = destroyTime;
}


void DestroyableTile::update() {
	if (playerIsOver) {
		timeSincePlayerIsOver++;
	}
	else {
		timeSincePlayerIsOver = 0;
	}
}

bool DestroyableTile::willDestroy() {
	return timeSincePlayerIsOver >= destroyTime;
}