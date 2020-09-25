#pragma once

#include "Game.h"

class Animation
{
public:
	Animation(string filename, float actorWidth, float actorHeight,
				float fileWidth, float fileHeight, int updateFrequence,
					int totalFrames, bool loop, Game* game);

	bool update();
	void draw(float x, float y);
	int actorWidth; // ancho del actor
	int actorHeight; // alto del actor
	int fileWidth; // Ancho del Sprite Sheet
	int fileHeight; // Alto del Sprite Sheet
	int currentFrame;
	int totalFrames;
	bool loop;
	Game* game;

	// Variables auxiliares
	SDL_Texture* texture; // imagen
	SDL_Rect source; // Parte de la imagen que se recorta del fichero
	float frameWidth; // Ancho de un fotograma
	float frameHeight; // Alto de un fotograma
	int updateFrequence = 0;
	int updateTime = 0;

};

