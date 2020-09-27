#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"
#include "Enemy.h"
#include "Projectile.h"
#include "EnemyProjectile.h"
#include "Text.h"
#include "Audio.h"
#include "HealthBar.h"

#include <list>

#define ENEMY_SPAWN_TIME 110
#define ENEMY_SPAWN_FREQUENCY 4		


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

	// HUD
	Actor* backgroundPoints;
	Actor* backgroundLifes;
	HealthBar* healthBar;

	// Controles
	bool controlShoot = false;
	int controlMoveX = 0;
	int controlMoveY = 0;

	// Lista de enemigos
	list<Enemy*> enemies;
	// Lista de proyectiles
	list<Projectile*> projectiles;
	// Lista de proyectiles enemigos
	list<EnemyProjectile*> enemyProjectiles;

	int newEnemyTime = 0;
	int killedEnemies = 0;


	// Puntuación
	Text* textPoints;
	int points;

	// Vidas
	Text* textLifes;

	// Audio
	Audio* audioBackground;
private:
	void addNewEnemy();
	void destroyEnemies();
	void destroyProjectiles();
	void destroyEnemyProjectiles();
	bool playerImpacted();
};

