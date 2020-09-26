#pragma once

#include "Actor.h"
#include "Animation.h"
#include "EnemyProjectile.h"

#define ENEMY_SPEED 2

class Enemy : public Actor
{
public:
	Enemy(float x, float y, Game* game);
	void update();
	void draw() override;
	EnemyProjectile* autoShoot();

	Animation* aMoving;
	Animation* animation; // Referencia a la animaci�n actual.

	// Cadencia de disparo del enemigo
	int shootCadence = 80;
	int shootTime = shootCadence / 2;
};

