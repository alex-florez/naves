#include "Enemy.h"

Enemy::Enemy(float x, float y, Game* game)
	: Actor("res/enemigo.png", x, y, 36, 40, game) {

	vx = -1 * ENEMY_SPEED;

	aMoving = new Animation("res/enemigo_movimiento.png", width, height,
								108, 40, 6, 3, true, game);
	aDying = new Animation("res/enemigo_morir.png", width, height, 280, 40, 6, 8, false, game);
	animation = aMoving;

	state = game->stateMoving;
}



void Enemy::update() {
	// Actualizar la animación
	bool endAnimation = animation->update();
	
	if (endAnimation) { // Terminó la animación
		
		if (state == game->stateDying) { // Estaba muriendo
			state = game->stateDead;
		}
	}

	if (state == game->stateMoving) { // Moviendo
		animation = aMoving;
	}
	if (state == game->stateDying) { // Muriendo
		animation = aDying;
	}

	if (state != game->stateDying) { // Si no se está muriendo...
		if (x - (width / 2) <= 0) {
			vx = vx * -1;
		}
	}

}

void Enemy::draw(float scrollX) {
	animation->draw(x - scrollX,y);
}

void Enemy::impacted() {
	if (state != game->stateDying) {
		state = game->stateDying;
	}
}