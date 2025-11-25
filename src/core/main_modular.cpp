#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "utils/stb_image.h"

#include "managers/AttractionManager.h"
#include "managers/RoadManager.h"
#include "attractions/MerryGoRound.h"
#include "attractions/FoodStall.h"
#include "attractions/GameStall.h"
#include "attractions/Road.h"
#include "core/ThemePark.h"

// Shader sources
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture1;

void main() {
    FragColor = texture(texture1, TexCoord);
}
)";

// Camera settings
float cameraX = 0.0f, cameraY = 10.0f, cameraZ = 30.0f;
float cameraRotX = 0.0f, cameraRotY = 0.0f, cameraRotZ = 0.0f;
float zoomLevel = 1.0f;
float modelRotationX = 0.0f;
float modelRotationY = 0.0f;
float modelRotationZ = 0.0f;

// First-person camera
bool firstPersonMode = false;
float playerX = 0.0f, playerY = 2.0f, playerZ = 30.0f;
float playerYaw = 0.0f, playerPitch = 0.0f;
float walkSpeed = 2.0f;

// Matrix functions
void setIdentityMatrix(float* mat) {
    for(int i = 0; i < 16; i++) mat[i] = 0.0f;
    mat[0] = mat[5] = mat[10] = mat[15] = 1.0f;
}

void setPerspectiveMatrix(float* mat, float fov, float aspect, float near, float far) {
    float f = 1.0f / tan(fov * 0.5f * 3.14159f / 180.0f);
    setIdentityMatrix(mat);
    mat[0] = f / aspect;
    mat[5] = f;
    mat[10] = (far + near) / (near - far);
    mat[11] = -1.0f;
    mat[14] = (2.0f * far * near) / (near - far);
    mat[15] = 0.0f;
}

void setViewMatrix(float* mat, float camX, float camY, float camZ, float rotX, float rotY, float rotZ) {
    setIdentityMatrix(mat);
    
    float cosX = cos(rotX), sinX = sin(rotX);
    float cosY = cos(rotY), sinY = sin(rotY);
    float cosZ = cos(rotZ), sinZ = sin(rotZ);
    
    // Rotation matrix
    mat[0] = cosY * cosZ;
    mat[1] = cosY * sinZ;
    mat[2] = -sinY;
    mat[4] = sinX * sinY * cosZ - cosX * sinZ;
    mat[5] = sinX * sinY * sinZ + cosX * cosZ;
    mat[6] = sinX * cosY;
    mat[8] = cosX * sinY * cosZ + sinX * sinZ;
    mat[9] = cosX * sinY * sinZ - sinX * cosZ;
    mat[10] = cosX * cosY;
    
    // Translation
    mat[12] = -(mat[0] * camX + mat[4] * camY + mat[8] * camZ);
    mat[13] = -(mat[1] * camX + mat[5] * camY + mat[9] * camZ);
    mat[14] = -(mat[2] * camX + mat[6] * camY + mat[10] * camZ);
}

void setFirstPersonViewMatrix(float* mat, float x, float y, float z, float yaw, float pitch) {
    setIdentityMatrix(mat);
    
    float cosYaw = cos(yaw), sinYaw = sin(yaw);
    float cosPitch = cos(pitch), sinPitch = sin(pitch);
    
    mat[0] = cosYaw;
    mat[2] = sinYaw;
    mat[4] = sinYaw * sinPitch;
    mat[5] = cosPitch;
    mat[6] = -cosYaw * sinPitch;
    mat[8] = -sinYaw * cosPitch;
    mat[9] = sinPitch;
    mat[10] = cosYaw * cosPitch;
    
    mat[12] = -(mat[0] * x + mat[4] * y + mat[8] * z);
    mat[13] = -(mat[1] * x + mat[5] * y + mat[9] * z);
    mat[14] = -(mat[2] * x + mat[6] * y + mat[10] * z);
}

void setModelMatrix(float* mat, float rotX, float rotY, float rotZ) {
    setIdentityMatrix(mat);
    float cosX = cos(rotX), sinX = sin(rotX);
    float cosY = cos(rotY), sinY = sin(rotY);
    float cosZ = cos(rotZ), sinZ = sin(rotZ);
    
    mat[0] = cosY * cosZ;
    mat[1] = cosY * sinZ;
    mat[2] = -sinY;
    mat[4] = sinX * sinY * cosZ - cosX * sinZ;
    mat[5] = sinX * sinY * sinZ + cosX * cosZ;
    mat[6] = sinX * cosY;
    mat[8] = cosX * sinY * cosZ + sinX * sinZ;
    mat[9] = cosX * sinY * sinZ - sinX * cosZ;
    mat[10] = cosX * cosY;
}

