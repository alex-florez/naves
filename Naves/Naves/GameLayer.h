#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"

class GameLayer : public Layer
{
public:
	GameLayer(Game* game);
	void init() override;
	void processControls() override;
	void update() override;
	void draw() override;
	void keysToControls(SDL_Event event);

	Player* player;
	Background* background;

	// Controles
	bool controlShoot = false;
	int controlMoveX = 0;
	int controlMoveY = 0;
};

