#include "Enemy.h"

Enemy::Enemy(float x, float y, Game* game)
	: Actor("res/enemigo.png", x, y, 36, 40, game) {

	vx = -1 * ENEMY_SPEED;

	aMoving = new Animation("res/enemigo_movimiento.png", width, height,
								108, 40, 6, 3, true, game);
	animation = aMoving;
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