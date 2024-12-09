#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "game_level.h"

class Game {
public:
	unsigned int Width, Height; // Dimensioni dello schermo
	bool Keys[1024];
	GameLevel* Level;

	Game(unsigned int Width, unsigned int Height);

	void Init(); // Metodo dove si inizializzano variabili, shader, texture ecc... e viene eseguito una singola volta
	void Update(float deltaTime); // Metodo dove vengono eseguiti i comportamenti che devono essere aggiornati ad ogni frame
	void Render(); // Metodo dove si eseguono esclusivamente i rendering
	void ProcessInput(float deltaTime); // Metodo dove si gestiscono gli input

};

#endif // !GAME_H

