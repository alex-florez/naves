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

};

