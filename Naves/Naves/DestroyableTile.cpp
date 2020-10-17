#include "DestroyableTile.h"

DestroyableTile::DestroyableTile(string filename, float x, float y, Game* game)
	: Tile(filename, x, y, game) {

}

void DestroyableTile::update() {

}

bool DestroyableTile::isDestroyable() {
	return true;
}

bool DestroyableTile::willDestroy() {
	return false;
}
