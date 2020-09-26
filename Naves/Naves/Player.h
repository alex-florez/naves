#pragma once

#include "Actor.h"
#include "Projectile.h"
#include "Audio.h"

#define PLAYER_SPEED 5
#define PLAYER_LIFES 3

class Player : public Actor
{
public:
	Player(float x, float y, Game* game);

	void update();
	void moveX(float axis);
	void moveY(float axis);
	Projectile* shoot();
	bool impact();

	int shootCadence = 30;
	int shootTime = 0;

	int lifes = PLAYER_LIFES;

	// Audio
	Audio* audioShoot;
	Audio* audioImpact;
};

