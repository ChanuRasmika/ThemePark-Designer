#include "renderer3d.h"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

Renderer3D::Renderer3D() {
    cameraPos  = glm::vec3(0.0f, 5.0f, 20.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

    yaw = -90.0f;
    pitch = 0.0f;
    firstMouse = true;
}

static unsigned int compileShader(const char* vs, const char* fs) {
    auto compile = [](GLenum type, const char* src) -> unsigned int {
        unsigned int s = glCreateShader(type);
        glShaderSource(s, 1, &src, NULL);
        glCompileShader(s);
        return s;
    };

    unsigned int vert = compile(GL_VERTEX_SHADER, vs);
    unsigned int frag = compile(GL_FRAGMENT_SHADER, fs);

    unsigned int program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    glDeleteShader(vert);
    glDeleteShader(frag);
    return program;
}

void Renderer3D::init() {
    const char* vs = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main() {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
    )";

    const char* fs = R"(
    #version 330 core
    out vec4 FragColor;
    void main() { FragColor = vec4(0.8, 0.8, 0.9, 1.0); }
    )";

    shaderProgram = compileShader(vs, fs);

    glEnable(GL_DEPTH_TEST);

    // Load models
    loadModel("Models/ferris.obj", ferrisModel);
    loadModel("Models/merry.obj", merryModel);
    loadModel("Models/stall.obj", stallModel);
    loadModel("Models/pond.obj", pondModel);
}

void Renderer3D::loadModel(const std::string& path, std::vector<Mesh>& storage) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);

    if (!scene || !scene->HasMeshes()) {
        std::cout << "Failed to load: " << path << std::endl;
        return;
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* m = scene->mMeshes[i];

        std::vector<float> vertices;
        for (unsigned int v = 0; v < m->mNumVertices; v++) {
            vertices.push_back(m->mVertices[v].x);
            vertices.push_back(m->mVertices[v].y);
            vertices.push_back(m->mVertices[v].z);
        }

        Mesh mesh;
        mesh.vertexCount = m->mNumVertices;

        glGenVertexArrays(1, &mesh.VAO);
        glGenBuffers(1, &mesh.VBO);

        glBindVertexArray(mesh.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        storage.push_back(mesh);
    }
}

void Renderer3D::drawModel(const std::vector<Mesh>& model, glm::vec3 pos, float scale) {
    glUseProgram(shaderProgram);

    glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), pos);
    modelMat = glm::scale(modelMat, glm::vec3(scale));

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));

    for (const auto& mesh : model) {
        glBindVertexArray(mesh.VAO);
        glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
    }
}

void Renderer3D::start3D() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glm::mat4 proj = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 500.0f);

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
}

void Renderer3D::processInput(GLFWwindow* window, float dt) {
    float speed = 10.0f * dt;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraPos += speed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPos -= speed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
}

void Renderer3D::mouseCallback(double xpos, double ypos) {
    if (firstMouse) { lastX = xpos; lastY = ypos; firstMouse = false; return; }

    float dx = xpos - lastX;
    float dy = lastY - ypos;
    lastX = xpos; lastY = ypos;

    float sensitivity = 0.1f;
    dx *= sensitivity;
    dy *= sensitivity;

    yaw += dx;
    pitch += dy;

    if (pitch > 89) pitch = 89;
    if (pitch < -89) pitch = -89;

    cameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront.y = sin(glm::radians(pitch));
    cameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
}
