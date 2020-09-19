#include "Player.h"

Player::Player(float x, float y, Game* game)
	: Actor("res/jugador.png", x, y, 50, 57, game) {

}

void Player::update() {
	x = x + vx;
	y = y + vy;
}

void Player::moveX(float axis) {
	vx = axis * PLAYER_SPEED;
}

void Player::moveY(float axis) {
	vy = axis * PLAYER_SPEED;
}