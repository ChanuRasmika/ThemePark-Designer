#pragma once
#include "core/Attraction.h"
#include <memory>
#include <vector>
#include <map>

class AttractionManager {
private:
    std::map<int, std::unique_ptr<Attraction>> attractions;
    
public:
    void registerAttraction(int id, std::unique_ptr<Attraction> attraction);
    void showMenu();
    std::vector<int> getUserSelection();
    bool loadAttractions(const std::vector<int>& selectedIds);
    void renderAttractions(unsigned int shaderProgram, float* model, float* view, float* projection);
    void cleanup();
};