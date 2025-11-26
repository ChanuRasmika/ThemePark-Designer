// src/stall.cpp

#include "stall.h"
#include <iostream>

// Constructor for rectangular stalls
Stall::Stall(int x, int y, int w, int h, bool isCircle)
    : x(x), y(y), width(w), height(h), isCircle(isCircle) {}

// Constructor for circular stalls
Stall::Stall(int x, int y, int radius)
    : x(x), y(y), width(radius), height(radius), isCircle(true) {}

void Stall::printStall() const {
    std::cout << "Stall at (" << x << ", " << y << ") with ";
    if (isCircle) {
        std::cout << "radius " << width << std::endl;
    } else {
        std::cout << "dimensions (" << width << " x " << height << ")" << std::endl;
    }
}
