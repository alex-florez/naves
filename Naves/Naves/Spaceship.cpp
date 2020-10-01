#include "Spaceship.h"

Spaceship::Spaceship(string filename, int speed, int shootCadence,
	int initialAmmo, float x, float y, int width, int height, Game* game)
	: Actor(filename, x, y, width, height, game) {

	this->speed = speed;
	this->ammo = initialAmmo;
	this->shootCadence = shootCadence;
	this->audioShoot = new Audio("res/efecto_disparo.wav", false);
}

void Spaceship::update() {
	x = x + vx;
	y = y + vy;
	// Cadencia de disparo
	if (shootTime > 0) {
		shootTime--;
	}
}

void Spaceship::moveX(float axis) {
	vx = this->speed * axis;
}

void Spaceship::moveY(float axis) {
	vy = this->speed * axis;
}

Projectile* Spaceship::shoot() {
	if (shootTime == 0 && ammo > 0) { // Comprobar munición y cadencia...
		audioShoot->play(); // Efecto de sonido del disparo.
		shootTime = shootCadence;
		ammo--; // Decrementamos el número de disparos restantes
		return new Projectile(x + width/2, y, game);
	}

	return nullptr;
}