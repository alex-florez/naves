#include "Enemy.h"

Enemy::Enemy(float x, float y, Game* game)
	: Actor("res/enemigo.png", x, y, 36, 40, game) {

	vx = -1 * ENEMY_SPEED;

	aMoving = new Animation("res/enemigo_movimiento.png", width, height,
								108, 40, 6, 3, game);
	animation = aMoving;

	// Cadencia de disparo aleatoria
	shootCadence = (rand() % (MAX_SHOOT_CADENCE - MIN_SHOOT_CADENCE)) + 1 + MIN_SHOOT_CADENCE;
}



void Enemy::update() {
	// Actualizar la animación
	animation->update();

	x = x + vx;
	/*if (x - (width / 2) <= 0) {
		vx = vx * -1;
	}*/
}

void Enemy::draw() {
	animation->draw(x,y);
}

EnemyProjectile* Enemy::autoShoot() {
	shootTime--;

	if (shootTime <= 0) {
		shootTime = shootCadence;
		return new EnemyProjectile(x - width/2, y, game);
	}
	return nullptr;
}