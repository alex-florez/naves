#pragma once

#include "Actor.h"
#include "Animation.h"
#include "EnemyProjectile.h"

#define ENEMY_SPEED 2
#define MIN_SHOOT_CADENCE 75
#define MAX_SHOOT_CADENCE 100

class Enemy : public Actor
{
public:
	Enemy(float x, float y, Game* game);
	void update();
	void draw() override;
	EnemyProjectile* autoShoot();

	Animation* aMoving;
	Animation* animation; // Referencia a la animación actual.

	// Cadencia de disparo del enemigo
	int shootCadence;
	int shootTime = shootCadence / 2;
};

