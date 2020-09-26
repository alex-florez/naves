#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Bomb.h"
#include "Text.h"
#include "Audio.h"

#include <list>

#define ENEMY_SPAWN_TIME 110
#define ENEMY_SPAWN_FREQUENCY 5		

#define BOMB_SPAWN_TIME 500


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
	Actor* backgroundPoints;

	// Controles
	bool controlShoot = false;
	int controlMoveX = 0;
	int controlMoveY = 0;

	// Lista de enemigos
	list<Enemy*> enemies;
	// Lista de proyectiles
	list<Projectile*> projectiles;
	// Lista de bombas
	list<Bomb*> bombs;

	// Variables para controlar el spawn de items
	int newEnemyTime = 0;
	int newBombTime = BOMB_SPAWN_TIME;


	int killedEnemies = 0;

	// Puntuación
	Text* textPoints;
	int points;

	// Audio
	Audio* audioBackground;
private:
	void addNewEnemy();
	void addNewBomb();
	void destroyEnemies();
	void destroyProjectiles();
};

