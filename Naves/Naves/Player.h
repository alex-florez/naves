#pragma once

#include "Projectile.h"
#include "Audio.h"
#include "Spaceship.h"

#define PLAYER_LIFES 3

class Player
{
public:
	Player(float x, float y, Game* game);

	void update();
	void moveX(float axis);
	void moveY(float axis);
	Projectile* shoot();
	bool impact();
	void addAmmo(int ammo);
	int getCurrentAmmo();


	// Vidas del jugador
	int lifes = PLAYER_LIFES;
	

	// Audio
	Audio* audioImpact;

	// Nave actual del jugador
	Spaceship* spaceship;
};

