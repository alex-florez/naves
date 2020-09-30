#pragma once

#include "Actor.h"
#include "Projectile.h"
#include "Audio.h"

#define PLAYER_SPEED 5
#define PLAYER_LIFES 3
#define PLAYER_INITIAL_AMMO 3

class Player : public Actor
{
public:
	Player(float x, float y, Game* game);

	void update();
	void moveX(float axis);
	void moveY(float axis);
	Projectile* shoot();
	bool impact();
	void addAmmo(int ammo);
	int shootCadence = 30;
	int shootTime = 0;

	// Vidas del jugador
	int lifes = PLAYER_LIFES;

	// Disparos/munición
	int ammo = PLAYER_INITIAL_AMMO;
	

	// Audio
	Audio* audioShoot;
	Audio* audioImpact;
};

