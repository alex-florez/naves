#pragma once

#include "Game.h"

class Actor
{
public:
	Actor(string filename, float x, float y, int width, int height, Game* game);
	~Actor();
	virtual void draw(float scrollX = 0);
	bool isOverlap(Actor* actor);
	bool isInRender(float scrollX = 0);


	// Colisiones
	bool collisionDown; // Indica si el actor está colisionando por abajo.
	bool collisionLeft; // Actor colisiona por la izquierda.
	bool collisionRight; // Actor colisiona por la derecha.

	SDL_Texture* texture;
	// Posición
	int x;
	int y;
	// Velocidad
	int vx;
	int vy;
	// Tamaño en el juego
	int width;
	int height;
	// Tamaño de la imagen real
	int fileWidth;
	int fileHeight;
	
	Game* game;

	// Capacidad para destruir a otros actores
	bool canDestroy = false;

	// Capacidad para ser destruido
	bool breakable = false;
	bool destroyed = false;
};

