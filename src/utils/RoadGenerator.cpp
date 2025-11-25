#include "utils/RoadGenerator.h"
#include <cmath>

std::vector<Point> RoadGenerator::bresenhamLine(int x0, int y0, int x1, int y1) {
    std::vector<Point> points;
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    
    while (true) {
        points.emplace_back(x0, y0);
        if (x0 == x1 && y0 == y1) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
    return points;
}

std::vector<Point> RoadGenerator::midpointCircle(int centerX, int centerY, int radius) {
    std::vector<Point> points;
    int x = 0, y = radius;
    int d = 1 - radius;
    
    auto addSymmetricPoints = [&](int px, int py) {
        points.emplace_back(centerX + px, centerY + py);
        points.emplace_back(centerX - px, centerY + py);
        points.emplace_back(centerX + px, centerY - py);
        points.emplace_back(centerX - px, centerY - py);
        points.emplace_back(centerX + py, centerY + px);
        points.emplace_back(centerX - py, centerY + px);
        points.emplace_back(centerX + py, centerY - px);
        points.emplace_back(centerX - py, centerY - px);
    };
    
    while (x <= y) {
        addSymmetricPoints(x, y);
        x++;
        if (d < 0) {
            d += 2 * x + 1;
        } else {
            y--;
            d += 2 * (x - y) + 1;
        }
    }
    return points;
}

std::vector<float> RoadGenerator::generateRoadMesh(const std::vector<Point>& points, float width) {
    std::vector<float> vertices;
    float halfWidth = width * 0.5f;
    
    for (size_t i = 0; i < points.size(); i++) {
        float x = points[i].x;
        float z = points[i].y;
        
        // Create quad for each point
        vertices.insert(vertices.end(), {
            x - halfWidth, 0.0f, z - halfWidth, 0.0f, 0.0f,
            x + halfWidth, 0.0f, z - halfWidth, 1.0f, 0.0f,
            x + halfWidth, 0.0f, z + halfWidth, 1.0f, 1.0f,
            x - halfWidth, 0.0f, z + halfWidth, 0.0f, 1.0f
        });
    }
    return vertices;
}