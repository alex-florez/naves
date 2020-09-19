#pragma once

#include "Game.h"

class Actor
{
public:
	Actor(string filename, float x, float y, int width, int height, Game* game);
	virtual void draw();

	SDL_Texture* texture;
	// Posici�n
	int x;
	int y;
	// Velocidad
	int vx;
	int vy;
	// Tama�o en el juego
	int width;
	int height;
	// Tama�o de la imagen real
	int fileWidth;
	int fileHeight;
	
	Game* game;

};
