#include "Game.h"
#undef main; // para quitar el main definido en la librería SDL_main

int main() {
	Game* game = new Game();
	return 0;
}