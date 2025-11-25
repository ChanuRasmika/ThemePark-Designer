#pragma once
#include <vector>
#include <string>

struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
};

class RoadGenerator {
public:
    static std::vector<Point> bresenhamLine(int x0, int y0, int x1, int y1);
    static std::vector<Point> midpointCircle(int centerX, int centerY, int radius);
    static std::vector<float> generateRoadMesh(const std::vector<Point>& points, float width = 2.0f);
};