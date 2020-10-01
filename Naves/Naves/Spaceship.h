#pragma once

#include "Actor.h"
#include "Audio.h"
#include "Projectile.h"

class Spaceship : public Actor
{
public:
	Spaceship(string filename, int speed, int cadence, int initialAmmo, 
		float x, float y, int width, int height, Game* game);
	void update();
	void moveX(float axis);
	void moveY(float axis);
	Projectile* shoot();


	int shootTime = 0;
	int shootCadence;
	int speed;
	int ammo; // Disparos/munición

	Audio* audioShoot;
};

