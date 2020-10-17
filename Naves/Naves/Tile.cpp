#include "Tile.h"


Tile::Tile(string filename, float x, float y, Game* game)
	: Actor(filename, x, y, 40, 32, game) {

}

void Tile::update() {

}

bool Tile::willDestroy() {
	return false;
}

bool Tile::isDestroyable() {
	return false;
}