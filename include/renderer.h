// src/renderer.h

#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <utility>

// Declare the Renderer class
class Renderer {
public:
    void init();
    void render2D(const std::vector<std::pair<int, int>>& points);
    void render2D(const std::vector<std::pair<int, int>>& points, float r, float g, float b);
    void renderPath(const std::vector<std::pair<int, int>>& points, float r, float g, float b);

    // Declaration of drawRectangularStall function
    void drawRectangularStall(int x, int y, int width, int height, std::vector<std::pair<int, int>>& points);
    
    // Declaration of drawLegend function
    void drawLegend();
};

#endif // RENDERER_H