unsigned int compileShader(GLenum type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "Shader compilation error: " << infoLog << std::endl;
    }
    return shader;
}

unsigned int createShaderProgram() {
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cout << "Shader linking error: " << infoLog << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
        firstPersonMode = !firstPersonMode;
        std::cout << (firstPersonMode ? "First-person mode" : "Overview mode") << std::endl;
    }
    
    if (firstPersonMode) {
        // First-person controls
        if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            playerX += sin(playerYaw) * walkSpeed;
            playerZ -= cos(playerYaw) * walkSpeed;
        }
        if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            playerX -= sin(playerYaw) * walkSpeed;
            playerZ += cos(playerYaw) * walkSpeed;
        }
        if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            playerX -= cos(playerYaw) * walkSpeed;
            playerZ -= sin(playerYaw) * walkSpeed;
        }
        if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            playerX += cos(playerYaw) * walkSpeed;
            playerZ += sin(playerYaw) * walkSpeed;
        }
        if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
            playerYaw -= 0.1f;
        if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
            playerYaw += 0.1f;
        if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
            playerPitch += 0.05f;
        if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
            playerPitch -= 0.05f;
    } else {
        // Overview mode controls - Camera movement
        if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
            cameraX -= 2.0f;
        if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
            cameraX += 2.0f;
        if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
            cameraZ -= 2.0f;
        if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
            cameraZ += 2.0f;
        
        // Camera rotation with arrow keys
        if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
            cameraRotX -= 0.05f;
        if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
            cameraRotX += 0.05f;
        if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
            cameraRotY -= 0.05f;
        if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
            cameraRotY += 0.05f;
        
        // Zoom controls
        if (key == GLFW_KEY_Z && (action == GLFW_PRESS || action == GLFW_REPEAT))
            zoomLevel *= 1.1f;
        if (key == GLFW_KEY_X && (action == GLFW_PRESS || action == GLFW_REPEAT))
            zoomLevel /= 1.1f;
    }
    
    if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT))
        cameraRotZ -= 0.05f;
    if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT))
        cameraRotZ += 0.05f;
    
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        cameraRotX = cameraRotY = cameraRotZ = 0.0f;
        cameraX = 0.0f; cameraY = 10.0f; cameraZ = 30.0f;
        zoomLevel = 1.0f;
        if (firstPersonMode) {
            playerX = 0.0f; playerZ = 30.0f; playerYaw = 0.0f; playerPitch = 0.0f;
        }
    }
    
    if (zoomLevel < 0.1f) zoomLevel = 0.1f;
    if (zoomLevel > 5.0f) zoomLevel = 5.0f;
    if (playerPitch > 1.5f) playerPitch = 1.5f;
    if (playerPitch < -1.5f) playerPitch = -1.5f;
}

