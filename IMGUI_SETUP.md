# ImGui Setup Instructions

To use the ImGui interface, you need to download ImGui:

1. Download ImGui from: https://github.com/ocornut/imgui
2. Extract to a folder named `imgui` in your project root
3. The structure should be:
   ```
   ThemeParkBuilder/
   ├── imgui/
   │   ├── imgui.cpp
   │   ├── imgui.h
   │   ├── backends/
   │   │   ├── imgui_impl_glfw.cpp
   │   │   ├── imgui_impl_glfw.h
   │   │   ├── imgui_impl_opengl2.cpp
   │   │   └── imgui_impl_opengl2.h
   │   └── ...
   ```

4. Build the project with CMake as usual
