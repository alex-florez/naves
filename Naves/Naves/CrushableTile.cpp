#include "CrushableTile.h"


CrushableTile::CrushableTile(string filename, int destroyTime, float x, float y, Game* game)
	: Tile(filename, x, y, game) {

	this->destroyTime = destroyTime;
}


void CrushableTile::update() {
	if (playerIsOver) {
		timeSincePlayerIsOver++;
	}
	else {
		timeSincePlayerIsOver = 0;
	}
}

bool CrushableTile::willDestroy() {
	return timeSincePlayerIsOver >= destroyTime;
}

bool CrushableTile::isDestroyable() {
	return false;
}