int main() {
    std::cout << "=== Theme Park Designer ===" << std::endl;
    std::cout << "Park Size: " << ThemeParkBounds::PARK_WIDTH << "x" << ThemeParkBounds::PARK_HEIGHT << " units" << std::endl;
    
    // Setup attraction manager first
    AttractionManager manager;
    
    // Ask about attractions
    char addMerry, addFood, addGame;
    std::cout << "\nAdd Merry-Go-Round to your theme park? (y/n): ";
    std::cin >> addMerry;
    bool hasMerry = (addMerry == 'y' || addMerry == 'Y');
    
    std::cout << "Add Food Stall to your theme park? (y/n): ";
    std::cin >> addFood;
    bool hasFood = (addFood == 'y' || addFood == 'Y');
    
    std::cout << "Add Game Stall to your theme park? (y/n): ";
    std::cin >> addGame;
    bool hasGame = (addGame == 'y' || addGame == 'Y');
    
    if (hasMerry) {
        manager.registerAttraction(1, std::make_unique<MerryGoRound>());
    }
    if (hasFood) {
        manager.registerAttraction(3, std::make_unique<FoodStall>());
    }
    if (hasGame) {
        manager.registerAttraction(4, std::make_unique<GameStall>());
    }
    
    // Setup road system
    RoadManager roadManager;
    auto roadSystem = roadManager.createRoadSystem();
    bool hasRoads = (roadSystem != nullptr);
    
    if (hasRoads) {
        manager.registerAttraction(2, std::move(roadSystem));
    }
    
    // Auto-select attractions
    std::vector<int> selected;
    if (hasMerry) selected.push_back(1);
    if (hasRoads) selected.push_back(2);
    if (hasFood) selected.push_back(3);
    if (hasGame) selected.push_back(4);
    
    if (selected.empty()) {
        std::cout << "No attractions selected. Exiting." << std::endl;
        return -1;
    }
    
    std::cout << "\nLoading theme park with:" << std::endl;
    if (hasMerry) std::cout << "- Merry-go-round at (" << ThemeParkBounds::MERRY_GO_ROUND_X << ", " << ThemeParkBounds::MERRY_GO_ROUND_Z << ")" << std::endl;
    if (hasRoads) std::cout << "- Road system" << std::endl;
    if (hasFood) std::cout << "- Food stall at (" << ThemeParkBounds::FOOD_STALL_X << ", " << ThemeParkBounds::FOOD_STALL_Z << ")" << std::endl;
    if (hasGame) std::cout << "- Game stall at (" << ThemeParkBounds::GAME_STALL_X << ", " << ThemeParkBounds::GAME_STALL_Z << ")" << std::endl;
    
    if (selected.empty()) {
        std::cout << "No attractions selected. Exiting." << std::endl;
        return -1;
    }
    
    // Initialize GLFW
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Theme Park", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 800, 600);
    
    unsigned int shaderProgram = createShaderProgram();
    
    // Load selected attractions
    if (!manager.loadAttractions(selected)) {
        std::cout << "Failed to load attractions" << std::endl;
        return -1;
    }
    
    float model[16], view[16], projection[16];
    
    std::cout << "\nControls:" << std::endl;
    std::cout << "TAB: Switch between overview/first-person mode" << std::endl;
    std::cout << "Overview: WASD (move camera), Arrow Keys (rotate X/Y), Q/E (rotate Z)" << std::endl;
    std::cout << "Z/X: Zoom in/out, R (reset), ESC (exit)" << std::endl;
    std::cout << "First-person: WASD (walk), Arrow Keys (look), R (reset)" << std::endl;
    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Set projection matrix with zoom
        setPerspectiveMatrix(projection, 45.0f / zoomLevel, 800.0f / 600.0f, 0.1f, 100.0f);
        
        if (firstPersonMode) {
            setFirstPersonViewMatrix(view, playerX, playerY, playerZ, playerYaw, playerPitch);
        } else {
            setViewMatrix(view, cameraX, cameraY, cameraZ, cameraRotX, cameraRotY, cameraRotZ);
        }
        
        glUseProgram(shaderProgram);
        
        // Render each attraction at its designated position
        if (hasMerry) {
            float modelMatrix[16];
            setIdentityMatrix(modelMatrix);
            modelMatrix[12] = ThemeParkBounds::MERRY_GO_ROUND_X;
            modelMatrix[14] = ThemeParkBounds::MERRY_GO_ROUND_Z;
            
            float rotMatrix[16];
            setModelMatrix(rotMatrix, modelRotationX, modelRotationY, modelRotationZ);
            
            float finalModel[16];
            for(int i = 0; i < 16; i++) finalModel[i] = 0;
            for(int i = 0; i < 4; i++) {
                for(int j = 0; j < 4; j++) {
                    for(int k = 0; k < 4; k++) {
                        finalModel[i*4+j] += modelMatrix[i*4+k] * rotMatrix[k*4+j];
                    }
                }
            }
            manager.renderAttraction(1, shaderProgram, finalModel, view, projection);
        }
        
        if (hasFood) {
            float modelMatrix[16];
            setIdentityMatrix(modelMatrix);
            modelMatrix[12] = ThemeParkBounds::FOOD_STALL_X;
            modelMatrix[14] = ThemeParkBounds::FOOD_STALL_Z;
            manager.renderAttraction(3, shaderProgram, modelMatrix, view, projection);
        }
        
        if (hasGame) {
            float modelMatrix[16];
            setIdentityMatrix(modelMatrix);
            modelMatrix[12] = ThemeParkBounds::GAME_STALL_X;
            modelMatrix[14] = ThemeParkBounds::GAME_STALL_Z;
            manager.renderAttraction(4, shaderProgram, modelMatrix, view, projection);
        }
        
        if (hasRoads) {
            float modelMatrix[16];
            setIdentityMatrix(modelMatrix);
            manager.renderAttraction(2, shaderProgram, modelMatrix, view, projection);
        }
        
        glfwSwapBuffers(window);
    }
    
    manager.cleanup();
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}