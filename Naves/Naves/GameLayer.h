#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Text.h"
#include "Audio.h"
#include "Tile.h"
#include "CrushableTile.h"
#include "DestroyableTile.h"
#include "Space.h"
#include "Pad.h"
#include "DoorTeleports.h"

#include <list>
#include <fstream> // Leer ficheros
#include <sstream> // Leer líneas / strings

#define ENEMY_SPAWN_TIME 110
#define ENEMY_SPAWN_FREQUENCY 5		


class GameLayer : public Layer
{
public:
	GameLayer(Game* game);
	void init() override;
	void processControls() override;
	void update() override;
	void draw() override;
	void keysToControls(SDL_Event event);
	void mouseToControls(SDL_Event event); // Uso del mouse

	// Métodos para cargar el mapa y los objetos del mismo
	void loadMap(string name);
	void loadMapObject(char character, float x, float y);

	// Scroll
	void calculateScroll();

	float scrollX;
	float scrollY;

	int mapWidth; // Ancho del mapa
	int mapHeight; // Alto del mapa
	list<Tile*> tiles; // Lista de tiles.

	// Motor de movimientos / físicas
	Space* space;

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

	int newEnemyTime = 0;
	int killedEnemies = 0;

	// Teletransportes
	DoorTeleports* doorTeleports;

	// Puntuación
	Text* textPoints;
	int points;

	// Audio
	Audio* audioBackground;

	// Copa de final de nivel
	Tile* cup;

	// Elementos de la interfaz
	Pad* pad;
	Actor* buttonJump;
	Actor* buttonShoot;

	// Pausa y mensajes
	Actor* message;
	bool pause;
	bool controlContinue;
private:
	void addNewEnemy();
	void destroyEnemies();
	void destroyProjectiles();
};

