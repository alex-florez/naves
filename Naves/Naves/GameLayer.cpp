#include "GameLayer.h"


void markEnemyForDelete(Enemy* enemy, list<Enemy*>& deleteList);
void markProjectileForDelete(Projectile* projectile, list<Projectile*>& deleteList);

template <class T>
void markActorForDelete(T* actor, const list<T*>& deleteList) {
	bool inList = std::find(deleteList.begin(),
		deleteList.end(),
		actor) != deleteList.end();
	if (!inList) {
		deleteList.push_back(actor);
	}
}


GameLayer::GameLayer(Game* game)
	: Layer(game) { // Se hace una llamada al constructor del padre
	init();
}

void GameLayer::init() {
	// Puntuaci�n
	points = 0;
	textPoints = new Text("hola", WIDTH * 0.92, HEIGHT * 0.05, game);
	textPoints->content = to_string(points);
	backgroundPoints = new Actor("res/icono_puntos.png", WIDTH * 0.85, HEIGHT * 0.05, 24, 24, game);

	// Destruir posibles objetos existentes
	delete player;
	destroyEnemies();
	destroyProjectiles();

	player = new Player(50, 50, game);
	background = new Background("res/fondo.png", WIDTH * 0.5, HEIGHT * 0.5, -1, game);
	

	// Enemigos
	enemies.clear(); // Vaciar la lista de enemigos, por si se reinicia el juego

	// Proyectiles
	projectiles.clear();

	// Enemigos eliminados
	killedEnemies = 0;

	// Semilla aleatoria para generar n�meros aleatorios
	srand(SDL_GetTicks());

	// Audio de fondo
	audioBackground = new Audio("res/musica_ambiente.mp3", true);
	audioBackground->play();

}

void GameLayer::processControls() {
	// obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		keysToControls(event);
	}
	// procesar controles
	// Disparar
	if (controlShoot) {
		Projectile* newProjectile = player->shoot();
		if (newProjectile != nullptr) {
			projectiles.push_back(newProjectile);
		}
	}
	// Eje X
	if (controlMoveX > 0) {
		player->moveX(1);
	}
	else if (controlMoveX < 0) {
		player->moveX(-1);
	}
	else {
		player->moveX(0);
	}

	// Eje Y
	if (controlMoveY > 0) {
		player->moveY(1);
	}
	else if (controlMoveY < 0) {
		player->moveY(-1);
	}
	else {
		player->moveY(0);
	}
}

void GameLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) { // Tecla pulsada
		int code = event.key.keysym.sym;
		switch (code) {
		case SDLK_ESCAPE: // Tecla Esc
			game->loopActive = false;
			break;
		case SDLK_1: // Tecla 1
			game->scale();
			break;
		case SDLK_d: // derecha
			controlMoveX = 1;
			break;
		case SDLK_a: // izquierda
			controlMoveX = -1;
			break;
		case SDLK_w: // arriba
			controlMoveY = -1;
			break;
		case SDLK_s: // abajo
			controlMoveY = 1;
			break;
		case SDLK_SPACE: // disparar
			controlShoot = true;
			break;
		}
	}

	if (event.type == SDL_KEYUP) { // Tecla levantada
		int code = event.key.keysym.sym;
		switch (code) {
		case SDLK_d: // derecha
			if (controlMoveX == 1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_a: // izquierda
			if (controlMoveX == -1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_w: // arriba
			if (controlMoveY == -1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_s: // abajo
			if (controlMoveY == 1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_SPACE: // disparar
			controlShoot = false;
			break;
		}
	}
	// Se hace click en la X para cerrar la ventana
	if (event.type == SDL_QUIT) {
		game->loopActive = false;
	}
}


/**
* M�todo encargado de actualizar todos los elementos del juego.
**/
void GameLayer::update() {

	list<Enemy*> deleteEnemies; // Enemigos a eliminar
	list<Projectile*> deleteProjectiles; // Proyectiles a eliminar

	// Actualizamos el fondo m�vil
	background->update();

	// Generamos los enemigos
	addNewEnemy();

	player->update();
	// Actualizamos los enemigos
	for (auto const& enemy : enemies) {
		enemy->update();
		// Enemigo a la izquierda de la pantalla
		if (enemy->x + enemy->width/2 <= 0) { 
			markEnemyForDelete(enemy, deleteEnemies);
		} else if (player->isOverlap(enemy)) { // Colisi�n con el jugador
			init();
			return; // Se reinicia el juego.
		}
	}
	// Actualizamos los proyectiles
	for (auto const& projectile : projectiles) {
		projectile->update();
		// Proyectil a la derecha de la pantalla
		if (!projectile->isInRender()) {
			markProjectileForDelete(projectile, deleteProjectiles);
		}
	}

	// Colisiones entre enemigos y proyectiles
	for (auto const& enemy : enemies) {
		for (auto const& projectile : projectiles) {
			if (enemy->isOverlap(projectile)) {
				// Incrementar el n� de enemigos eliminados
				killedEnemies++;
				// Incrementar puntuaci�n
				points++;
				textPoints->content = to_string(points);
				/*markProjectileForDelete(projectile, deleteProjectiles);
				markEnemyForDelete(enemy, deleteEnemies);*/

				markActorForDelete(projectile, deleteProjectiles);
			}
		}
	}


	// Eliminamos los proyectiles y enemigos necesarios
	for (auto const& delEnemy : deleteEnemies) {
		enemies.remove(delEnemy);
		delete delEnemy; // Se destruye el enemigo.
	}
	deleteEnemies.clear();

	for (auto const& delProjectile : deleteProjectiles) {
		projectiles.remove(delProjectile);
		delete delProjectile; // Se destruye el proyectil.
	}
	deleteProjectiles.clear();

	// Informaci�n
	cout << "Killed Enemies: " << killedEnemies 
		 << " Current enemies: " << enemies.size() 
		 << " Shoots: " << projectiles.size() << endl;
}


void GameLayer::draw() {
	background->draw();
	player->draw();
	// Dibujamos los enemigos
	for (auto const& enemy : enemies) {
		enemy->draw();
	}
	// Dibujamos los proyectiles
	for (auto const& projectile : projectiles) {
		projectile->draw();
	}

	textPoints->draw();
	backgroundPoints->draw();

	SDL_RenderPresent(game->renderer); // Renderiza el juego
}


// M�todos privados
// **********************************************************************************

void markEnemyForDelete(Enemy* enemy, list<Enemy*>& deleteList) {
	bool inList = std::find(deleteList.begin(),
		deleteList.end(),
		enemy) != deleteList.end();
	if (!inList) {
		deleteList.push_back(enemy);
	}
}

void markProjectileForDelete(Projectile* projectile, list<Projectile*>& deleteList) {
	bool inList = std::find(deleteList.begin(),
		deleteList.end(),
		projectile) != deleteList.end();
	if (!inList) {
		deleteList.push_back(projectile);
	}
}

// Molar�a esto
// markActorForDelete(projectile, deleteProjectiles);
// markActorForDelete(enemy, deleteEnemies);



void GameLayer::addNewEnemy() {
	newEnemyTime--;
	if (newEnemyTime <= 0) {
		for (int i = 0; i < (killedEnemies / ENEMY_SPAWN_FREQUENCY) + 1; i++) {
			cout << "New enemy spawned" << endl;
			// Random position
			int rX = (rand() % (600 - 500)) + 1 + 500;
			int rY = (rand() % (300 - 60)) + 1 + 60;
			enemies.push_back(new Enemy(rX, rY, game));
		}
		newEnemyTime = ENEMY_SPAWN_TIME;
	}
}

void GameLayer::destroyEnemies() {
	for (auto const& enemy : enemies) {
		delete enemy;
	}
}

void GameLayer::destroyProjectiles() {
	for (auto const& projectile : projectiles) {
		delete projectile;
	}
}

