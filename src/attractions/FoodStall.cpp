#include "attractions/FoodStall.h"
#include <glad/glad.h>
#include <iostream>
#include "utils/stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

extern unsigned int loadTexture(const char* path);
extern bool loadModel(const char* path, std::vector<float>& vertices);

bool FoodStall::load() {
    if (!loadModel("assets/food stalls/source/Untitled.obj", vertices)) {
        std::cout << "Failed to load food stall model" << std::endl;
        return false;
    }
    
    texture = loadTexture("assets/food stalls/textures/redstrip_texture.png");
    if (texture == 0) {
        std::cout << "Failed to load food stall texture" << std::endl;
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
    std::cout << "Food stall loaded successfully with " << vertexCount << " vertices" << std::endl;
    return true;
}

void FoodStall::render(unsigned int shaderProgram, float* model, float* view, float* projection) {
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, model);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, projection);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void FoodStall::cleanup() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &texture);
}