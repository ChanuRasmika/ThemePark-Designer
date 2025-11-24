#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

// Simple matrix functions
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
    
    // Create rotation matrices
    float cosX = cos(rotX), sinX = sin(rotX);
    float cosY = cos(rotY), sinY = sin(rotY);
    float cosZ = cos(rotZ), sinZ = sin(rotZ);
    
    // Combined rotation matrix (Z * Y * X)
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

// Load texture
unsigned int loadTexture(const char* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        std::cout << "Texture loaded successfully: " << width << "x" << height << std::endl;
    } else {
        std::cout << "Failed to load texture: " << path << std::endl;
    }
    stbi_image_free(data);
    return textureID;
}

// Compile shader
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

// Create shader program
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

// Load model with Assimp
bool loadModel(const char* path, std::vector<float>& vertices) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "Error: " << importer.GetErrorString() << std::endl;
        return false;
    }
    
    // Process all meshes
    for (unsigned int m = 0; m < scene->mNumMeshes; m++) {
        aiMesh* mesh = scene->mMeshes[m];
        
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                unsigned int index = face.mIndices[j];
                
                // Position
                vertices.push_back(mesh->mVertices[index].x);
                vertices.push_back(mesh->mVertices[index].y);
                vertices.push_back(mesh->mVertices[index].z);
                
                // Texture coordinates
                if (mesh->mTextureCoords[0]) {
                    vertices.push_back(mesh->mTextureCoords[0][index].x);
                    vertices.push_back(mesh->mTextureCoords[0][index].y);
                } else {
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                }
            }
        }
    }
    
    std::cout << "Model loaded: " << vertices.size() / 5 << " vertices from " << scene->mNumMeshes << " meshes" << std::endl;
    return true;
}

// Keyboard callback
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    // Camera controls
    if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
        cameraAngle -= 0.05f;
    if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
        cameraAngle += 0.05f;
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
        cameraDistance -= 1.0f;
    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
        cameraDistance += 1.0f;
    
    // Model rotation controls - X axis (pitch)
    if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
        modelRotationX -= 0.02f;
    if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
        modelRotationX += 0.02f;
    
    // Model rotation controls - Y axis (yaw)
    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
        modelRotationY -= 0.02f;
    if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
        modelRotationY += 0.02f;
    
    // Model rotation controls - Z axis (roll)
    if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT))
        modelRotationZ -= 0.02f;
    if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT))
        modelRotationZ += 0.02f;
    
    // Reset rotation
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        modelRotationX = 0.0f;
        modelRotationY = 0.0f;
        modelRotationZ = 0.0f;
    }
    
    if (cameraDistance < 5.0f) cameraDistance = 5.0f;
    if (cameraDistance > 50.0f) cameraDistance = 50.0f;
}

int main() {
    // Prompt user
    std::string userInput;
    std::cout << "Enter object name: ";
    std::getline(std::cin, userInput);
    
    if (userInput != "merry go round") {
        std::cout << "Invalid input. Exiting." << std::endl;
        return -1;
    }
    
    std::cout << "Loading merry go round..." << std::endl;
    
    // Initialize GLFW
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Merry Go Round", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    
    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 800, 600);
    
    // Create shader program
    unsigned int shaderProgram = createShaderProgram();
    
    // Load model
    std::vector<float> vertices;
    if (!loadModel("Obj_texture/merry-go-round/source/merry-go-round.obj", vertices)) {
        std::cout << "Failed to load model" << std::endl;
        return -1;
    }
    
    // Load texture
    unsigned int texture = loadTexture("Obj_texture/merry-go-round/textures/Pallete.png");
    
    // Create VAO and VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    int vertexCount = vertices.size() / 5;
    
    // Matrices
    float model[16], view[16], projection[16];
    setPerspectiveMatrix(projection, 45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
    
    std::cout << "\nControls:" << std::endl;
    std::cout << "WASD: Move camera around" << std::endl;
    std::cout << "Arrow Keys: Rotate model X/Y axis" << std::endl;
    std::cout << "Q/E: Rotate model Z axis (roll)" << std::endl;
    std::cout << "R: Reset rotation" << std::endl;
    std::cout << "ESC: Exit" << std::endl;
    
    // Render loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Update camera
        float camX = sin(cameraAngle) * cameraDistance;
        float camZ = cos(cameraAngle) * cameraDistance;
        setViewMatrix(view, camX, 2.0f, camZ);
        
        // Set model matrix with XYZ rotations
        setModelMatrix(model, modelRotationX, modelRotationY, modelRotationZ);
        
        // Use shader and set uniforms
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, model);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, view);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, projection);
        
        // Bind texture and draw
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        
        glfwSwapBuffers(window);
    }
    
    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}
