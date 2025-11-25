#pragma once
#include "core/Attraction.h"
#include "utils/RoadGenerator.h"
#include <vector>

class Road : public Attraction {
private:
    unsigned int VAO, VBO, EBO, texture;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::string texturePath;
    std::string roadType;

public:
    Road(const std::string& type, const std::string& texPath);
    bool load() override;
    void render(unsigned int shaderProgram, float* model, float* view, float* projection) override;
    void cleanup() override;
    std::string getName() const override { return roadType + " Road"; }
    std::string getDescription() const override { return "Road system using " + roadType; }
    
    void addStraightRoad(int x0, int y0, int x1, int y1);
    void addCircularRoad(int centerX, int centerY, int radius);
};