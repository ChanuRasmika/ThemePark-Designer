#include "managers/RoadManager.h"
#include <iostream>

void RoadManager::showRoadMenu() {
    std::cout << "\n=== Road System ===" << std::endl;
    std::cout << "Available road types:" << std::endl;
    for (size_t i = 0; i < roadTypes.size(); i++) {
        std::cout << (i + 1) << ". " << roadTypes[i] << " Road" << std::endl;
    }
}

int RoadManager::getRoadTypeChoice() {
    int choice;
    std::cout << "Select road type (1-" << roadTypes.size() << "): ";
    std::cin >> choice;
    
    if (choice < 1 || choice > (int)roadTypes.size()) {
        std::cout << "Invalid choice. Using Brick road." << std::endl;
        return 0;
    }
    return choice - 1;
}

void RoadManager::getRoadLayout(Road* road) {
    std::cout << "\n=== Theme Park Road System ===" << std::endl;
    std::cout << "Park boundaries: 100x100 units (-50 to +50)" << std::endl;
    std::cout << "Merry-go-round location: (-20, -20)" << std::endl;
    std::cout << "\nPre-designed road options:" << std::endl;
    std::cout << "1. Main entrance road" << std::endl;
    std::cout << "2. Circular path around merry-go-round" << std::endl;
    std::cout << "3. Cross paths" << std::endl;
    std::cout << "4. Custom road" << std::endl;
    std::cout << "5. Finish" << std::endl;
    
    int choice;
    while (true) {
        std::cout << "Choice (1-5): ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input!" << std::endl;
            continue;
        }
        
        if (choice == 1) {
            road->addStraightRoad(0, -50, 0, 0);
            std::cout << "Main entrance road added!" << std::endl;
        }
        else if (choice == 2) {
            road->addCircularRoad(-20, -20, 15);
            std::cout << "Circular path around merry-go-round added!" << std::endl;
        }
        else if (choice == 3) {
            road->addStraightRoad(-50, 0, 50, 0);
            road->addStraightRoad(0, -50, 0, 50);
            std::cout << "Cross paths added!" << std::endl;
        }
        else if (choice == 4) {
            int x0, y0, x1, y1;
            std::cout << "Enter start (-50 to 50): ";
            std::cin >> x0 >> y0;
            std::cout << "Enter end (-50 to 50): ";
            std::cin >> x1 >> y1;
            road->addStraightRoad(x0, y0, x1, y1);
            std::cout << "Custom road added!" << std::endl;
        }
        else if (choice == 5) {
            break;
        }
        else {
            std::cout << "Invalid choice! Enter 1-5." << std::endl;
        }
    }
}

std::unique_ptr<Road> RoadManager::createRoadSystem() {
    char addRoads;
    std::cout << "Do you want to add roads to your theme park? (y/n): ";
    std::cin >> addRoads;
    
    if (addRoads != 'y' && addRoads != 'Y') {
        return nullptr;
    }
    
    showRoadMenu();
    int typeIndex = getRoadTypeChoice();
    
    auto road = std::make_unique<Road>(roadTypes[typeIndex], texturePaths[typeIndex]);
    getRoadLayout(road.get());
    
    return road;
}