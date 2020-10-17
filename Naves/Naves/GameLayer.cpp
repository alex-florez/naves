#include "GameLayer.h"


void markEnemyForDelete(Enemy* enemy, list<Enemy*>& deleteList);
void markProjectileForDelete(Projectile* projectile, list<Projectile*>& deleteList);
void markTileForDelete(Tile* tile, list<Tile*>& deleteList);

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
	pause = true;
	message = new Actor("res/mensaje_como_jugar.png", WIDTH*0.5, HEIGHT*0.5,
		WIDTH, HEIGHT, game);
	doorTeleports = new DoorTeleports();

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
	scrollY = 0; // Arriba del todo

	// limpiar los tiles
	tiles.clear();

	space = new Space(1); // Instanciamos el motor de físicas con gravedad.

	// Destruir posibles objetos existentes
	delete player;
	destroyEnemies();
	destroyProjectiles();

	background = new Background("res/fondo_2.png", WIDTH * 0.5, HEIGHT * 0.5, -1, game);
	
	// Teletransportes
	doorTeleports->clearTeleports();

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

	// Botones de la interfaz
	buttonJump = new Actor("res/boton_salto.png", WIDTH*0.9, HEIGHT * 0.55, 100, 100, game);
	buttonShoot = new Actor("res/boton_disparo.png", WIDTH * 0.75, HEIGHT * 0.83, 100, 100, game);
	// Pad
	pad = new Pad(WIDTH * 0.15, HEIGHT * 0.80, game);

	// cargamos el mapa a partir del fichero
	loadMap("res/" + to_string(game->currentLevel) + ".txt");
}

void GameLayer::processControls() {
	// obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		// Cambio automático del input
		switch (event.type)
		{
		case SDL_QUIT: // Se hace click en la X para cerrar la ventana
			game->loopActive = false;
			break;
		case SDL_KEYDOWN:
			game->input = GameInputType::KEYBOARD;
			break;
		case SDL_MOUSEBUTTONDOWN:
			game->input = GameInputType::MOUSE;
			break;
		}

		if (game->input == GameInputType::KEYBOARD) {
			keysToControls(event);
		}
		if (game->input == GameInputType::MOUSE) {
			mouseToControls(event);
		}
	}
	// procesar controles
	// Despausar el juego
	if (controlContinue) {
		pause = false;
		controlContinue = false;
	}

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
	
}

void GameLayer::mouseToControls(SDL_Event event) {
	// Modificación de coordenadas por posible escalado
	float motionX = event.motion.x / game->scaleLower;
	float motionY = event.motion.y / game->scaleLower;

	// Cada vez que el usuario hace click
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		controlContinue = true;
		if (pad->containsPoint(motionX, motionY)) {
			pad->clicked = true;
			controlMoveX = pad->getOrientationX(motionX);
		}
		if (buttonShoot->containsPoint(motionX, motionY)) {
			controlShoot = true;
		}
		if (buttonJump->containsPoint(motionX, motionY)) {
			controlMoveY = -1;
		}
	}
	// Cada vez que se mueve
	if (event.type == SDL_MOUSEMOTION) {
		if (pad->clicked && pad->containsPoint(motionX, motionY)) { // Cursor dentro del pad y además se ha hecho click
			controlMoveX = pad->getOrientationX(motionX);
			// Rango de -20 a 20 lo consideramos como cero
			if (controlMoveX > -20 && controlMoveX < 20) {
				controlMoveX = 0;
			}
		} else{
			pad->clicked = false; // han sacado el ratón del pad
			controlMoveX = 0;
		}


		if (!buttonShoot->containsPoint(motionX, motionY)) { // Ratón se mueve fuera del botón de disparo
			controlShoot = false;
		}
		if (!buttonJump->containsPoint(motionX, motionY)) {
			controlMoveY = 0;
		}
	}

	// Cada vez que se levanta el click
	if (event.type == SDL_MOUSEBUTTONUP) {
		if (pad->containsPoint(motionX, motionY)) {
			pad->clicked = false;
			controlMoveX = 0;
		}
		if (buttonShoot->containsPoint(motionX, motionY)) {
			controlShoot = false;
		}
		if (buttonJump->containsPoint(motionX, motionY)) {
			controlMoveY = 0;
		}
	}
}

