#pragma once
#include "core/Attraction.h"
#include <vector>

class GameStall : public Attraction {
private:
    std::vector<float> vertices;
    unsigned int VAO, VBO, texture, signTexture;
    int vertexCount;

public:
    bool load() override;
    void render(unsigned int shaderProgram, float* model, float* view, float* projection) override;
    void cleanup() override;
    std::string getName() const override { return "Game Stall"; }
    std::string getDescription() const override { return "Fun carnival games and prizes"; }
};