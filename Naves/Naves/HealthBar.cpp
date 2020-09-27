#include "HealthBar.h"

HealthBar::HealthBar(float x, float y, Game* game)
	: Actor("res/heart.png", x, y, 24, 24, game) {
	lifes = INITIAL_LIFES;
	textLifes = new Text(to_string(lifes), x + 34, y, game);
}

void HealthBar::setLifes(int lifes) {
	this->lifes = lifes;
}


void HealthBar::update() {
	textLifes->content = to_string(this->lifes);
}

void HealthBar::draw() {
	Actor::draw();
	textLifes->draw();
}