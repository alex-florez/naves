#include "Projectile.h"


Projectile::Projectile(float x, float y, Game* game)
	: Actor("res/disparo_jugador.png", x, y, 18, 6, game) {
	
	vx = PROJECTILE_SPEED;
}

void Projectile::update() {
	x = x + vx;
}