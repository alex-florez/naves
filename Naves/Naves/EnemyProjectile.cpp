#include "EnemyProjectile.h"


EnemyProjectile::EnemyProjectile(float x, float y, Game* game)
	: Actor("res/disparo_enemigo.png", x, y, 20, 20, game) {
	vx = -ENEMY_PROJECTILE_SPEED;
}

void EnemyProjectile::update() {
	x = x + vx;
}
