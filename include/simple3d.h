#ifndef SIMPLE3D_H
#define SIMPLE3D_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <string>

class Simple3D {
public:
    float cameraX, cameraY, cameraZ;
    float cameraYaw, cameraPitch;
    bool firstMouse;
    float lastMouseX, lastMouseY;
    unsigned int grassTexture;
    unsigned int woodTexture, goldTexture, circusTexture, horseTexture, goldPatternTexture;
    unsigned int steelTexture, beamTexture, cabinTexture, cableTexture;
    unsigned int waterTexture, stoneTexture, fountainTexture, lilypadTexture, rockTexture;
    unsigned int brickTexture;

    Simple3D();
    void init();
    void processInput(GLFWwindow* window, float deltaTime);
    void mouseCallback(double xpos, double ypos);
    void render(int width, int height);
    void drawCube(float x, float y, float z, float size, float r, float g, float b);
    void drawCylinder(float x, float y, float z, float radius, float height, float r, float g, float b);
    void drawSphere(float x, float y, float z, float radius, float r, float g, float b);
    void drawPath(float x1, float y1, float z1, float x2, float y2, float z2, float width, float r, float g, float b);
    void drawCarousel(float x, float y, float z, float radius);
    void drawFerrisWheel(float x, float y, float z, float radius);
    void drawStall(float x, float y, float z, float width, float height);
    void drawPond(float x, float y, float z, float radius);
    
private:
    unsigned int loadTexture(const char* filename);

};

#endif