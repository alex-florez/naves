#pragma once

#include "Door.h"
#include <list>

/*
	Representa un conjunto de pares de puertas
	interconectadas entre sí.
*/
class DoorTeleports
{
public:
	DoorTeleports();
	void addDoor(Door* door); // Añade una nueva puerta
	void clearTeleports(); // Elimina todos los teletransportes
	void draw(float scrollX = 0, float scrollY = 0); // Dibuja las puertas de los teletransportes
	
	list<Door*> doors; // Listado de puertas

private:
	Door* findById(char id); // Busca en la lista de puertas una puerta con el id pasado como parámetro
};

