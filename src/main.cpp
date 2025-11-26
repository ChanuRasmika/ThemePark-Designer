#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <GLFW/glfw3.h>
#include <cmath>
#include <algorithm>
#include <random>
#include <ctime>
#include <set>
#include "midpoint_circle.h"
#include "bresenham.h"
#include "renderer.h"
#include "simple3d.h"
#include "stall.h"
#include "merryGoRound.h"
#include "ferrisWheel.h"
#include "path.h"
#include "pond.h"
#include "imgui_setup.h"




struct Element {
    int x, y, size;
    int type; // 0=merry, 1=ferris, 2=stall, 3=pond
};

bool is3D = false;

// Toggle for auto-generation (set to false for manual input)
bool autoGenerate = false; // Change to true for auto-generation

Simple3D renderer3D;

bool checkOverlap(const std::vector<Element>& elements, int x, int y, int size) {
    for (const auto& elem : elements) {
        int dist = sqrt((x - elem.x) * (x - elem.x) + (y - elem.y) * (y - elem.y));
        if (dist < size + elem.size + 30) return true;
    }
    return x < 50 || y < 50 || x > 550 || y > 450;
}



void placeElements(std::vector<MerryGoRound>& merryGoRounds,
                  std::vector<FerrisWheel>& ferrisWheels,
                  std::vector<Stall>& stalls,
                  std::vector<Pond>& ponds,
                  std::vector<Path>& paths,
                  int numMerry, int numFerris, int numStalls, int numPonds) {
    std::vector<Element> allElements;
    
    int merrySize = 30, ferrisSize = 40, stallSize = 20, pondSize = 25;
    
    // Calculate minimum grid size based on total elements
    int totalElements = numMerry + numFerris + numStalls + numPonds;
    int gridSize = std::max(1, (int)std::ceil(std::sqrt(totalElements)));
    const int cellSize = 150;
    const int startX = 150, startY = 150;
    
    std::cout << "Creating " << gridSize << "x" << gridSize << " grid for " << totalElements << " elements" << std::endl;
    
    std::vector<std::pair<int, int>> gridPositions;
    for (int row = 0; row < gridSize; ++row) {
        for (int col = 0; col < gridSize; ++col) {
            int centerX = startX + col * cellSize;
            int centerY = startY + row * cellSize;
            gridPositions.push_back({centerX, centerY});
        }
    }
    
    std::srand(std::time(nullptr));
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(gridPositions.begin(), gridPositions.end(), g);
    
    int posIndex = 0;
    
    // Place elements at grid positions
    for (int i = 0; i < numMerry && posIndex < gridPositions.size(); ++i, ++posIndex) {
        int x = gridPositions[posIndex].first;
        int y = gridPositions[posIndex].second;
        merryGoRounds.push_back({x, y, merrySize});
        allElements.push_back({x, y, merrySize, 0});
    }
    
    for (int i = 0; i < numFerris && posIndex < gridPositions.size(); ++i, ++posIndex) {
        int x = gridPositions[posIndex].first;
        int y = gridPositions[posIndex].second;
        ferrisWheels.push_back({x, y, ferrisSize});
        allElements.push_back({x, y, ferrisSize, 1});
    }
    
    for (int i = 0; i < numStalls && posIndex < gridPositions.size(); ++i, ++posIndex) {
        int x = gridPositions[posIndex].first;
        int y = gridPositions[posIndex].second;
        stalls.push_back(Stall(x, y, stallSize, stallSize));
        allElements.push_back({x, y, stallSize, 2});
    }
    
    for (int i = 0; i < numPonds && posIndex < gridPositions.size(); ++i, ++posIndex) {
        int x = gridPositions[posIndex].first;
        int y = gridPositions[posIndex].second;
        ponds.push_back({x, y, pondSize});
        allElements.push_back({x, y, pondSize, 3});
    }
    
    // Generate grid lines as paths (4 horizontal + 4 vertical lines)
    // Horizontal grid lines
    for (int i = 0; i <= gridSize; ++i) {
        int y = startY - cellSize/2 + i * cellSize;
        paths.push_back(Path(startX - cellSize/2, y, startX - cellSize/2 + gridSize * cellSize, y));
    }
    // Vertical grid lines
    for (int i = 0; i <= gridSize; ++i) {
        int x = startX - cellSize/2 + i * cellSize;
        paths.push_back(Path(x, startY - cellSize/2, x, startY - cellSize/2 + gridSize * cellSize));
    }
    
    std::cout << "Generated " << paths.size() << " paths connecting " << allElements.size() << " elements." << std::endl;
}

