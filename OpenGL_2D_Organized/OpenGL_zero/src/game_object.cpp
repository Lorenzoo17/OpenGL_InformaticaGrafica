#include "game_object.h"


GameObject::GameObject() : Position(0.0f, 0.0f, 0.0f), Size(1.0f, 1.0f, 1.0f), MoveDirection(0.0f), Color(1.0f), Rotation(0.0f), Sprite(), Speed(0.0f), IsSolid(false), Destroyed(false){
    initRenderData();
}

GameObject::GameObject(glm::vec3 pos, glm::vec3 size, Texture2D sprite, float speed, glm::vec3 color, glm::vec3 moveDirection)
    : Position(pos), Size(size), MoveDirection(moveDirection), Color(color), Rotation(0.0f), Sprite(sprite), Speed(speed), IsSolid(false), Destroyed(false){
    initRenderData();
}

void GameObject::SetShader(Shader shader) {
    ObjectShader = shader;
}

void GameObject::initRenderData() {
    // configurazione VAO/VBO
    unsigned int VBO;
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f
    };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(this->quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void GameObject::Draw()
{
    this->ObjectShader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(this->Position));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    // Le traslazioni sono per avere come origine il punto il alto a sinistra
    // model = glm::translate(model, glm::vec3(0.5f * this->Size.x, 0.5f * this->Size.y, 0.0f)); // move origin of rotation to center of quad
    model = glm::rotate(model, this->Rotation, glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    // model = glm::translate(model, glm::vec3(-0.5f * this->Size.x, -0.5f * this->Size.y, 0.0f)); // move origin back

    model = glm::scale(model, glm::vec3(this->Size)); // last scale

    this->ObjectShader.SetMatrix4("model", model); // aggiorno model matrix per l'oggetto che sto attualmente renderizzando

    // render textured quad
    // this->shader.SetVector3f("spriteColor", color);

    glActiveTexture(GL_TEXTURE0); // al posto di assegnare dalla uniform si fa direttamente questo 
    this->Sprite.Bind(); // poi si fa il bind

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}