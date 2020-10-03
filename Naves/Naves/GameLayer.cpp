#include "GameLayer.h"


void markEnemyForDelete(Enemy* enemy, list<Enemy*>& deleteList);
void markProjectileForDelete(Projectile* projectile, list<Projectile*>& deleteList);

//template <class T>
//void markActorForDelete(T* actor, const list<T*>& deleteList) {
//	bool inList = std::find(deleteList.begin(),
//		deleteList.end(),
//		actor) != deleteList.end();
//	if (!inList) {
//		deleteList.push_back(actor);
//	}
//}


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

	// Scroll
	scrollX = 0; // A la izquierda del todo

	// limpiar los tiles
	tiles.clear();

	space = new Space(1); // Instanciamos el motor de físicas con gravedad.

	// Destruir posibles objetos existentes
	delete player;
	destroyEnemies();
	destroyProjectiles();

	background = new Background("res/fondo_2.png", WIDTH * 0.5, HEIGHT * 0.5, -1, game);
	

	// Enemigos
	enemies.clear(); // Vaciar la lista de enemigos, por si se reinicia el juego

	// Proyectiles
	projectiles.clear();

	// Enemigos eliminados
	killedEnemies = 0;

	// Semilla aleatoria para generar números aleatorios
	srand(SDL_GetTicks());

	// Audio de fondo
	audioBackground = new Audio("res/musica_ambiente.mp3", true);
	audioBackground->play();

	// cargamos el mapa a partir del fichero
	loadMap("res/0.txt");
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
			space->addDynamicActor(newProjectile);
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
	
	}
	else if (controlMoveY < 0) {
		player->jump();
	}
	else {
		
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

	// Actualizamos todos los actores dinámicos
	space->update();

	// Actualizamos el fondo móvil
	background->update();

	player->update();
	// Actualizamos los enemigos
	for (auto const& enemy : enemies) {
		enemy->update();
		// Enemigo a la izquierda de la pantalla
		if (enemy->x + enemy->width/2 <= 0) { 
			markEnemyForDelete(enemy, deleteEnemies);
		} else if (player->isOverlap(enemy)) { // Colisión con el jugador
			init();
			return; // Se reinicia el juego.
		}
	}
	// Actualizamos los proyectiles
	for (auto const& projectile : projectiles) {
		projectile->update();
		// Proyectil a la derecha de la pantalla o proyectil sin velocidad (impacto con algún elemnto estático)
		if (!projectile->isInRender(scrollX) || projectile->vx == 0) {
			markProjectileForDelete(projectile, deleteProjectiles);
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
				markProjectileForDelete(projectile, deleteProjectiles);
				//markEnemyForDelete(enemy, deleteEnemies);
				enemy->impacted();
			}
		}
	}

	for (auto const& enemy : enemies) { // Marcamos para eliminar aquellos enemigos en el estado muerto.
		if (enemy->state == game->stateDead) {
			markEnemyForDelete(enemy, deleteEnemies);
		}
	}

	// Eliminamos los proyectiles y enemigos necesarios
	for (auto const& delEnemy : deleteEnemies) {
		enemies.remove(delEnemy);
		space->removeDynamicActor(delEnemy); // Eliminamos al enemigo del motor de físicas.
		delete delEnemy; // Se destruye el enemigo.
	}
	deleteEnemies.clear();

	for (auto const& delProjectile : deleteProjectiles) {
		projectiles.remove(delProjectile);
		space->removeDynamicActor(delProjectile);
		delete delProjectile; // Se destruye el proyectil.
	}
	deleteProjectiles.clear();

	// Información
	/*cout << "Killed Enemies: " << killedEnemies 
		 << " Current enemies: " << enemies.size() 
		 << " Shoots: " << projectiles.size() << endl;*/
}

/*
	Método encargado de calcular el scrollX
	en base a la posición del jugador.
*/
void GameLayer::calculateScroll() {
	// Límite izquierda
	if (player->x > WIDTH * 0.3) {
		if (player->x - scrollX < WIDTH * 0.3) {
			scrollX = player->x - WIDTH * 0.3;
		}
	}
	
	// Límite derecha
	if (player->x < mapWidth - WIDTH * 0.3) {
		if (player->x - scrollX > WIDTH * 0.7) {
			scrollX = player->x - WIDTH * 0.7;
		}
	}
}


void GameLayer::draw() {
	// Calculamos el scroll
	calculateScroll();

	background->draw();

	// Dibujamos los tiles
	for (auto const& tile : tiles) {
		tile->draw(scrollX);
	}

	player->draw(scrollX);
	// Dibujamos los enemigos
	for (auto const& enemy : enemies) {
		enemy->draw(scrollX);
	}
	// Dibujamos los proyectiles
	for (auto const& projectile : projectiles) {
		projectile->draw(scrollX);
	}

	textPoints->draw();
	backgroundPoints->draw();

	SDL_RenderPresent(game->renderer); // Renderiza el juego
}

/*
	Método para cargar el mapa a partir del fichero
	de nombre pasado como parámetro.
*/
void GameLayer::loadMap(string name) {
	char character;
	string line;
	ifstream streamFile(name.c_str()); // No es una función, es el constructor de ifstream
	if (!streamFile.is_open()) {
		cout << "Falla abrir el fichero de mapa" << endl;
		return;
	}
	else {
		// Por línea
		for (int i = 0; getline(streamFile, line); i++) {
			istringstream streamLine(line);
			mapWidth = line.length() * 40; // Ancho del mapa en pixels
			// Por carácter (en cada línea)
			for (int j = 0; !streamLine.eof(); j++) {
				streamLine >> character; // Leer character 
				cout << character;
				float x = 40 / 2 + j * 40; // x central
				float y = 32 + i * 32; // y suelo
				loadMapObject(character, x, y);
			}

			cout << character << endl;
		}
	}
	streamFile.close();
}

/*
	Recibe un caracter y una posición en el suelo de dicho caracter
	y crea el objeto correspondiente y lo añade al motor de físicas.
*/
void GameLayer::loadMapObject(char character, float x, float y)
{
	switch (character) {
		case '1': {
			player = new Player(x, y, game);
			// modificación para empezar a contar desde el suelo.
			player->y = player->y - player->height / 2;
			space->addDynamicActor(player);
			break;
		}
		case '#': {
			Tile* tile = new Tile("res/bloque_tierra.png", x, y, game);
			// modificación para empezar a contar desde el suelo.
			tile->y = tile->y - tile->height / 2;
			tiles.push_back(tile);
			space->addStaticActor(tile);
			break;
		}
		case 'E': {
			Enemy* enemy = new Enemy(x, y, game);
			enemy->y = enemy->y - enemy->height / 2;
			enemies.push_back(enemy);
			space->addDynamicActor(enemy);
			break;
		}
	}
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

// Molaría esto
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