int main() {
    std::vector<MerryGoRound> merryGoRounds;
    std::vector<FerrisWheel> ferrisWheels;
    std::vector<Path> paths;
    std::vector<Stall> stalls;
    std::vector<Pond> ponds;
    bool elementsPlaced = false;
    int numMerry = 2, numFerris = 1, numStalls = 3, numPonds = 2;

    // Initialize GLFW
    if (!glfwInit()) return -1;
    
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Theme Park Builder", monitor, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    
    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    // Initialize renderers
    Renderer renderer;
    renderer.init();
    
    // Setup ImGui
    setupImGui(window);
    
    // Set up mouse callback for 3D camera
    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
        if (is3D) renderer3D.mouseCallback(x, y);
    });


    float lastTime = 0.0f;
    
    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        glfwPollEvents();
        
        // Handle ImGui input if elements not placed yet
        if (!elementsPlaced) {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            
            glClear(GL_COLOR_BUFFER_BIT);
            glViewport(0, 0, width, height);
            glDisable(GL_DEPTH_TEST);
            
            renderImGui();
            if (showInputWindow(numMerry, numFerris, numStalls, numPonds, autoGenerate)) {
                placeElements(merryGoRounds, ferrisWheels, stalls, ponds, paths, numMerry, numFerris, numStalls, numPonds);
                elementsPlaced = true;
            }
            glfwSwapBuffers(window);
            continue;
        }
        
        // Toggle between 2D and 3D
        static bool key3Pressed = false, key2Pressed = false;
        
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && !key3Pressed) {
            is3D = true;
            renderer3D.init();
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            int width, height;
            glfwGetWindowSize(window, &width, &height);
            glfwSetCursorPos(window, width/2.0, height/2.0);
            renderer3D.firstMouse = true;
            key3Pressed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_RELEASE) key3Pressed = false;
        
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && !key2Pressed) {
            is3D = false;
            renderer.init();
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            key2Pressed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE) key2Pressed = false;
        
        if (is3D) {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            
            renderer3D.processInput(window, deltaTime);
            renderer3D.render(width, height);
            
            // Draw 3D objects
            for (auto& merry : merryGoRounds) {
                renderer3D.drawCarousel(merry.x/10.0f, 0, merry.y/10.0f, merry.radius/10.0f);
            }
            
            for (auto& ferris : ferrisWheels) {
                renderer3D.drawFerrisWheel(ferris.x/10.0f, 0, ferris.y/10.0f, ferris.radius/10.0f);
            }
            
            for (auto& stall : stalls) {
                renderer3D.drawStall(stall.x/10.0f, 0, stall.y/10.0f, stall.width/10.0f, stall.height/10.0f);
            }
            
            for (auto& pond : ponds) {
                renderer3D.drawPond(pond.x/10.0f, 0, pond.y/10.0f, pond.radius/10.0f);
            }
            
            // Draw paths last (on top of everything)
            for (auto& path : paths) {
                renderer3D.drawPath(path.x1/10.0f, 0.01f, path.y1/10.0f, path.x2/10.0f, 0.01f, path.y2/10.0f, 1.5f, 1.0f, 1.0f, 0.0f);
            }
            
            glfwSwapBuffers(window);
            continue;
        }
        
        // 2D Mode rendering
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, 800.0, 600.0, 0.0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Render merry-go-rounds
        for (auto& merryGoRound : merryGoRounds) {
            std::vector<std::pair<int, int>> circlePoints;
            midpointCircle(merryGoRound.x, merryGoRound.y, merryGoRound.radius, circlePoints);
            renderer.render2D(circlePoints, 1.0f, 0.5f, 0.0f); // Orange
        }

        // Render ferris wheels
        for (auto& ferrisWheel : ferrisWheels) {
            std::vector<std::pair<int, int>> wheelPoints;
            midpointCircle(ferrisWheel.x, ferrisWheel.y, ferrisWheel.radius, wheelPoints);
            for (int i = 0; i < 8; i++) {
                double angle = i * 3.14159 / 4;
                int endX = ferrisWheel.x + ferrisWheel.radius * cos(angle);
                int endY = ferrisWheel.y + ferrisWheel.radius * sin(angle);
                bresenham(ferrisWheel.x, ferrisWheel.y, endX, endY, wheelPoints);
            }
            renderer.render2D(wheelPoints, 0.0f, 1.0f, 1.0f); // Cyan
        }

        // Render stalls
        for (auto& stall : stalls) {
            std::vector<std::pair<int, int>> stallPoints;
            if (stall.isCircle) {
                midpointCircle(stall.x, stall.y, stall.width, stallPoints);
            } else {
                bresenham(stall.x, stall.y, stall.x + stall.width, stall.y, stallPoints);
                bresenham(stall.x + stall.width, stall.y, stall.x + stall.width, stall.y + stall.height, stallPoints);
                bresenham(stall.x + stall.width, stall.y + stall.height, stall.x, stall.y + stall.height, stallPoints);
                bresenham(stall.x, stall.y + stall.height, stall.x, stall.y, stallPoints);
            }
            renderer.render2D(stallPoints, 1.0f, 0.0f, 1.0f); // Magenta
        }

        // Render ponds
        for (auto& pond : ponds) {
            std::vector<std::pair<int, int>> pondPoints;
            midpointCircle(pond.x, pond.y, pond.radius, pondPoints);
            renderer.render2D(pondPoints, 0.0f, 0.5f, 1.0f); // Blue
        }

        // Render paths (behind elements)
        for (auto& path : paths) {
            std::vector<std::pair<int, int>> linePoints;
            bresenham(path.x1, path.y1, path.x2, path.y2, linePoints);
            renderer.renderPath(linePoints, 1.0f, 1.0f, 0.0f); // Yellow
        }

        // Draw legend
        renderer.drawLegend();

        glfwSwapBuffers(window);
    }

    cleanupImGui();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}