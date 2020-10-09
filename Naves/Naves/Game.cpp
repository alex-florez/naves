#include "Game.h"
#include "GameLayer.h"
#include "MenuLayer.h"

Game::Game() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Error SDL_Init " << SDL_GetError() << endl;
	}

	if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer) < 0) {
		cout << "Error Window y Renderer" << SDL_GetError() << endl;
	}

	SDL_SetWindowTitle(window, "Juego de Naves");
	// Escalado de imágenes de calidad 
	// https://wiki.libsdl.org/SDL_HINT_RENDER_SCALE_QUALITY
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	// Creamos la gameLayer
	gameLayer = new GameLayer(this);
	menuLayer = new MenuLayer(this);

	layer = menuLayer; // Pantalla inicial MenuLayer
	// Fuentes de texto
	TTF_Init();
	font = TTF_OpenFont("res/sans.ttf", 24);

	loopActive = true; // Bucle activo
	loop();
}

void Game::loop() {
	int initTick; // ms de inicio loop
	int endTick; // ms de fin de loop
	int differenceTick; // fin - inicio
	while (loopActive) {
		initTick = SDL_GetTicks();

		// Controles
		layer->processControls();
		// Actualizar elementos
		layer->update();
		// Dibujar
		layer->draw();

		endTick = SDL_GetTicks();
		differenceTick = endTick - initTick;

		if (differenceTick < (1000 / FPS)) {
			SDL_Delay((1000 / FPS) - differenceTick);
		}
	}
}

void Game::scale() {
	scaledToMax = !scaledToMax;

	if (scaledToMax) {
		SDL_DisplayMode PCdisplay;
		SDL_GetCurrentDisplayMode(0, &PCdisplay);
		float scaleX = (float)PCdisplay.w / (float)WIDTH;
		float scaleY = (float)PCdisplay.h / (float)HEIGHT;
		// Nos quedamos con la menor  de las dos escalas para no deformar el juego.
		scaleLower = scaleX;
		if (scaleY < scaleX) {
			scaleLower = scaleY;
		}
		// Cambiar dimensiones de la ventana
		SDL_SetWindowSize(window, WIDTH * scaleLower, HEIGHT * scaleLower);
		// Cambiar escala del render
		SDL_RenderSetScale(renderer, scaleLower, scaleLower);
	}
	else { // Escala original
		scaleLower = 1;
		// Cambiar dimensiones de la ventana
		SDL_SetWindowSize(window, WIDTH, HEIGHT);
		// Cambiar escala del render
		SDL_RenderSetScale(renderer, 1, 1);
	}

	// Para centrar la pantalla
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

SDL_Texture* Game::getTexture(string filename) {
	if (mapTextures.find(filename) != mapTextures.end()) {
		cout << "Retorno recurso cacheado" << filename << endl;
	}
	else {
		cout << "Cachear nuevo recurso" << filename << endl;
		SDL_Surface* surface = IMG_Load(filename.c_str());
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		mapTextures[filename] = texture;
	}

	return mapTextures[filename];
}