/**
* Método encargado de actualizar todos los elementos del juego.
**/
void GameLayer::update() {

	list<Enemy*> deleteEnemies; // Enemigos a eliminar
	list<Projectile*> deleteProjectiles; // Proyectiles a eliminar
	list<Tile*> deleteTiles; // Tiles a eliminar

	if (pause) {
		return;
	}

	// Comprobamos si el jugador se cae del mapa
	if ((player->y - player->height / 2) > HEIGHT) {
		init();
		return;
	}

	// Comprobamos si el jugador colisiona con el elemento de final de nivel
	if (cup->isOverlap(player)) {
		game->currentLevel++;
		if (game->currentLevel > game->finalLevel) {
			game->currentLevel = 0;
		}
		message = new Actor("res/mensaje_ganar.png", WIDTH*0.5, HEIGHT*0.5, WIDTH, HEIGHT, game);
		pause = true;
		init();
		return;
	}

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
			// Comprobamos si el player ha saltado encima del enemigo
			if (player->isOver(enemy)) {
				player->audioJumpOverEnemy->play(); // Reproducir sonido de aplastamiento
				markEnemyForDelete(enemy, deleteEnemies);
			}
			else {
				player->loseLife();
				if (player->lifes <= 0) { // Jugador sin vidas
					init();
					return; // Se reinicia el juego.
				}
			}
		}
	}
	// Actualizamos los proyectiles
	for (auto const& projectile : projectiles) {
		projectile->update();
		// Proyectil a la derecha de la pantalla o proyectil sin velocidad (impacto con algún elemnto estático)
		if (!projectile->isInRender(scrollX, scrollY) || projectile->vx == 0) {
			markProjectileForDelete(projectile, deleteProjectiles);
		}

		// Comprobamos colisiones de proyectiles con tiles 
		for (auto const& tile : tiles) {
			if (tile->isOverlap(projectile)) {
				if (tile->isDestroyable()) {// Si el tile es destruible, se elimina
					markTileForDelete(tile, deleteTiles);
					markProjectileForDelete(projectile, deleteProjectiles);
				}	
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

	// Actualizamos los tiles
	for (auto const& tile : tiles) {
		tile->playerIsOver = player->isOver(tile);  // Jugador encima del tile
		tile->update(); // Actualizar ticks del tile
		if (tile->willDestroy()) { // Si el tile se va a destruir...
			markTileForDelete(tile, deleteTiles); // Eliminarlo
		}
	}


	// Colisiones del jugador con una puerta
	for (auto const& door : doorTeleports->doors) {
		if (player->isOverlap(door)) {
			door->teleport(player); // Teletransportar al jugador
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

	// Eliminar los tiles marcados
	for (auto const& tile : deleteTiles) {
		tiles.remove(tile);
		delete tile;
	}
	deleteTiles.clear();

	// Información
	/*cout << "Killed Enemies: " << killedEnemies 
		 << " Current enemies: " << enemies.size() 
		 << " Shoots: " << projectiles.size() << endl;*/
}

/*
	Método encargado de calcular el scrollX y el
	scrollY en base a la posición del jugador.
*/
void GameLayer::calculateScroll() {
	// ScrollX
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

	// ScrollY
	// Límite arriba
	if (player->y > HEIGHT * 0.3) {
		if (player->y - scrollY < HEIGHT * 0.3) {
			scrollY = player->y - (HEIGHT * 0.3);
		}
	}


	// Límite abajo
	if (player->y < mapHeight - HEIGHT * 0.3) {
		if (player->y - scrollY > HEIGHT * 0.7) {
			scrollY = player->y - (HEIGHT * 0.7);
		}
	}
}


void GameLayer::draw() {
	// Calculamos el scroll
	calculateScroll();

	background->draw();

	// Dibujamos los tiles
	for (auto const& tile : tiles) {
		tile->draw(scrollX, scrollY);
	}
	// Dibujamos al jugador
	player->draw(scrollX, scrollY);

	//Dibujamos la copa
	cup->draw(scrollX, scrollY);

	// Dibujamos los enemigos
	for (auto const& enemy : enemies) {
		enemy->draw(scrollX, scrollY);
	}
	// Dibujamos los proyectiles
	for (auto const& projectile : projectiles) {
		projectile->draw(scrollX, scrollY);
	}

	// Dibujamos los teletransportes
	doorTeleports->draw(scrollX, scrollY);

	textPoints->draw();
	backgroundPoints->draw();
	// HUD
	if (game->input == GameInputType::MOUSE) { // Dibujar el HUD solo si el tipo de entrada es el mouse
		buttonJump->draw(); // NO TIENEN SCROLL, POSISICIÓN FIJA
		buttonShoot->draw();
		pad->draw(); // Sin scroll - posición fija
	}

	if (pause) {
		message->draw();
	}
	

	SDL_RenderPresent(game->renderer); // Renderiza el juego
}

/*
	Método para cargar el mapa a partir del fichero
	de nombre pasado como parámetro.
*/
void GameLayer::loadMap(string name) {
	char character;
	int totalLines = 0;
	string line;
	ifstream streamFile(name.c_str()); // No es una función, es el constructor de ifstream
	if (!streamFile.is_open()) {
		cout << "Falla abrir el fichero de mapa" << endl;
		return;
	}
	else {
		// Por línea
		for (int i = 0; getline(streamFile, line); i++) {
			totalLines++;
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
		mapHeight = totalLines * 32;
	}

	scrollY = mapHeight - HEIGHT;
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
		case 'C': {
			cup = new Tile("res/copa.png", x, y, game);
			cup->y = cup->y - cup->height / 2;
			break;
		}
		case 'W': {
			Tile* crushableTile = new CrushableTile("res/bloque_fondo_muro.png", 20, x, y, game);
			crushableTile->y = crushableTile->y - crushableTile->height / 2;
			tiles.push_back(crushableTile);
			space->addStaticActor(crushableTile);
			break;
		}

		case 'U': {
			Tile* destroyableTile = new DestroyableTile("res/caja_madera.png", x, y, game);
			destroyableTile->y = destroyableTile->y - destroyableTile->height / 2;
			tiles.push_back(destroyableTile);
			space->addStaticActor(destroyableTile);
			break;
		}
		case '4':
		case '5':
		case '7':
		case '8':
		case '9': {
			Door* door = new Door(character, x, y, game);
			door->y = door->y - door->height / 2;
			space->addStaticActor(door);
			doorTeleports->addDoor(door);
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


void markTileForDelete(Tile* tile, list<Tile*>& deleteList) {
	bool inList = std::find(deleteList.begin(),
		deleteList.end(),
		tile) != deleteList.end();
	if (!inList) {
		deleteList.push_back(tile);
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

