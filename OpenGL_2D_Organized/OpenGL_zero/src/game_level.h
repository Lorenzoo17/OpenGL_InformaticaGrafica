#ifndef GAME_LEVEL
#define GAME_LEVEL

#include <vector>
#include "game_object.h"

// Definisce elementi di scena da renderizzare come pareti ed oggetti statici
class GameLevel {
public:
	std::vector<GameObject> walls = std::vector<GameObject>(); // vettore ralativo alle pareti

	GameLevel(); // costruttore
	void Draw();
	void Load(std::vector<GameObject> w); // per il caricamento delle varie componenti del livello (aggiungere man mano)
	void init(); // Vengono inizializzate le strutture direttamente in GameLevel
};

#endif // !GAME_LEVEL

