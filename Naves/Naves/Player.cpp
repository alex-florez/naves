#include "Player.h"

Player::Player(float x, float y, Game* game)
	: Actor("res/jugador.png", x, y, 35, 35, game) {

	audioShoot = new Audio("res/efecto_disparo.wav", false);

	// Animaciones del jugador
	aIdleRight = new Animation("res/jugador_idle_derecha.png", width, height, 320, 40, 6, 8, true, game);
	aIdleLeft = new Animation("res/jugador_idle_izquierda.png", width, height, 320, 40, 6, 8, true, game);
	aRunningRight = new Animation("res/jugador_corriendo_derecha.png", width, height, 320, 40, 6, 8, true, game);
	aRunningLeft = new Animation("res/jugador_corriendo_izquierda.png", width, height, 320, 40, 6, 8, true, game);

	aShootingLeft = new Animation("res/jugador_disparando_izquierda.png", width, height, 160, 40, 6, 4, false, game);
	aShootingRight = new Animation("res/jugador_disparando_derecha.png", width, height, 160, 40, 6, 4, false, game);

	animation = aIdleRight; // Animación actual

	state = game->stateMoving;
	orientation = game->orientationRight;
	onAir = false;
}

void Player::update() {
	bool endAnimation = animation->update();

	onAir = !collisionDown;
	
	// Si terminó la animación
	if (endAnimation) {
		// Si estaba disparando
		if (state == game->stateShooting) {
			state = game->stateMoving;
		}
	}


	// Establecemos la orientación
	if (vx > 0) {
		orientation = game->orientationRight;
	}
	if (vx < 0) {
		orientation = game->orientationLeft;
	}

	// Seleccionamos la animación basándonos en el estado.
	if (state == game->stateShooting) { // Jugador disparando
		if (orientation == game->orientationRight) {
			animation = aShootingRight;
		}
		if (orientation == game->orientationLeft) {
			animation = aShootingLeft;
		}
	}

	if (state == game->stateMoving) { // Estado en movimiento
		if (vx != 0) { // Jugador moviendose
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
	}
	
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

void Player::jump() {
	if (!onAir) {
		vy = -16;
		onAir = true;
	}
}

Projectile* Player::shoot() {
	if (shootTime == 0) {
		audioShoot->play(); // Efecto de sonido del disparo.
		shootTime = shootCadence;
		state = game->stateShooting;
		Projectile* projectile = new Projectile(x + width/2, y, game);
		
		if (orientation == game->orientationLeft)
			projectile->vx = projectile->vx * -1; // Invertir velocidad del proyectil
		
		return projectile;
	}
	return NULL;
}

void Player::draw(float scrollX) {
	animation->draw(x - scrollX, y);
}