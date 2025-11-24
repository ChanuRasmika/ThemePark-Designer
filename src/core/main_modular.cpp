#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "utils/stb_image.h"

#include "managers/AttractionManager.h"
#include "attractions/MerryGoRound.h"

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
float cameraAngle = 0.0f;
float cameraDistance = 15.0f;
float modelRotationX = 0.0f;
float modelRotationY = 0.0f;
float modelRotationZ = 0.0f;

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

void setViewMatrix(float* mat, float camX, float camY, float camZ) {
    setIdentityMatrix(mat);
    mat[12] = -camX;
    mat[13] = -camY;
    mat[14] = -camZ;
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
    
    if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
        cameraAngle -= 0.05f;
    if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
        cameraAngle += 0.05f;
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
        cameraDistance -= 1.0f;
    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
        cameraDistance += 1.0f;
    
    if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
        modelRotationX -= 0.02f;
    if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
        modelRotationX += 0.02f;
    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
        modelRotationY -= 0.02f;
    if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
        modelRotationY += 0.02f;
    if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT))
        modelRotationZ -= 0.02f;
    if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT))
        modelRotationZ += 0.02f;
    
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        modelRotationX = modelRotationY = modelRotationZ = 0.0f;
    }
    
    if (cameraDistance < 5.0f) cameraDistance = 5.0f;
    if (cameraDistance > 50.0f) cameraDistance = 50.0f;
}

int main() {
    // Setup attraction manager
    AttractionManager manager;
    manager.registerAttraction(1, std::make_unique<MerryGoRound>());
    
    // Show menu and get user selection
    manager.showMenu();
    std::vector<int> selected = manager.getUserSelection();
    
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
    setPerspectiveMatrix(projection, 45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
    
    std::cout << "\nControls: WASD (camera), Arrow Keys (rotate), Q/E (roll), R (reset), ESC (exit)" << std::endl;
    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        float camX = sin(cameraAngle) * cameraDistance;
        float camZ = cos(cameraAngle) * cameraDistance;
        setViewMatrix(view, camX, 2.0f, camZ);
        setModelMatrix(model, modelRotationX, modelRotationY, modelRotationZ);
        
        glUseProgram(shaderProgram);
        manager.renderAttractions(shaderProgram, model, view, projection);
        
        glfwSwapBuffers(window);
    }
    
    manager.cleanup();
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}