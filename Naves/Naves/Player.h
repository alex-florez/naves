#pragma once

#include "Actor.h"
#include "Projectile.h"

#define PLAYER_SPEED 3

class Player : public Actor
{
public:
	Player(float x, float y, Game* game);

	void update();
	void moveX(float axis);
	void moveY(float axis);
	Projectile* shoot();

	int shootCadence = 30;
	int shootTime = 0;
};

