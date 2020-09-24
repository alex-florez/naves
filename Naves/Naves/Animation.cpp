#include "Animation.h"


Animation::Animation(string filename, float actorWidth, float actorHeight,
	float fileWidth, float fileHeight, int updateFrequence,
	int totalFrames, Game* game) {

	// Cargar la textura
	SDL_Surface* surface = IMG_Load(filename.c_str());
	texture = SDL_CreateTextureFromSurface(game->renderer, surface);

	this->actorWidth = actorWidth;
	this->actorHeight = actorHeight;
	this->fileWidth = fileWidth;
	this->fileHeight = fileHeight;
	this->updateFrequence = updateFrequence;
	this->totalFrames = totalFrames;
	this->game = game;

	updateTime = 0; // �ltima actualizaci�n
	currentFrame = 0;

	// Calcular lo que mide un fotograma
	frameWidth = fileWidth / totalFrames;
	frameHeight = fileHeight;
	// Rect�ngulo de recorte del fotograma
	source.x = 0;
	source.y = 0;
	source.w = frameWidth;
	source.h = frameHeight;
}


bool Animation::update() {
	updateTime++;

	if (updateTime > updateFrequence) {
		updateTime = 0;
		// Actualizar el frame
		currentFrame++;
		// Si estamos en el �ltimo frame, volver al primero
		if (currentFrame >= totalFrames) {
			currentFrame = 0;
		}
	}
	// Actualizar el rect�ngulo del source (siguiente frame)
	source.x = currentFrame * frameWidth;
	return false;
}

void Animation::draw(float x, float y) {
	// Donde se va a pegar en el renderer
	SDL_Rect destination;
	destination.x = x - actorWidth / 2;
	destination.y = y - actorHeight / 2;
	destination.w = actorWidth;
	destination.h = actorHeight;
	// Modificar para que la referencia sea el punto central

	SDL_RenderCopyEx(game->renderer, texture, &source, &destination, 0, NULL, SDL_FLIP_NONE);
}