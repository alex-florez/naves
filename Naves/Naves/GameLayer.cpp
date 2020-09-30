#include "GameLayer.h"


void markEnemyForDelete(Enemy* enemy, list<Enemy*>& deleteList);
void markProjectileForDelete(Projectile* projectile, list<Projectile*>& deleteList);
void markEnemyProjectileForDelete(EnemyProjectile* eProjetile, list<EnemyProjectile*>& deleteList);


GameLayer::GameLayer(Game* game)
	: Layer(game) { // Se hace una llamada al constructor del padre
	init();
}

void GameLayer::init() {
	// Puntuación
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

	player = new Player(50, 50, game);
	background = new Background("res/fondo.png", WIDTH * 0.5, HEIGHT * 0.5, -1, game);
	

	// Enemigos
	enemies.clear(); // Vaciar la lista de enemigos, por si se reinicia el juego

	// Proyectiles
	projectiles.clear();

	// Proyectiles de los enemigos
	enemyProjectiles.clear();

	// Enemigos eliminados
	killedEnemies = 0;

	// Semilla aleatoria para generar números aleatorios
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
* Método encargado de actualizar todos los elementos del juego.
**/
void GameLayer::update() {

	list<Enemy*> deleteEnemies; // Enemigos a eliminar
	list<Projectile*> deleteProjectiles; // Proyectiles a eliminar
	list<EnemyProjectile*> deleteEnemyProjectiles; // Proyectiles del enemigo a eliminar

	// Actualizamos el fondo móvil
	background->update();

	// Actualizamos la UI
	healthBar->update();
	ammoUI->update(player->ammo);

	// Generamos los enemigos
	addNewEnemy();

	player->update();
	// Actualizamos los enemigos
	for (auto const& enemy : enemies) {
		enemy->update();
		if (enemy->x + enemy->width/2 <= 0) {  // Enemigo a la izquierda de la pantalla
			markEnemyForDelete(enemy, deleteEnemies);
		} else if (player->isOverlap(enemy)) { // Colisión con el jugador
			markEnemyForDelete(enemy, deleteEnemies); // Se elimina el enemigo
			if (playerImpacted()) { // Se reinicia el juego, si ya no quedan vidas.
				init();
				return; 
			}
		}
		// El enemigo realiza un disparo, solo si está dentro del render
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
		else if (player->isOverlap(eProjectile)) { // Colisiones entre jugador y proyectiles enemigos
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
				// Incrementar el nº de enemigos eliminados
				killedEnemies++;
				// Incrementar puntuación
				points++;
				textPoints->content = to_string(points);
				// Incrementar en uno la munición
				player->addAmmo(1);
				markProjectileForDelete(projectile, deleteProjectiles);
				markEnemyForDelete(enemy, deleteEnemies);
			}
		}
	}




	// Eliminación de elementos del juego
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

	// Información
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
	// Se dibujan los proyectiles del enemigo
	for (auto const& eProjectile : enemyProjectiles) {
		eProjectile->draw();
	}

	// Dibujamos elementos de la UI
	textPoints->draw();
	backgroundPoints->draw();
	healthBar->draw();
	ammoUI->draw();

	SDL_RenderPresent(game->renderer); // Renderiza el juego
}


// Métodos privados
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


// Molaría a esto
// markActorForDelete(projectile, deleteProjectiles);
// markActorForDelete(enemy, deleteEnemies);
void markActorForDelete(Actor* actor, list<Actor*>& deleteList) {

}


void GameLayer::addNewEnemy() {
	newEnemyTime--;
	int n = enemies.size(); // Nº actual de enemigos
	int numEnemies = (killedEnemies / ENEMY_SPAWN_FREQUENCY) + 1;
	if (newEnemyTime <= 0) {
		for (int i = 0; i < numEnemies; i++) { // Crear nº de enemigos definido por la función
			// Random position
			int rX = (rand() % (600 - 500)) + 1 + 500;
			int rY = (rand() % (300 - 60)) + 1 + 60;
			enemies.push_back(new Enemy(rX + 4 * numEnemies, rY, game));
		}
		newEnemyTime = ENEMY_SPAWN_TIME + 2*(numEnemies + n);
	}
}

// Métodos encargados de destruir objetos en memoria
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

/*
	Método invocado cuando el actor Player
	es impactado por un proyectil enemigo o por una
	nave enemiga. Devuelve true si el jugador se ha quedado
	sin vidas.
*/
bool GameLayer::playerImpacted() {
	bool noLifes = player->impact();
	healthBar->setLifes(player->lifes); // Actualizar vidas en la UI
	return noLifes;
}