#include "GameLayer.h"

GameLayer::GameLayer(Game* game)
	: Layer(game) { // Se hace una llamada al constructor del padre
	init();
}

void GameLayer::init() {

}

void GameLayer::processControls() {

}

void GameLayer::update() {
	cout << "update GameLayer" << endl;
}

void GameLayer::draw() {
	SDL_RenderPresent(game->renderer); // Renderiza el juego
}
