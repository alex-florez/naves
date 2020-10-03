#include "Projectile.h"


Projectile::Projectile(float x, float y, Game* game)
	: Actor("res/disparo_jugador2.png", x, y, 20, 20, game) {
	
	vx = 9;
	vy = -1;

	canDestroy = true; // El proyectil puede destruir elementos destruibles.
}

void Projectile::update() {
	vy = vy - 1; // La gravedad suma 1 en cada tick, para anularla restamos uno.
}