#pragma once

#include "Game.h"

class Text
{
public:
	Text(string content, float x, float y, Game* game);
	void draw();

	string content; // Texto
	int x;
	int y;
	int width;
	int height;
	Game* game; // Referencia al juego
};

