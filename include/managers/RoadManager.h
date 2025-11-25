#pragma once
#include "attractions/Road.h"
#include <memory>
#include <vector>
#include <string>

class RoadManager {
private:
    std::vector<std::string> roadTypes = {"Brick", "Stone", "Paving"};
    std::vector<std::string> texturePaths = {
        "assets/road-textures/Bricks075A_1K-JPG_Color.jpg",
        "assets/road-textures/PavingStones138_1K-JPG_Color.jpg", 
        "assets/road-textures/PavingStones149.jpg"
    };

public:
    std::unique_ptr<Road> createRoadSystem();
    void showRoadMenu();
    int getRoadTypeChoice();
    void getRoadLayout(Road* road);
};