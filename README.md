# Theme Park Project - Modular Attraction System

## How to Add New Attractions

### 1. Create Your Attraction Class
Copy `FerrisWheel.h` as template:
```cpp
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
Create `YourAttraction.cpp` and implement the methods:
- `load()`: Load your .obj file and textures
- `render()`: Draw your attraction
- `cleanup()`: Delete OpenGL resources

### 3. Register Your Attraction
In `main_modular.cpp`, add:
```cpp
#include "YourAttraction.h"
// In main():
manager.registerAttraction(2, std::make_unique<YourAttraction>());
```

### 4. Add Your Assets
Put your files in `Obj_texture/your-attraction/`:
- `source/model.obj`
- `textures/texture.png`

## File Structure
```
Attraction.h          - Base class interface
AttractionManager.*   - Manages multiple attractions
MerryGoRound.*       - Example implementation
main_modular.cpp     - Main program with menu system
```

## Building
Compile with all .cpp files:
```
g++ main_modular.cpp MerryGoRound.cpp AttractionManager.cpp [YourAttraction.cpp] -lglfw3 -lglad -lassimp
```

## Usage
Run the program and select attractions from the menu:
```
1. Merry Go Round - Classic spinning carousel
2. Your Attraction - Your description
Enter numbers: 1 2
```