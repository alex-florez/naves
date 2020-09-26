#pragma once

#include "Actor.h"

#define ENEMY_PROJECTILE_SPEED 4

class EnemyProjectile : public Actor
{
public:
	EnemyProjectile(float x, float y, Game* game);
	void update();
};

