#include "GameLayer.h"


void markEnemyForDelete(Enemy* enemy, list<Enemy*>& deleteList);
void markProjectileForDelete(Projectile* projectile, list<Projectile*>& deleteList);
void markEnemyProjectileForDelete(EnemyProjectile* eProjetile, list<EnemyProjectile*>& deleteList);
void markAmmoCollectableForDelete(AmmoCollectable* ammoCollectable, list<AmmoCollectable*>& deletelist);
void markLifeCollectableForDelete(LifeCollectable* lifeCollectable, list<LifeCollectable*>& deleteList);

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

	// Elementos de la UI
	healthBar = new HealthBar(WIDTH * 0.7, HEIGHT * 0.05, game);
	ammoUI = new AmmoUI(WIDTH * 0.55, HEIGHT * 0.05, game);

	// Destruir posibles objetos existentes
	delete player;
	destroyEnemies();
	destroyProjectiles();
	destroyEnemyProjectiles();
	destroyAmmoCollectables();
	destroyLifeCollectables();

	player = new Player(50, 50, game);
	background = new Background("res/fondo.png", WIDTH * 0.5, HEIGHT * 0.5, -1, game);
	

	// Enemigos
	enemies.clear(); // Vaciar la lista de enemigos, por si se reinicia el juego

	// Proyectiles
	projectiles.clear();

	// Proyectiles de los enemigos
	enemyProjectiles.clear();

	// Elementos recolectables
	ammoCollectables.clear();
	lifeCollectables.clear();


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
			player->changeSpaceship("nave1"); // Cambiamos a la nave1
			break;
		case SDLK_2: // Tecla 2
			player->changeSpaceship("nave2"); // Cambiamos a la nave2
			break;
		case SDLK_m: // Tecla m
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
	list<EnemyProjectile*> deleteEnemyProjectiles; // Proyectiles del enemigo a eliminar
	list<AmmoCollectable*> deleteAmmoCollectables; // Coleccionables de munici�n a eliminar
	list<LifeCollectable*> deleteLifeCollectables; // Coleccionables de vidas a eliminar

	// Actualizamos el fondo m�vil
	background->update();

	// Actualizamos la UI
	healthBar->update();
	ammoUI->update(player->getCurrentAmmo());

	// Generamos los enemigos
	addNewEnemy();

	// Generamos los recolectables
	addNewAmmoCollectable();
	addNewLifeCollectable();

	player->update();
	// Actualizamos los enemigos
	for (auto const& enemy : enemies) {
		enemy->update();
		if (enemy->x + enemy->width/2 <= 0) {  // Enemigo a la izquierda de la pantalla
			markEnemyForDelete(enemy, deleteEnemies);
		} else if (player->spaceship->isOverlap(enemy)) { // Colisi�n con el jugador
			markEnemyForDelete(enemy, deleteEnemies); // Se elimina el enemigo
			if (playerImpacted()) { // Se reinicia el juego, si ya no quedan vidas.
				init();
				return; 
			}
		}
		// El enemigo realiza un disparo, solo si est� dentro del render
		if (enemy->isInRender()) {
			EnemyProjectile* eProjectile = enemy->autoShoot();
			if (eProjectile != nullptr) {
				enemyProjectiles.push_back(eProjectile);
			}
		}		
	}
	// Actualizamos los proyectiles
	for (auto const& projectile : projectiles) {
		projectile->update();
		// Proyectil fuera de la pantalla
		if (!projectile->isInRender()) {
			markProjectileForDelete(projectile, deleteProjectiles);
		}
	}

	// Actualizar proyectiles del enemigo
	for (auto const& eProjectile : enemyProjectiles) {
		eProjectile->update();
		if (!eProjectile->isInRender()) { // Proyectil fuera del render
			markEnemyProjectileForDelete(eProjectile, deleteEnemyProjectiles);
		}
		else if (player->spaceship->isOverlap(eProjectile)) { // Colisiones entre jugador y proyectiles enemigos
			markEnemyProjectileForDelete(eProjectile, deleteEnemyProjectiles); // Se elimina el proyectil
			if (playerImpacted()) {
				init();
				return;
			}
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
				// Incrementar en uno la munici�n
				player->addAmmo(1);
				markProjectileForDelete(projectile, deleteProjectiles);
				markEnemyForDelete(enemy, deleteEnemies);
			}
		}
	}

	// Colisiones del jugador con elementos recolectables
	for (auto const& ammoCollectable : ammoCollectables) {
		if (player->spaceship->isOverlap(ammoCollectable)) { // Jugador recoge munici�n
			player->addAmmo(ammoCollectable->ammoValue); // Se incrementa el n�mero de disparos
			markAmmoCollectableForDelete(ammoCollectable, deleteAmmoCollectables); // Se elimina el recolectable.
		}
	}

	for (auto const& lifeCollectable : lifeCollectables) {
		if (player->spaceship->isOverlap(lifeCollectable)) { // Jugador recoge vida
			player->lifes++; // Se incrementa el n�mero de vidas
			healthBar->setLifes(player->lifes); // Actualizar HealthBar
			markLifeCollectableForDelete(lifeCollectable, deleteLifeCollectables);
		}
	}


	// Eliminaci�n de elementos del juego
	// **********************************

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

	// Eliminamos los proyectiles enemigos necesarios
	for (EnemyProjectile* eProjectile : deleteEnemyProjectiles) {
		enemyProjectiles.remove(eProjectile);
		delete eProjectile;
	}
	deleteEnemyProjectiles.clear();

	// Eliminamos elementos recolectables
	for (auto const& ammoCollectable : deleteAmmoCollectables) {
		ammoCollectables.remove(ammoCollectable);
		delete ammoCollectable;
	}
	deleteAmmoCollectables.clear();

	for (auto const& lifeCollectable : deleteLifeCollectables) {
		lifeCollectables.remove(lifeCollectable);
		delete lifeCollectable;
	}
	deleteLifeCollectables.clear();

	// Informaci�n
	cout << "Killed Enemies: " << killedEnemies
		<< " Current enemies: " << enemies.size()
		<< " Shoots: " << projectiles.size() << endl;
}


