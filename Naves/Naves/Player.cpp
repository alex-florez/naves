#include "Player.h"

Player::Player(float x, float y, Game* game) {
	audioImpact = new Audio("res/efecto_explosion.wav", false); // Audio al recibir un impacto.

	// Creamos las naves
	availableSpaceships["nave1"] = new Spaceship("res/nave1.png", 5, 30, 5, x, y,
		50, 57, game);
	availableSpaceships["nave2"] = new Spaceship("res/nave2.png", 7, 10, 4, x, y,
		36, 36, game);
	// Establecemos por defecto la nave 1
	spaceship = availableSpaceships["nave1"];
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

void Player::addAmmo(int extraAmmo) {
	this->spaceship->ammo += extraAmmo;
}

bool Player::impact() {
	audioImpact->play(); // Efecto de sonido.
	lifes--;
	return lifes <= 0;
}

int Player::getCurrentAmmo() {
	return this->spaceship->ammo;
}

/*
	Recibe el nombre de una nave, y si existe en el mapa
	de naves, la establece como nave actual del jugador,
	con las mismas coordenadas en las que estaba la nave anterior.
	Además restauramos el valor de su shootTime a 0 para tener en cuenta
	la cadencia.
*/
void Player::changeSpaceship(string name) {
	// Coordenadas de la nave anterior
	int x = spaceship->x;
	int y = spaceship->y;
	if (availableSpaceships.find(name) != availableSpaceships.end()) {
		this->spaceship = availableSpaceships[name];
		this->spaceship->x = x;
		this->spaceship->y = y;
		this->spaceship->shootTime = 0; // Restablecemos el shootTime a 0
	}
}
