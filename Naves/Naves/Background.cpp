#include "Background.h"


Background::Background(string filename, float x, float y, Game* game)
	: Actor(filename, x, y, WIDTH, HEIGHT, game) {

}

Background::Background(string filename, float x, float y, float vx, Game* game)
	: Actor(filename, x, y, WIDTH, HEIGHT, game) {
	
	this->vx = vx;
	if (vx != 0) {
		backgroundAux = new Background(filename, x + WIDTH, y, game);
	}
}

void Background::update() {
	if (vx != 0) {
		x = x + vx;

		// Se sale por la izquierda
		if (x + width / 2 < 0) {
			// Hacemos que vuelva a aparecer por la derecha
			x = WIDTH + width / 2;
		}
		// Se sale por la derecha
		if (x - width / 2 > WIDTH) {
			// vuelve a aparecer por la izquierda
			x = 0 - width / 2;
		}
	}
}

void Background::draw(float scrollX, float scrollY) {
	Actor::draw(scrollX, scrollY); // Invocación al método del padre

	if (backgroundAux != NULL) {
		// Zona sin cubrir por la izquierda
		if (x - width / 2 > 0) {
			// Pintar fondo auxiliar por la izquierda
			backgroundAux->x = x - width;
		}
		// Zona sin cubrir por la derecha
		if (x + width / 2 < WIDTH) {
			// Pintar fondo auxiliar por la derecha
			backgroundAux->x = x + width;
		}

		backgroundAux->draw();
	}
}