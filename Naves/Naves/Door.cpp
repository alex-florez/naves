#include "Door.h"


Door::Door(char id, float x, float y, Game* game)
	: Actor("res/door.png", x, y, 35, 50, game) {
	this->id = id;
}

void Door::teleport(Player* player) {
	int offset = 0; // Offset para no quedar atrapados en el teletransporte.
	// Comprobar la orientación
	if (player->orientation == game->orientationRight) {
		offset = 35 + player->width / 2;
	}
	if (player->orientation == game->orientationLeft) {
		offset = -(35 + player->width / 2);
	}

	player->x = this->destinyX + offset;
	player->y = this->destinyY;
}