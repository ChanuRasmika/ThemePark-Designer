#ifndef IMGUI_SETUP_H
#define IMGUI_SETUP_H

#include <GLFW/glfw3.h>

void setupImGui(GLFWwindow* window);
void cleanupImGui();
void renderImGui();
bool showInputWindow(int& numMerry, int& numFerris, int& numStalls, int& numPonds, bool& autoGenerate);

#endif