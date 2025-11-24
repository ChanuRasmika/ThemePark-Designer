#pragma once
#include "core/Attraction.h"
#include <vector>

class MerryGoRound : public Attraction {
private:
    std::vector<float> vertices;
    unsigned int VAO, VBO, texture;
    int vertexCount;

public:
    bool load() override;
    void render(unsigned int shaderProgram, float* model, float* view, float* projection) override;
    void cleanup() override;
    std::string getName() const override { return "Merry Go Round"; }
    std::string getDescription() const override { return "Classic spinning carousel with horses"; }
};