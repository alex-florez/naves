#pragma once

// Librerías SDL y otras utilidades
#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <map>

using namespace std;

// Valores generales
#define WIDTH 480
#define HEIGHT 320
#define FPS 30

#include "Layer.h"
class Layer;

enum class GameInputType {
	KEYBOARD, MOUSE, GAMEPAD
};

class Game
{
public:
	Game();
	void loop();
	void scale();
	SDL_Texture* getTexture(string filename);
	map<string, SDL_Texture*> mapTextures; // map - caché para las texturas.


	SDL_Window* window; // ventana
	SDL_Renderer* renderer; // renderizador
	bool loopActive; // Juego activo?
	bool scaledToMax = false;
	float scaleLower = 1; // Factor de escalado
	TTF_Font* font;

	// Layers
	Layer* layer;
	Layer* menuLayer;
	Layer* gameLayer;

	// Estados y orientaciones
	int const stateMoving = 1;
	int const stateJumping = 2;
	int const stateDying = 3;
	int const stateDead = 4;
	int const stateShooting = 5;

	int const orientationRight = 1;
	int const orientationLeft = 2;

	// Control de niveles
	int currentLevel = 0;
	int finalLevel = 2;

	// Tipos de entrada
	GameInputType input;


	int const inputKeyboard = 1;
	int const inputMouse = 2;
	int const inputGamePad = 3;

};

