#pragma once

#include "Actor.h"

#define ENEMY_SPEED 2

class Enemy : public Actor
{
public:
	Enemy(float x, float y, Game* game);
	void update();
};

