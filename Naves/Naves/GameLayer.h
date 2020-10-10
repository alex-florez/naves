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
#include "AmmoUI.h"
#include "AmmoCollectable.h"
#include "LifeCollectable.h"

#include <list>

#define ENEMY_SPAWN_TIME 110
#define ENEMY_SPAWN_FREQUENCY 4	

// Frecuencia de aparici�n de elementos coleccionables.
#define AMMO_COLLECTABLE_SPAWN_TIME 350
#define LIFE_COLLECTABLE_SPAWN_TIME 400

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
	AmmoUI* ammoUI;

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
	// Elementos recolectables
	list<AmmoCollectable*> ammoCollectables;
	list<LifeCollectable*> lifeCollectables;

	int newEnemyTime = 0;
	int killedEnemies = 0;

	// Tiempo para que aparezcan nuevos coleccionables
	int newAmmoCollectableTime = AMMO_COLLECTABLE_SPAWN_TIME;
	int newLifeCollectableTime = LIFE_COLLECTABLE_SPAWN_TIME;


	// Puntuaci�n
	Text* textPoints;
	int points;


	// Audio
	Audio* audioBackground;
private:
	void addNewEnemy();
	void addNewAmmoCollectable();
	void addNewLifeCollectable();
	bool playerImpacted();

	void destroyEnemies();
	void destroyProjectiles();
	void destroyEnemyProjectiles();
	void destroyAmmoCollectables();
	void destroyLifeCollectables();
};

