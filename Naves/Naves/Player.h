#pragma once

#include "Actor.h"

#define PLAYER_SPEED 3

class Player : public Actor
{
public:
	Player(float x, float y, Game* game);

	void update();
	void moveX(float axis);
	void moveY(float axis);
};

