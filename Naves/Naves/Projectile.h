#pragma once

#include "Actor.h"


#define PROJECTILE_SPEED 9

class Projectile : public Actor
{
public:
	Projectile(float x, float y, Game* game);
	void update();
};

