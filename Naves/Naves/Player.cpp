#include "Player.h"

Player::Player(float x, float y, Game* game)
	: Actor("res/jugador_nave.png", x, y, 50, 57, game) {

	audioShoot = new Audio("res/efecto_disparo.wav", false);
	audioImpact = new Audio("res/efecto_explosion.wav", false);
}

void Player::update() {
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
	if (shootTime == 0 && ammo > 0) {
		audioShoot->play(); // Efecto de sonido del disparo.
		shootTime = shootCadence;
		ammo--; // Decrementamos el número de disparos restantes
		return new Projectile(x + width/2, y, game);
	}
	return NULL;
}

bool Player::impact() {
	audioImpact->play(); // Efecto de sonido.
	lifes--;
	return lifes <= 0;
}

void Player::addAmmo(int extraAmmo) {
	this->ammo += extraAmmo;
}