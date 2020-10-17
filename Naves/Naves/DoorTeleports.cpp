#include "DoorTeleports.h"



DoorTeleports::DoorTeleports() {

}

void DoorTeleports::addDoor(Door* door) {
	// Buscar su pareja (mismo id)
	Door* partnerDoor = findById(door->id);
	if (partnerDoor != nullptr) { // Si se encuentra su pareja, asignar coordenadas de destino
		door->destinyX = partnerDoor->x;
		door->destinyY = partnerDoor->y;

		partnerDoor->destinyX = door->x;
		partnerDoor->destinyY = door->y;
	}
	
	doors.push_back(door);
}

void DoorTeleports::clearTeleports() {
	this->doors.clear();
}

void DoorTeleports::draw(float scrollX, float scrollY) {
	for (auto const& door : doors) {
		door->draw(scrollX, scrollY);
	}
}

Door* DoorTeleports::findById(char id) {
	for (auto const& door : doors) {
		if (door->id == id) {
			return door;
		}
	}
	return nullptr;
}
