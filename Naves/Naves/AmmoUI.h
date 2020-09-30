#pragma once

#include "Actor.h"
#include "Text.h"

class AmmoUI : public Actor
{
public:
	AmmoUI(float x, float y, Game* game);
	void update(int ammo);
	void draw() override;

private:
	int ammo;
	Text* textAmmo;
};

