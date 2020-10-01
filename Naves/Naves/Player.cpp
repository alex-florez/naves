#include "Player.h"

Player::Player(float x, float y, Game* game) {
	audioImpact = new Audio("res/efecto_explosion.wav", false);
	// Creamos la nave predeterminada
	spaceship = new Spaceship("res/jugador_nave.png", 5, 30, 3, x, y,
		50, 57, game);
}

void Player::update() {
	spaceship->update();
}

void Player::moveX(float axis) {
	spaceship->moveX(axis);
}

void Player::moveY(float axis) {
	spaceship->moveY(axis);
}

Projectile* Player::shoot() {
	return spaceship->shoot();
}

bool Player::impact() {
	audioImpact->play(); // Efecto de sonido.
	lifes--;
	return lifes <= 0;
}

void Player::addAmmo(int extraAmmo) {
	this->spaceship->ammo += extraAmmo;
}

int Player::getCurrentAmmo() {
	return this->spaceship->ammo;
}