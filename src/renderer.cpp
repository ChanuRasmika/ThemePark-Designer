// src/renderer.cpp

#include "renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void Renderer::init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Black background
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, 800, 600);  // Set to window size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 800.0, 600.0, 0.0, -1.0, 1.0);  // 2D orthogonal projection
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Set point and line sizes for visibility
    glPointSize(3.0f);
    glLineWidth(3.0f);
}

void Renderer::render2D(const std::vector<std::pair<int, int>>& points) {
    glBegin(GL_POINTS);
    for (auto& point : points) {
        glVertex2i(point.first, point.second);
    }
    glEnd();
}

void Renderer::render2D(const std::vector<std::pair<int, int>>& points, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_POINTS);
    for (auto& point : points) {
        glVertex2i(point.first, point.second);
    }
    glEnd();
}

void Renderer::renderPath(const std::vector<std::pair<int, int>>& points, float r, float g, float b) {
    if (points.size() < 2) return;
    glColor3f(r, g, b);
    glBegin(GL_LINE_STRIP);
    for (auto& point : points) {
        glVertex2i(point.first, point.second);
    }
    glEnd();
}

void Renderer::drawRectangularStall(int x, int y, int width, int height, std::vector<std::pair<int, int>>& points) {
    // Draw the four corners of the rectangle (stall)
    points.push_back({x, y});
    points.push_back({x + width, y});
    points.push_back({x, y + height});
    points.push_back({x + width, y + height});
}

void Renderer::drawLegend() {
    int startX = 650, startY = 20, boxSize = 20, spacing = 30;
    float colors[][3] = {{1.0f, 0.5f, 0.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 1.0f}, {0.0f, 0.5f, 1.0f}};
    char labels[] = {'M', 'F', 'P', 'S', 'O'};
    
    for (int i = 0; i < 5; i++) {
        // Draw colored box
        glColor3f(colors[i][0], colors[i][1], colors[i][2]);
        glBegin(GL_QUADS);
        glVertex2i(startX, startY + i * spacing);
        glVertex2i(startX + boxSize, startY + i * spacing);
        glVertex2i(startX + boxSize, startY + i * spacing + boxSize);
        glVertex2i(startX, startY + i * spacing + boxSize);
        glEnd();
        
        // Draw letter using simple shapes
        glColor3f(1.0f, 1.0f, 1.0f);
        int textX = startX + 25;
        int textY = startY + i * spacing + 5;
        
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        
        switch(labels[i]) {
            case 'M': // Draw M
                glVertex2i(textX, textY); glVertex2i(textX, textY + 15);
                glVertex2i(textX, textY + 15); glVertex2i(textX + 4, textY + 10);
                glVertex2i(textX + 4, textY + 10); glVertex2i(textX + 8, textY + 15);
                glVertex2i(textX + 8, textY + 15); glVertex2i(textX + 8, textY);
                break;
            case 'F': // Draw F
                glVertex2i(textX, textY); glVertex2i(textX, textY + 15);
                glVertex2i(textX, textY + 15); glVertex2i(textX + 8, textY + 15);
                glVertex2i(textX, textY + 8); glVertex2i(textX + 6, textY + 8);
                break;
            case 'P': // Draw P
                glVertex2i(textX, textY); glVertex2i(textX, textY + 15);
                glVertex2i(textX, textY + 15); glVertex2i(textX + 6, textY + 15);
                glVertex2i(textX + 6, textY + 15); glVertex2i(textX + 6, textY + 8);
                glVertex2i(textX + 6, textY + 8); glVertex2i(textX, textY + 8);
                break;
            case 'S': // Draw S
                glVertex2i(textX, textY); glVertex2i(textX + 6, textY);
                glVertex2i(textX, textY); glVertex2i(textX, textY + 7);
                glVertex2i(textX, textY + 7); glVertex2i(textX + 6, textY + 7);
                glVertex2i(textX + 6, textY + 7); glVertex2i(textX + 6, textY + 15);
                glVertex2i(textX + 6, textY + 15); glVertex2i(textX, textY + 15);
                break;
            case 'O': // Draw O
                glVertex2i(textX, textY); glVertex2i(textX + 6, textY);
                glVertex2i(textX + 6, textY); glVertex2i(textX + 6, textY + 15);
                glVertex2i(textX + 6, textY + 15); glVertex2i(textX, textY + 15);
                glVertex2i(textX, textY + 15); glVertex2i(textX, textY);
                break;
        }
        
        glEnd();
    }
}
