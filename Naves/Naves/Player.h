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
	void draw(float scrollX = 0) override;

	int shootCadence = 30;
	int shootTime = 0;

	// Audio
	Audio* audioShoot;

	// Animaciones
	Animation* aIdleRight;
	Animation* aIdleLeft;
	Animation* aRunningRight;
	Animation* aRunningLeft;

	Animation* aShootingLeft;
	Animation* aShootingRight;

	Animation* animation;

	// Estado y orientación
	int state;
	int orientation;
};


