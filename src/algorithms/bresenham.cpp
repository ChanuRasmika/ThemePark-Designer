// src/algorithms/bresenham.cpp

#include "bresenham.h"
#include <cmath>

void bresenham(int x1, int y1, int x2, int y2, std::vector<std::pair<int, int>>& points) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        points.push_back({x1, y1});
        if (x1 == x2 && y1 == y2) break;
        int e2 = err * 2;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
}
