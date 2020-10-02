#pragma once

#include "Actor.h"
#include <list>

class Space
{
public:
	Space(float gravity);

	void update();
	void updateMoveRight(Actor* dynamicAct);
	void updateMoveLeft(Actor* dynamicAct);

	void updateMoveTop(Actor* dynamicAct);
	void updateMoveBottom(Actor* dynamicAct);



	void addDynamicActor(Actor* actor);
	void addStaticActor(Actor* actor);
	void removeDynamicActor(Actor* actor);
	void removeStaticActor(Actor* actor);


	float gravity;
	list<Actor*> dynamicActors; // Actores dinámicos
	list<Actor*> staticActors; // Actores estáticos
};


