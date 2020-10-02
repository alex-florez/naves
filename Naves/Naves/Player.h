#pragma once

#include "Projectile.h"
#include "Audio.h"
#include "Spaceship.h"
#include <map>

#define PLAYER_LIFES 3

class Player
{
public:
	Player(float x, float y, Game* game);

	void update();
	void moveX(float axis);
	void moveY(float axis);
	Projectile* shoot();
	bool impact();
	void addAmmo(int ammo);
	int getCurrentAmmo(); // Devuelve la munición actual de la nave seleccionada.
	void changeSpaceship(string name); // Método para cambiar de nave

	// Vidas del jugador
	int lifes = PLAYER_LIFES;

	// Audio
	Audio* audioImpact;

	// Mapa de naves disponibles para el jugador
	map<string, Spaceship*> availableSpaceships;

	// Nave actual del jugador
	Spaceship* spaceship;
};

