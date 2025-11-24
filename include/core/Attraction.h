#pragma once
#include <string>
#include <vector>

class Attraction {
public:
    virtual ~Attraction() = default;
    virtual bool load() = 0;
    virtual void render(unsigned int shaderProgram, float* model, float* view, float* projection) = 0;
    virtual void cleanup() = 0;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
};