#include "game.h"
#include "resource_manager.h"
#include "game_object.h"
#include "Camera.h"

void CheckDistance(GameObject* o1, GameObject* o2);

Camera* camera;
glm::vec3 INITIAL_CAMERA_POSITION(0.0f, 0.0f, 5.0f);

GameObject* Player;
const glm::vec3 INITIAL_PLAYER_POSITION(0.0f, 0.0f, 3.0f);
const float PLAYER_INITIAL_VELOCITY(2.5f);

GameObject* Box;

bool interactPressed; // booleano che va a true se si preme E (test)

// rendering della mappa tramite classe apposita GameLevel
// rendering di scatole e interazione in process input : quasi, migliorare 

Game::Game(unsigned int width, unsigned int height) : Width(width), Height(height) {

};

void Game::Init(){
	// Caricamento degli shader
	ResourceManager::LoadShader("assets/shaders/shader.vs", "assets/shaders/shader.fs", nullptr, "base");

	// Caricamento delle texture
	ResourceManager::LoadTexture("assets/textures/container.jpg", false, "cassa");
	ResourceManager::LoadTexture("assets/textures/robot.png", true, "robot");
	ResourceManager::LoadTexture("assets/textures/wall.png", true, "parete");

	// Si definisce la matrice di proiezione 
	glm::mat4 projection = glm::mat4(1.0f);
	float cameraDistance = 3.0f;
	float aspect = (float)this->Width / this->Height;
	projection = glm::ortho(-aspect * cameraDistance, aspect * cameraDistance, -1.0f * cameraDistance, 1.0f * cameraDistance, 0.1f, 100.0f);
	
	// Si assegna la matrice di proiezione agli shader e si settano le texture
	ResourceManager::GetShader("base").Use();
	ResourceManager::GetShader("base").SetMatrix4("projection", projection);
	ResourceManager::GetShader("base").SetInteger("texture1", 0); // dico che con GL_TEXTURE0 mi sto riferendeo a sampler2D texture1

	// Si creano i gameobject e si assegna lo shader corretto 
	// Creazione del player con texture robot, velocita 2.5 e direzione vec3(0)
	Player = new GameObject(INITIAL_PLAYER_POSITION, glm::vec3(1.0f), ResourceManager::GetTexture("robot"), PLAYER_INITIAL_VELOCITY, glm::vec3(1.0f), glm::vec3(0.0f));
	Player->SetShader(ResourceManager::GetShader("base")); // Si assegna anche uno shader al player

	// Creazione oggetto generico
	Box = new GameObject(glm::vec3(3.0f, 0.0f, 0.0f), glm::vec3(0.5f), ResourceManager::GetTexture("cassa"));
	Box->SetShader(ResourceManager::GetShader("base"));

	this->Level = new GameLevel(); // Si inizializza il livello (crezione pareti ed oggetti statici)

	// Settaggio camera
	camera = new Camera(INITIAL_CAMERA_POSITION);
}

void Game::Render() {
	if (!Box->Destroyed)
		Box->Draw();

	Level->Draw(); // renderizzo componenti statici del livello

	// Va renderizzato dopo per il blending (vedi glEnable(GL_BLEND) nel main)
	Player->Draw(); // Rendering del player, si attiva in automatico lo shader giusto e viene assegnata la texture corretta
}

void Game::Update(float deltaTime) {
	// Camera follow qui

	// aggiorno la matrice di vista e la assegno agli shader, da mettere in update se camera cambia posizione/rotazione
	ResourceManager::GetShader("base").SetMatrix4("view", camera->GetViewMatrix());

	CheckDistance(Player, Box);
}

void Game::ProcessInput(float deltaTime) {
	Player->MoveDirection = glm::vec3(0.0f); // resetto per evitare che continui a muoversi senza il mio input

	if (this->Keys[GLFW_KEY_W])
		Player->MoveDirection += glm::vec3(0.0f, 1.0f, 0.0f);
	if (this->Keys[GLFW_KEY_S]) {
		Player->MoveDirection += glm::vec3(0.0f, -1.0f, 0.0f);
	}
	if (this->Keys[GLFW_KEY_A]) {
		Player->MoveDirection += glm::vec3(-1.0f, 0.0f, 0.0f);
	}
	if (this->Keys[GLFW_KEY_D]) {
		Player->MoveDirection += glm::vec3(1.0f, 0.0f, 0.0f);
	}

	if (glm::length(Player->MoveDirection) > 0.0f) { // Se il player si è mosso (modulo di moveDirection > 0)
		// Normalizzo la direzione
		Player->MoveDirection = Player->MoveDirection / sqrt(glm::dot(Player->MoveDirection, Player->MoveDirection));
	}

	Player->Position += Player->MoveDirection * Player->Speed * deltaTime; // Posizione del player = direzione * speed * deltatime

	interactPressed = this->Keys[GLFW_KEY_E]; // Per interazioni (test)
}

void CheckDistance(GameObject* o1, GameObject* o2) {
	if (sqrt(pow((o2->Position.x - o1->Position.x), 2) + pow((o2->Position.y - o1->Position.y), 2) < 0.3f)) {
		if (interactPressed) {
			o2->Destroyed = true; // setto l'oggetto 2 destroyed, non viene renderizzato (test)
		}
	}
}
