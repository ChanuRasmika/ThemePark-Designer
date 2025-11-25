# Theme Park Project - Modular Attraction System

## Project Structure
```
src/
├── core/               # Core engine files
│   └── main_modular.cpp
├── attractions/        # Attraction implementations
│   └── MerryGoRound.cpp
├── managers/           # Manager classes
│   └── AttractionManager.cpp
└── utils/              # Utility files

include/
├── core/               # Core headers
│   └── Attraction.h
├── attractions/        # Attraction headers
│   └── MerryGoRound.h
├── managers/           # Manager headers
│   └── AttractionManager.h
└── utils/              # Utility headers
    ├── stb_image.h
    └── tiny_obj_loader.h

assets/                 # 3D models and textures
└── merry-go-round/
    ├── source/
    └── textures/

build/                  # Build output
lib/                    # External libraries
```

## How to Add New Attractions

### 1. Create Your Attraction Header
Create `include/attractions/YourAttraction.h`:
```cpp
#pragma once
#include "core/Attraction.h"

class YourAttraction : public Attraction {
    // Add your private members (VAO, VBO, texture, etc.)
public:
    bool load() override;  // Load your 3D model and textures
    void render(...) override;  // Render your attraction
    void cleanup() override;  // Clean up resources
    std::string getName() const override { return "Your Attraction Name"; }
    std::string getDescription() const override { return "Description"; }
};
```

### 2. Implement Your Class
Create `src/attractions/YourAttraction.cpp` and implement the methods:
- `load()`: Load your .obj file and textures from `assets/your-attraction/`
- `render()`: Draw your attraction
- `cleanup()`: Delete OpenGL resources

### 3. Register Your Attraction
In `src/core/main_modular.cpp`, add:
```cpp
#include "attractions/YourAttraction.h"
// In main():
manager.registerAttraction(2, std::make_unique<YourAttraction>());
```

### 4. Add Your Assets
Put your files in `assets/your-attraction/`:
- `source/model.obj`
- `textures/texture.png`

## Building
Use the Makefile to compile:
```
make
```

This will create `build/merrygoround.exe`

To clean build files:
```
make clean
```

## Usage
Run the program and select attractions:
```
cd build
merrygoround.exe
```

The program will ask you to add:
- Merry-Go-Round (positioned at -20, -20)
- Food Stall (positioned at 25, -15) 
- Game Stall (positioned at 15, 25)
- Road System (connects the park)

Each attraction is positioned at different locations to create a proper 3D theme park layout.

## Controls
- **WASD**: Move camera around
- **Arrow Keys**: Rotate model
- **Q/E**: Roll rotation
- **R**: Reset rotations
- **ESC**: Exit