#pragma once

#include "Actor.h"
#include "Animation.h"

#define ENEMY_SPEED 2

class Enemy : public Actor
{
public:
	Enemy(float x, float y, Game* game);
	void update();
	void draw(float scrollX = 0, float scrollY = 0) override;
	void impacted(); // Enemigo recibe un impacto

	Animation* aMoving;
	Animation* aDying;
	Animation* animation; // Referencia a la animación actual.

	int state;
	float vxIntelligence;
};

