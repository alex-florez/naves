#include "AmmoUI.h"
#include "Player.h"

AmmoUI::AmmoUI(float x, float y, Game* game)
	: Actor("res/bullet-icon.png", x, y, 24, 24, game) {
	ammo = PLAYER_INITIAL_AMMO;
	textAmmo = new Text(to_string(ammo), x + 34, y, game);
}

void AmmoUI::update(int ammo) {
	this->ammo = ammo;
	textAmmo->content = to_string(this->ammo);
}

void AmmoUI::draw() {
	Actor::draw();
	textAmmo->draw();
}