void GameLayer::draw() {
	background->draw();
	player->spaceship->draw();
	// Dibujamos los enemigos
	for (auto const& enemy : enemies) {
		enemy->draw();
	}
	// Dibujamos los proyectiles
	for (auto const& projectile : projectiles) {
		projectile->draw();
	}
	// Se dibujan los proyectiles del enemigo
	for (auto const& eProjectile : enemyProjectiles) {
		eProjectile->draw();
	}

	// Dibujamos elementos coleccionables
	for (auto const& ammoCollectable : ammoCollectables) {
		ammoCollectable->draw();
	}

	for (auto const& lifeCollectable : lifeCollectables) {
		lifeCollectable->draw();
	}

	// Dibujamos elementos de la UI
	textPoints->draw();
	backgroundPoints->draw();
	healthBar->draw();
	ammoUI->draw();

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

void markEnemyProjectileForDelete(EnemyProjectile* eProjetile, list<EnemyProjectile*>& deleteList) {
	bool inList = std::find(deleteList.begin(),
		deleteList.end(),
		eProjetile) != deleteList.end();
	if (!inList) {
		deleteList.push_back(eProjetile);
	}
}

void markAmmoCollectableForDelete(AmmoCollectable* ammoCollectable, list<AmmoCollectable*>& deleteList) {
	bool inList = std::find(deleteList.begin(),
		deleteList.end(),
		ammoCollectable) != deleteList.end();
	if (!inList) {
		deleteList.push_back(ammoCollectable);
	}
}


void markLifeCollectableForDelete(LifeCollectable* lifeCollectable, list<LifeCollectable*>& deleteList) {
	bool inList = std::find(deleteList.begin(),
		deleteList.end(),
		lifeCollectable) != deleteList.end();
	if (!inList) {
		deleteList.push_back(lifeCollectable);
	}
}


// Molar�a a esto
// markActorForDelete(projectile, deleteProjectiles);
// markActorForDelete(enemy, deleteEnemies);
void markActorForDelete(Actor* actor, list<Actor*>& deleteList) {

}


void GameLayer::addNewEnemy() {
	newEnemyTime--;
	int n = enemies.size(); // N� actual de enemigos
	int numEnemies = (killedEnemies / ENEMY_SPAWN_FREQUENCY) + 1;
	if (newEnemyTime <= 0) {
		for (int i = 0; i < numEnemies; i++) { // Crear n� de enemigos definido por la funci�n
			// Random position
			int rX = (rand() % (600 - 500)) + 1 + 500;
			int rY = (rand() % (300 - 60)) + 1 + 60;
			enemies.push_back(new Enemy(rX + 4 * numEnemies, rY, game));
		}
		newEnemyTime = ENEMY_SPAWN_TIME + 2*(numEnemies + n);
	}
}


void GameLayer::addNewAmmoCollectable() {
	newAmmoCollectableTime--;

	if (newAmmoCollectableTime <= 0) {
		// Posici�n aleatoria para que aparezca en la mitad derecha del render
		int rX = (rand() % (WIDTH - WIDTH / 2)) + 1 + WIDTH/2;
		int rY = (rand() % (HEIGHT-18-18)) + 1 + 18;
		ammoCollectables.push_back(new AmmoCollectable(10, rX, rY, game));
		newAmmoCollectableTime = AMMO_COLLECTABLE_SPAWN_TIME;
	}
}

void GameLayer::addNewLifeCollectable() {
	newLifeCollectableTime--;

	if (newLifeCollectableTime <= 0) {
		// Posici�n aleatoria para que aparezca en la mitad derecha del render
		int rX = (rand() % (WIDTH - WIDTH / 2)) + 1 + WIDTH / 2;
		int rY = (rand() % (HEIGHT - 18 - 18)) + 1 + 18;
		lifeCollectables.push_back(new LifeCollectable(rX, rY, game));
		newLifeCollectableTime = LIFE_COLLECTABLE_SPAWN_TIME;
	}
}

// M�todos encargados de destruir objetos en memoria
// *************************************************

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

void GameLayer::destroyEnemyProjectiles() {
	for (auto const& eProjectile : enemyProjectiles) {
		delete eProjectile;
	}
}

void GameLayer::destroyAmmoCollectables() {
	for (auto const& ammoCollectable : ammoCollectables) {
		delete ammoCollectable;
	}
}

void GameLayer::destroyLifeCollectables() {
	for (auto const& lifeCollectable : lifeCollectables) {
		delete lifeCollectable;
	}
}

/*
	M�todo invocado cuando el actor Player
	es impactado por un proyectil enemigo o por una
	nave enemiga. Devuelve true si el jugador se ha quedado
	sin vidas.
*/
bool GameLayer::playerImpacted() {
	bool noLifes = player->impact();
	healthBar->setLifes(player->lifes); // Actualizar vidas en la UI
	return noLifes;
}