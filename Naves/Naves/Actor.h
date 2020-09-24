#pragma once

#include "Game.h"

class Actor
{
public:
	Actor(string filename, float x, float y, int width, int height, Game* game);
	~Actor();
	virtual void draw();
	bool isOverlap(Actor* actor);
	bool isInRender();

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

