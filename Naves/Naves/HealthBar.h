#pragma once

#include "Actor.h"
#include "Text.h"

#define INITIAL_LIFES 3;

class HealthBar : public Actor
{
public:
	HealthBar(float x, float y, Game* game);
	void update();
	void setLifes(int lifes);
	void draw() override;
private:
	int lifes;
	Text* textLifes;
};

