#include "game_level.h"
#include "resource_manager.h"

int numPositions = 7;
glm::vec3 wallBricksPositions[] = {
	glm::vec3(-3.0f, -3.0f, 0.0f),
	glm::vec3(-3.0f, -2.0f, 0.0f),
	glm::vec3(-3.0f, -1.0f, 0.0f),
	glm::vec3(-3.0f, 0.0f, 0.0f),
	glm::vec3(-3.0f, 1.0f, 0.0f),
	glm::vec3(-3.0f, 2.0f, 0.0f),
	glm::vec3(-3.0f, 3.0f, 0.0f)
};

GameLevel::GameLevel() {
	init(); // Di default si esegue GameLevel::init, per una disposizione di base
}

void GameLevel::Load(std::vector<GameObject> w) {
	this->walls = w;
}

void GameLevel::Draw() {
	if (this->walls.size() > 0) {
		for (GameObject& w : this->walls) {
			w.Draw(); // Si richiama direttamente il metodo draw dei gameobject
		}
	}
}

void GameLevel::init() {
	for (int i = 0; i < numPositions; i++) {
		GameObject brick(wallBricksPositions[i], glm::vec3(1.0f, 1.0f, 1.0f), ResourceManager::GetTexture("parete"));
		brick.Rotation = glm::radians(0.0f);
		brick.SetShader(ResourceManager::GetShader("base"));
		this->walls.push_back(brick);
	}
}
