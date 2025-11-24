#include "managers/AttractionManager.h"
#include <iostream>
#include <sstream>

void AttractionManager::registerAttraction(int id, std::unique_ptr<Attraction> attraction) {
    attractions[id] = std::move(attraction);
}

void AttractionManager::showMenu() {
    std::cout << "\n=== Theme Park Attractions ===" << std::endl;
    std::cout << "Available attractions:" << std::endl;
    
    for (const auto& pair : attractions) {
        std::cout << pair.first << ". " << pair.second->getName() 
                  << " - " << pair.second->getDescription() << std::endl;
    }
    
    std::cout << "\nEnter attraction numbers (space-separated, e.g., '1 2'): ";
}

std::vector<int> AttractionManager::getUserSelection() {
    std::vector<int> selected;
    std::string input;
    std::getline(std::cin, input);
    
    std::istringstream iss(input);
    int id;
    while (iss >> id) {
        if (attractions.find(id) != attractions.end()) {
            selected.push_back(id);
        } else {
            std::cout << "Warning: Attraction " << id << " not found!" << std::endl;
        }
    }
    
    return selected;
}

bool AttractionManager::loadAttractions(const std::vector<int>& selectedIds) {
    for (int id : selectedIds) {
        std::cout << "Loading " << attractions[id]->getName() << "..." << std::endl;
        if (!attractions[id]->load()) {
            std::cout << "Failed to load " << attractions[id]->getName() << std::endl;
            return false;
        }
    }
    return true;
}

void AttractionManager::renderAttractions(unsigned int shaderProgram, float* model, float* view, float* projection) {
    for (const auto& pair : attractions) {
        pair.second->render(shaderProgram, model, view, projection);
    }
}

void AttractionManager::cleanup() {
    for (const auto& pair : attractions) {
        pair.second->cleanup();
    }
}