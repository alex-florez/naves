#pragma once

#include "Actor.h"
#include "Animation.h"

#define ENEMY_SPEED 2

class Enemy : public Actor
{
public:
	Enemy(float x, float y, Game* game);
	void update();
	void draw() override;

	Animation* aMoving;
	Animation* animation; // Referencia a la animación actual.
};

