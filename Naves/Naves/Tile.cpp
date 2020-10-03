#include "Tile.h"


Tile::Tile(string filename, bool breakable, float x, float y, Game* game)
	: Actor(filename, x, y, 40, 32, game) {
	
	this->breakable = breakable;
}