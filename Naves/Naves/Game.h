#pragma once

// Librerías SDL y otras utilidades
#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

using namespace std;

// Valores generales
#define WIDTH 480
#define HEIGHT 320
#define FPS 30

#include "Layer.h"
class Layer;

class Game
{
public:
	Game();
	void loop();
	void scale();

	SDL_Window* window; // ventana
	SDL_Renderer* renderer; // renderizador
	bool loopActive; // Juego activo?
	Layer* gameLayer;
	bool scaledToMax = false;
	float scaleLower = 1; // Factor de escalado
};

