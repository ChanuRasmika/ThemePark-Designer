// src/algorithms/midpoint_circle.cpp

#include "midpoint_circle.h"
#include <vector>
#include <cmath>

void midpointCircle(int x_center, int y_center, int radius, std::vector<std::pair<int, int>>& points) {
    int x = radius;
    int y = 0;
    int radiusError = 1 - x;

    while (x >= y) {
        points.push_back({x_center + x, y_center + y});
        points.push_back({x_center + y, y_center + x});
        points.push_back({x_center - y, y_center + x});
        points.push_back({x_center - x, y_center + y});
        points.push_back({x_center - x, y_center - y});
        points.push_back({x_center - y, y_center - x});
        points.push_back({x_center + y, y_center - x});
        points.push_back({x_center + x, y_center - y});

        y++;
        if (radiusError <= 0) {
            radiusError += 2 * y + 1;
        } else {
            x--;
            radiusError += 2 * (y - x + 1);
        }
    }
}
