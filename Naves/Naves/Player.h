#pragma once

#include "Actor.h"
#include "Projectile.h"
#include "Audio.h"
#include "Animation.h"

#define PLAYER_SPEED 3

class Player : public Actor
{
public:
	Player(float x, float y, Game* game);

	void update();
	void moveX(float axis);
	void moveY(float axis);
	Projectile* shoot();
	void draw(float scrollX = 0, float scrollY = 0) override;
	void jump(); // Saltar
	void loseLife();
	bool isOver(Actor* actor);

	bool onAir; // indica si el jugador está en el aire.

	int shootCadence = 30;
	int shootTime = 0;

	// Audio
	Audio* audioShoot;
	Audio* audioJumpOverEnemy;

	// Animaciones
	Animation* aIdleRight;
	Animation* aIdleLeft;
	Animation* aRunningRight;
	Animation* aRunningLeft;

	Animation* aShootingLeft;
	Animation* aShootingRight;

	// Salto
	Animation* aJumpingRight;
	Animation* aJumpingLeft;

	Animation* animation;

	// Estado y orientación
	int state;
	int orientation;

	// Vidas y tiempo de invulnerabilidad
	int lifes = 3;
	int invulnerableTime = 0;
};


