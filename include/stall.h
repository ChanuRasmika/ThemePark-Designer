// src/stall.h

#ifndef STALL_H
#define STALL_H

#include <vector>

// Structure for Stalls
struct Stall {
    int x, y, width, height;
    bool isCircle;  // To differentiate between circular and rectangular stalls

    // Constructor for rectangular stalls
    Stall(int x, int y, int w, int h, bool isCircle = false);

    // Constructor for circular stalls
    Stall(int x, int y, int radius);

    // Method to print stall information
    void printStall() const;
};

#endif // STALL_H
