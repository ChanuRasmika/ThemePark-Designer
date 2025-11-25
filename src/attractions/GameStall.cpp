#include "attractions/GameStall.h"
#include <glad/glad.h>
#include <iostream>
#include "utils/stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

extern unsigned int loadTexture(const char* path);
extern bool loadModel(const char* path, std::vector<float>& vertices);

bool GameStall::load() {
    if (!loadModel("assets/gamestalls/source/gamestalls.obj", vertices)) {
        std::cout << "Failed to load game stall model" << std::endl;
        return false;
    }
    
    texture = loadTexture("assets/food stalls/textures/yellowstrip_texture.png");
    signTexture = loadTexture("assets/gamestalls/textures/Untitled304_20240324093838.png");
    if (texture == 0 || signTexture == 0) {
        std::cout << "Failed to load game stall textures" << std::endl;
        return false;
    }
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    vertexCount = vertices.size() / 5;
    std::cout << "Game stall loaded successfully with " << vertexCount << " vertices" << std::endl;
    return true;
}

void GameStall::render(unsigned int shaderProgram, float* model, float* view, float* projection) {
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, model);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, projection);
    
    // Render with yellow strip texture first
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount/2);
    
    // Render sign part with name texture
    glBindTexture(GL_TEXTURE_2D, signTexture);
    glDrawArrays(GL_TRIANGLES, vertexCount/2, vertexCount/2);
}

void GameStall::cleanup() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &texture);
    glDeleteTextures(1, &signTexture);
}