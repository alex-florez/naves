#include "Player.h"

Player::Player(float x, float y, Game* game)
	: Actor("res/jugador.png", x, y, 50, 57, game) {

	audioShoot = new Audio("res/efecto_disparo.wav", false);

	aIdleRight = new Animation("res/jugador_idle_derecha.png", width, height, 320, 40, 6, 8, game);
	aIdleLeft = new Animation("res/jugador_idle_izquierda.png", width, height, 320, 40, 6, 8, game);
	aRunningRight = new Animation("res/jugador_corriendo_derecha.png", width, height, 320, 40, 6, 8, game);
	aRunningLeft = new Animation("res/jugador_corriendo_izquierda.png", width, height, 320, 40, 6, 8, game);

	animation = aIdleRight;

	state = game->stateMoving;
	orientation = game->orientationRight;
}

void Player::update() {
	animation->update();

	if (vx > 0) {
		orientation = game->orientationRight;
	} 
	if (vx < 0) {
		orientation = game->orientationLeft;
	}

	if (vx != 0) { // Jugador corriendo
		if (orientation == game->orientationRight) {
			animation = aRunningRight;
		}

		if (orientation == game->orientationLeft) {
			animation = aRunningLeft;
		}
	}

	if (vx == 0) { // Jugador parado
		if (orientation == game->orientationRight) {
			animation = aIdleRight;
		}

		if (orientation == game->orientationLeft) {
			animation = aIdleLeft;
		}
	}

	x = x + vx;
	y = y + vy;
	// Cadencia de disparo
	if (shootTime > 0) {
		shootTime--;
	}
}

void Player::moveX(float axis) {
	vx = axis * PLAYER_SPEED;
}

void Player::moveY(float axis) {
	vy = axis * PLAYER_SPEED;
}

Projectile* Player::shoot() {
	if (shootTime == 0) {
		audioShoot->play(); // Efecto de sonido del disparo.
		shootTime = shootCadence;
		return new Projectile(x + width/2, y, game);
	}
	return NULL;
}

void Player::draw() {
	animation->draw(x, y);
}