#ifndef RENDERER3D_H
#define RENDERER3D_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

struct Mesh {
    unsigned int VAO, VBO;
    int vertexCount;
};

class Renderer3D {
public:
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;

    float yaw, pitch;
    float lastX, lastY;
    bool firstMouse;

    unsigned int shaderProgram;

    std::vector<Mesh> ferrisModel;
    std::vector<Mesh> merryModel;
    std::vector<Mesh> stallModel;
    std::vector<Mesh> pondModel;

    Renderer3D();

    void init();
    void processInput(GLFWwindow* window, float dt);
    void mouseCallback(double xpos, double ypos);
    void loadModel(const std::string& path, std::vector<Mesh>& storage);
    void drawModel(const std::vector<Mesh>& model, glm::vec3 pos, float scale);

    void start3D();
};

#endif
