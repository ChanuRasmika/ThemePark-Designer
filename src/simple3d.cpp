#define STB_IMAGE_IMPLEMENTATION
#include "simple3d.h"
#include <cmath>
#include <iostream>
#include "../stb_image.h"

Simple3D::Simple3D() {
    cameraX = 0.0f;
    cameraY = 15.0f;
    cameraZ = 10.0f;
    cameraYaw = 0.0f;
    cameraPitch = -45.0f;
    firstMouse = true;
    lastMouseX = 400.0f;
    lastMouseY = 300.0f;
    grassTexture = 0;
    woodTexture = goldTexture = circusTexture = horseTexture = goldPatternTexture = 0;
    waterTexture = stoneTexture = fountainTexture = lilypadTexture = rockTexture = 0;
    brickTexture = 0;
}

void Simple3D::init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float lightPos[] = {10.0f, 20.0f, 10.0f, 1.0f};
    float lightColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    
    grassTexture = loadTexture("textures/grass.png");
    if (grassTexture == 0) {
        grassTexture = loadTexture("../textures/grass.png");
    }
    
    // Load carousel textures with debug output
    std::cout << "Loading carousel textures..." << std::endl;
    woodTexture = loadTexture("textures/woodenFloor.jpg");
    std::cout << "Wood texture ID: " << woodTexture << std::endl;
    goldTexture = loadTexture("textures/gold.jpg");
    std::cout << "Gold texture ID: " << goldTexture << std::endl;
    circusTexture = loadTexture("textures/circusStrip.jpg");
    std::cout << "Circus texture ID: " << circusTexture << std::endl;
    horseTexture = loadTexture("textures/horseFur.jpg");
    std::cout << "Horse texture ID: " << horseTexture << std::endl;
    goldPatternTexture = loadTexture("textures/goldPattern.jpg");
    
    // Load ferris wheel textures
    steelTexture = loadTexture("textures/galvanizedSteel.jpg");
    beamTexture = loadTexture("textures/paintedBeam.jpg");
    cabinTexture = loadTexture("textures/cabin.jpg");
    cableTexture = loadTexture("textures/cablePattern.jpg");
    std::cout << "Gold pattern texture ID: " << goldPatternTexture << std::endl;
    
    // Load pond textures
    waterTexture = loadTexture("textures/water.jpg");
    stoneTexture = loadTexture("textures/concreteMoss.jpg");
    fountainTexture = loadTexture("textures/fountainStone.jpg");
    lilypadTexture = loadTexture("textures/lilypad.jpg");
    rockTexture = loadTexture("textures/riverRock.jpg");
    
    // Load path texture
    brickTexture = loadTexture("textures/brickPathway.jpeg");
    std::cout << "Brick pathway texture ID: " << brickTexture << std::endl;
}

void Simple3D::processInput(GLFWwindow* window, float deltaTime) {
    float speed = 5.0f * deltaTime;

    // Convert yaw to radians
    float yaw = cameraYaw * 3.1415926535f / 180.0f;

    // Standard FPS movement vectors
    float forwardX = -sinf(yaw);  // Forward in camera direction
    float forwardZ = -cosf(yaw);

    // Right vector (90 degrees from forward)
    float rightX = cosf(yaw);
    float rightZ = -sinf(yaw);

    // Forward (W)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraX += forwardX * speed;
        cameraZ += forwardZ * speed;
    }

    // Backward (S)
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraX -= forwardX * speed;
        cameraZ -= forwardZ * speed;
    }

    // Strafe Left (A)
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraX -= rightX * speed;
        cameraZ -= rightZ * speed;
    }

    // Strafe Right (D)
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraX += rightX * speed;
        cameraZ += rightZ * speed;
    }

    // Vertical movement
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraY += speed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraY -= speed;
    
    // Arrow keys for looking around
    float lookSpeed = 50.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        cameraYaw -= lookSpeed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        cameraYaw += lookSpeed;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cameraPitch += lookSpeed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cameraPitch -= lookSpeed;
    
    // Clamp pitch to prevent flipping
    if (cameraPitch > 89.0f) cameraPitch = 89.0f;
    if (cameraPitch < -89.0f) cameraPitch = -89.0f;
}


void Simple3D::mouseCallback(double xpos, double ypos) {
    if (firstMouse) {
        lastMouseX = xpos;
        lastMouseY = ypos;
        firstMouse = false;
        return;
    }

    float xoffset = (float)(xpos - lastMouseX);
    float yoffset = (float)(lastMouseY - ypos);

    lastMouseX = xpos;
    lastMouseY = ypos;

    const float sensitivity = 0.003f;  // Increased for better responsiveness
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    cameraYaw   += xoffset;
    cameraPitch += yoffset;

    // Clamp pitch to prevent flipping
    if (cameraPitch > 89.0f)  cameraPitch = 89.0f;
    if (cameraPitch < -89.0f) cameraPitch = -89.0f;
}

void Simple3D::render(int width, int height) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)width / (float)height;
    float fH = tan(60.0f * 3.1415926535f / 360.0f) * 0.1f;
    float fW = fH * aspect;
    glFrustum(-fW, fW, -fH, fH, 0.1f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Camera rotation with correct sign convention
    glRotatef(-cameraPitch, 1.0f, 0.0f, 0.0f);
    glRotatef(-cameraYaw,   0.0f, 1.0f, 0.0f);
    glTranslatef(-cameraX, -cameraY, -cameraZ);

    // Ground with grass texture
    glDisable(GL_CULL_FACE);
    if (grassTexture > 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, grassTexture);
        glColor3f(1.0f, 1.0f, 1.0f);
        
        glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-100, 0.0f, -100);
        glTexCoord2f(20.0f, 0.0f); glVertex3f( 100, 0.0f, -100);
        glTexCoord2f(20.0f, 20.0f); glVertex3f( 100, 0.0f,  100);
        glTexCoord2f(0.0f, 20.0f); glVertex3f(-100, 0.0f,  100);
        glEnd();
        
        glDisable(GL_TEXTURE_2D);
    } else {
        glColor3f(0.3f, 0.8f, 0.3f);
        glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);
        glVertex3f(-100, 0.0f, -100);
        glVertex3f( 100, 0.0f, -100);
        glVertex3f( 100, 0.0f,  100);
        glVertex3f(-100, 0.0f,  100);
        glEnd();
    }
    glEnable(GL_CULL_FACE);



    // Draw crosshair
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    // Horizontal line
    glVertex2f(width/2 - 10, height/2);
    glVertex2f(width/2 + 10, height/2);
    // Vertical line
    glVertex2f(width/2, height/2 - 10);
    glVertex2f(width/2, height/2 + 10);
    glEnd();
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

void Simple3D::drawCube(float x, float y, float z, float size, float r, float g, float b) {
    glEnable(GL_COLOR_MATERIAL);
    glColor3f(r, g, b);
    glPushMatrix();
    glTranslatef(x, y + size/2, z);
    glScalef(size, size, size);

    glBegin(GL_QUADS);
    // Front
    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3f(-0.5f,-0.5f, 0.5f);
    glTexCoord2f(1,0); glVertex3f(0.5f,-0.5f, 0.5f);
    glTexCoord2f(1,1); glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(0,1); glVertex3f(-0.5f, 0.5f, 0.5f);
    // Back
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0); glVertex3f(0.5f,-0.5f,-0.5f);
    glTexCoord2f(1,0); glVertex3f(-0.5f,-0.5f,-0.5f);
    glTexCoord2f(1,1); glVertex3f(-0.5f, 0.5f,-0.5f);
    glTexCoord2f(0,1); glVertex3f(0.5f, 0.5f,-0.5f);
    // Top
    glNormal3f(0,1,0);
    glTexCoord2f(0,0); glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(1,0); glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(1,1); glVertex3f(0.5f, 0.5f,-0.5f);
    glTexCoord2f(0,1); glVertex3f(-0.5f, 0.5f,-0.5f);
    // Bottom
    glNormal3f(0,-1,0);
    glTexCoord2f(0,0); glVertex3f(-0.5f,-0.5f,-0.5f);
    glTexCoord2f(1,0); glVertex3f(0.5f,-0.5f,-0.5f);
    glTexCoord2f(1,1); glVertex3f(0.5f,-0.5f, 0.5f);
    glTexCoord2f(0,1); glVertex3f(-0.5f,-0.5f, 0.5f);
    // Right
    glNormal3f(1,0,0);
    glTexCoord2f(0,0); glVertex3f(0.5f,-0.5f, 0.5f);
    glTexCoord2f(1,0); glVertex3f(0.5f,-0.5f,-0.5f);
    glTexCoord2f(1,1); glVertex3f(0.5f, 0.5f,-0.5f);
    glTexCoord2f(0,1); glVertex3f(0.5f, 0.5f, 0.5f);
    // Left
    glNormal3f(-1,0,0);
    glTexCoord2f(0,0); glVertex3f(-0.5f,-0.5f,-0.5f);
    glTexCoord2f(1,0); glVertex3f(-0.5f,-0.5f, 0.5f);
    glTexCoord2f(1,1); glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(0,1); glVertex3f(-0.5f, 0.5f,-0.5f);
    glEnd();

    glPopMatrix();
    glDisable(GL_COLOR_MATERIAL);
}

void Simple3D::drawCylinder(float x, float y, float z, float radius, float height, float r, float g, float b) {
    glEnable(GL_COLOR_MATERIAL);
    glColor3f(r, g, b);
    glPushMatrix();
    glTranslatef(x, y, z);

    int segments = 20;

    // Top cap
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0,1,0);
    glTexCoord2f(0.5f, 0.5f); glVertex3f(0, height, 0);
    for (int i = 0; i <= segments; ++i) {
        float a = 2.0f * 3.1415926535f * i / segments;
        glTexCoord2f(0.5f + 0.5f * cosf(a), 0.5f + 0.5f * sinf(a));
        glVertex3f(radius * cosf(a), height, radius * sinf(a));
    }
    glEnd();

    // Sides
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; ++i) {
        float a = 2.0f * 3.1415926535f * i / segments;
        float cx = radius * cosf(a);
        float cz = radius * sinf(a);
        float u = (float)i / segments;
        glNormal3f(cx/radius, 0, cz/radius);
        glTexCoord2f(u, 0); glVertex3f(cx, 0, cz);
        glTexCoord2f(u, 1); glVertex3f(cx, height, cz);
    }
    glEnd();

    glPopMatrix();
    glDisable(GL_COLOR_MATERIAL);
}

void Simple3D::drawSphere(float x, float y, float z, float radius, float r, float g, float b) {
    glColor3f(r, g, b);
    glPushMatrix();
    glTranslatef(x, y + radius, z);

    const int stacks = 12;
    const int slices = 20;

    for (int i = 0; i < stacks; ++i) {
        float lat0 = 3.1415926535f * (-0.5f + (float)i / stacks);
        float lat1 = 3.1415926535f * (-0.5f + (float)(i + 1) / stacks);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; ++j) {
            float lng = 2.0f * 3.1415926535f * j / slices;
            float u = (float)j / slices;
            float v0 = (float)i / stacks;
            float v1 = (float)(i + 1) / stacks;

            float x0 = radius * cosf(lat0) * cosf(lng);
            float y0 = radius * sinf(lat0);
            float z0 = radius * cosf(lat0) * sinf(lng);

            float x1 = radius * cosf(lat1) * cosf(lng);
            float y1 = radius * sinf(lat1);
            float z1 = radius * cosf(lat1) * sinf(lng);

            glTexCoord2f(u, v0); glVertex3f(x0, y0, z0);
            glTexCoord2f(u, v1); glVertex3f(x1, y1, z1);
        }
        glEnd();
    }
    glPopMatrix();
}

unsigned int Simple3D::loadTexture(const char* filename) {
    unsigned int textureID = 0;
    
    std::cout << "Attempting to load: " << filename << std::endl;
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data) {
        glGenTextures(1, &textureID);
        GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        stbi_image_free(data);
        std::cout << "Successfully loaded: " << filename << " (" << width << "x" << height << ", channels: " << nrChannels << ")" << std::endl;
    } else {
        std::cout << "Failed to load: " << filename << " - " << stbi_failure_reason() << std::endl;
        
        // Try alternative path
        std::string altPath = "../" + std::string(filename);
        std::cout << "Trying alternative path: " << altPath << std::endl;
        data = stbi_load(altPath.c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glGenTextures(1, &textureID);
            GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
            
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            stbi_image_free(data);
            std::cout << "Successfully loaded alternative: " << altPath << std::endl;
        } else {
            std::cout << "Alternative path also failed: " << stbi_failure_reason() << std::endl;
        }
    }
    
    return textureID;
}

void Simple3D::drawPond(float x, float y, float z, float radius) {
    glPushMatrix();
    glTranslatef(x, y, z);
    
    // Stone rim with cubes (dynamic count based on radius)
    int numStones = (int)(radius * 8.0f); // More stones for larger radius
    for (int i = 0; i < numStones; i++) {
        float angle = 2.0f * 3.1415926535f * i / numStones;
        float stoneX = radius * 1.1f * cosf(angle);
        float stoneZ = radius * 1.1f * sinf(angle);
        
        glPushMatrix();
        glTranslatef(stoneX, 0.1f, stoneZ);
        
        if (stoneTexture > 0) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, stoneTexture);
            glColor3f(1.0f, 1.0f, 1.0f);
            drawCube(0, 0, 0, 0.4f, 1.0f, 1.0f, 1.0f);
            glDisable(GL_TEXTURE_2D);
        } else {
            glColor3f(0.6f, 0.6f, 0.5f);
            drawCube(0, 0, 0, 0.4f, 0.6f, 0.6f, 0.5f);
        }
        
        glPopMatrix();
    }
    
    // Water surface
    glDisable(GL_CULL_FACE);
    if (waterTexture > 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, waterTexture);
        glColor3f(1.0f, 1.0f, 1.0f);
    } else {
        glColor3f(0.2f, 0.5f, 0.8f);
    }
    
    int waterSegments = 20;
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, 1, 0);
    if (waterTexture > 0) glTexCoord2f(0.5f, 0.5f);
    glVertex3f(0, 0.05f, 0);
    
    for (int i = 0; i <= waterSegments; i++) {
        float angle = 2.0f * 3.1415926535f * i / waterSegments;
        float waterX = radius * cosf(angle);
        float waterZ = radius * sinf(angle);
        
        if (waterTexture > 0) {
            float u = 0.5f + 0.5f * cosf(angle);
            float v = 0.5f + 0.5f * sinf(angle);
            glTexCoord2f(u, v);
        }
        glVertex3f(waterX, 0.05f, waterZ);
    }
    glEnd();
    
    if (waterTexture > 0) glDisable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    
    glPopMatrix();
}

void Simple3D::drawStall(float x, float y, float z, float width, float height) {
    glPushMatrix();
    glTranslatef(x, y, z);
    
    float stallHeight = 4.0f;
    
    // Support poles (4 corners) - bigger poles
    glColor3f(0.4f, 0.2f, 0.1f);
    drawCylinder(-width*0.6f, 0, -width*0.5f, 0.12f, stallHeight, 0.4f, 0.2f, 0.1f);
    drawCylinder(width*0.6f, 0, -width*0.5f, 0.12f, stallHeight, 0.4f, 0.2f, 0.1f);
    drawCylinder(-width*0.6f, 0, width*0.5f, 0.12f, stallHeight, 0.4f, 0.2f, 0.1f);
    drawCylinder(width*0.6f, 0, width*0.5f, 0.12f, stallHeight, 0.4f, 0.2f, 0.1f);
    
    // Tent roof (triangular) - bigger roof
    glColor3f(0.8f, 0.1f, 0.1f);
    glBegin(GL_TRIANGLES);
    // Front triangle
    glNormal3f(0, 0.7f, 0.7f);
    glVertex3f(-width*0.7f, stallHeight, width*0.6f);
    glVertex3f(width*0.7f, stallHeight, width*0.6f);
    glVertex3f(0, stallHeight + width*0.5f, 0);
    // Back triangle
    glNormal3f(0, 0.7f, -0.7f);
    glVertex3f(width*0.7f, stallHeight, -width*0.6f);
    glVertex3f(-width*0.7f, stallHeight, -width*0.6f);
    glVertex3f(0, stallHeight + width*0.5f, 0);
    // Left side
    glNormal3f(-0.7f, 0.7f, 0);
    glVertex3f(-width*0.7f, stallHeight, -width*0.6f);
    glVertex3f(-width*0.7f, stallHeight, width*0.6f);
    glVertex3f(0, stallHeight + width*0.5f, 0);
    // Right side
    glNormal3f(0.7f, 0.7f, 0);
    glVertex3f(width*0.7f, stallHeight, width*0.6f);
    glVertex3f(width*0.7f, stallHeight, -width*0.6f);
    glVertex3f(0, stallHeight + width*0.5f, 0);
    glEnd();
    
    // Tent walls (fabric panels)
    glColor3f(0.9f, 0.8f, 0.2f);
    // Back wall - bigger
    glBegin(GL_QUADS);
    glNormal3f(0, 0, -1);
    glVertex3f(-width*0.6f, 0.2f, -width*0.5f);
    glVertex3f(width*0.6f, 0.2f, -width*0.5f);
    glVertex3f(width*0.6f, stallHeight, -width*0.5f);
    glVertex3f(-width*0.6f, stallHeight, -width*0.5f);
    glEnd();
    
    // Side walls (partial) - bigger
    glBegin(GL_QUADS);
    // Left wall
    glNormal3f(-1, 0, 0);
    glVertex3f(-width*0.6f, 0.2f, -width*0.5f);
    glVertex3f(-width*0.6f, stallHeight, -width*0.5f);
    glVertex3f(-width*0.6f, stallHeight, 0);
    glVertex3f(-width*0.6f, 0.2f, 0);
    // Right wall
    glNormal3f(1, 0, 0);
    glVertex3f(width*0.6f, 0.2f, 0);
    glVertex3f(width*0.6f, stallHeight, 0);
    glVertex3f(width*0.6f, stallHeight, -width*0.5f);
    glVertex3f(width*0.6f, 0.2f, -width*0.5f);
    glEnd();
    
    // Serving counter - bigger
    glColor3f(0.6f, 0.4f, 0.2f);
    glPushMatrix();
    glTranslatef(0, 1.2f, width*0.4f);
    glScalef(width*0.9f, 0.2f, 0.6f);
    drawCube(0, 0, 0, 1.0f, 0.6f, 0.4f, 0.2f);
    glPopMatrix();
    
    // Menu board - bigger
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(0, stallHeight*0.6f, -width*0.48f);
    glScalef(width*0.8f, 1.2f, 0.05f);
    drawCube(0, 0, 0, 1.0f, 0.2f, 0.2f, 0.2f);
    glPopMatrix();
    
    // Hanging lanterns
    glColor3f(1.0f, 0.8f, 0.2f);
    drawSphere(-width*0.2f, stallHeight-0.3f, width*0.1f, 0.1f, 1.0f, 0.8f, 0.2f);
    drawSphere(width*0.2f, stallHeight-0.3f, width*0.1f, 0.1f, 1.0f, 0.8f, 0.2f);
    
    glPopMatrix();
}

void Simple3D::drawFerrisWheel(float x, float y, float z, float radius) {
    glPushMatrix();
    glTranslatef(x, y, z);
    
    float towerHeight = radius * 1.2f;
    
    // Center circle with steel texture
    if (steelTexture > 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, steelTexture);
        glColor3f(1.0f, 1.0f, 1.0f);
        glPushMatrix();
        glTranslatef(0, towerHeight, 0);
        drawCylinder(0, 0, 0, radius * 0.2f, 0.3f, 1.0f, 1.0f, 1.0f);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    } else {
        glColor3f(0.6f, 0.6f, 0.6f);
        glPushMatrix();
        glTranslatef(0, towerHeight, 0);
        drawCylinder(0, 0, 0, radius * 0.2f, 0.3f, 0.6f, 0.6f, 0.6f);
        glPopMatrix();
    }
    
    // Outer rim with steel texture
    glDisable(GL_CULL_FACE);
    if (steelTexture > 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, steelTexture);
        glColor3f(1.0f, 1.0f, 1.0f);
    } else {
        glColor3f(0.8f, 0.8f, 0.8f);
    }
    
    glPushMatrix();
    glTranslatef(0, towerHeight, 0);
    
    int rimSegments = 24;
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= rimSegments; i++) {
        float angle = 2.0f * 3.1415926535f * i / rimSegments;
        float x1 = radius * cosf(angle);
        float y1 = radius * sinf(angle);
        float u = (float)i / rimSegments;
        
        glNormal3f(cosf(angle), sinf(angle), 0);
        if (steelTexture > 0) {
            glTexCoord2f(u, 0); glVertex3f(x1, y1, -0.1f);
            glTexCoord2f(u, 1); glVertex3f(x1, y1, 0.1f);
        } else {
            glVertex3f(x1, y1, -0.1f);
            glVertex3f(x1, y1, 0.1f);
        }
    }
    glEnd();
    glPopMatrix();
    
    if (steelTexture > 0) glDisable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    
    // Spokes with steel texture
    int numGondolas = 8;
    for (int i = 0; i < numGondolas; i++) {
        float angle = 2.0f * 3.1415926535f * i / numGondolas;
        
        if (steelTexture > 0) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, steelTexture);
            glColor3f(1.0f, 1.0f, 1.0f);
            glPushMatrix();
            glTranslatef(0, towerHeight, 0);
            glRotatef(angle * 180.0f / 3.1415926535f, 0, 0, 1);
            drawCylinder(0, 0, 0, 0.05f, radius, 1.0f, 1.0f, 1.0f);
            glPopMatrix();
            glDisable(GL_TEXTURE_2D);
        } else {
            glColor3f(0.7f, 0.7f, 0.7f);
            glPushMatrix();
            glTranslatef(0, towerHeight, 0);
            glRotatef(angle * 180.0f / 3.1415926535f, 0, 0, 1);
            drawCylinder(0, 0, 0, 0.05f, radius, 0.7f, 0.7f, 0.7f);
            glPopMatrix();
        }
    }
    
    // Gondolas with cabin texture
    for (int i = 0; i < numGondolas; i++) {
        float angle = 2.0f * 3.1415926535f * i / numGondolas;
        float gondolaX = radius * cosf(angle);
        float gondolaY = radius * sinf(angle);
        
        glPushMatrix();
        glTranslatef(gondolaX, towerHeight + gondolaY, 0);
        
        // Gondola cabin with texture
        if (cabinTexture > 0) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, cabinTexture);
            glColor3f(1.0f, 1.0f, 1.0f);
            drawCube(0, -0.8f, 0, 0.8f, 1.0f, 1.0f, 1.0f);
            glDisable(GL_TEXTURE_2D);
        } else {
            glColor3f(0.2f, 0.4f, 0.8f);
            drawCube(0, -0.8f, 0, 0.8f, 0.2f, 0.4f, 0.8f);
        }
        
        // Support cables with cable texture
        if (cableTexture > 0) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, cableTexture);
            glColor3f(1.0f, 1.0f, 1.0f);
            drawCylinder(-0.3f, -0.4f, 0, 0.02f, 0.4f, 1.0f, 1.0f, 1.0f);
            drawCylinder(0.3f, -0.4f, 0, 0.02f, 0.4f, 1.0f, 1.0f, 1.0f);
            glDisable(GL_TEXTURE_2D);
        } else {
            glColor3f(0.3f, 0.3f, 0.3f);
            drawCylinder(-0.3f, -0.4f, 0, 0.02f, 0.4f, 0.3f, 0.3f, 0.3f);
            drawCylinder(0.3f, -0.4f, 0, 0.02f, 0.4f, 0.3f, 0.3f, 0.3f);
        }
        
        glPopMatrix();
    }
    
    // Cross bracing with beam texture
    if (beamTexture > 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, beamTexture);
        glColor3f(1.0f, 1.0f, 1.0f);
        glPushMatrix();
        glTranslatef(0, towerHeight * 0.3f, 0);
        glRotatef(90, 0, 1, 0);
        drawCylinder(0, 0, 0, 0.08f, radius * 0.6f, 1.0f, 1.0f, 1.0f);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    } else {
        glColor3f(0.6f, 0.6f, 0.6f);
        glPushMatrix();
        glTranslatef(0, towerHeight * 0.3f, 0);
        glRotatef(90, 0, 1, 0);
        drawCylinder(0, 0, 0, 0.08f, radius * 0.6f, 0.6f, 0.6f, 0.6f);
        glPopMatrix();
    }
    
    glPopMatrix();
}

void Simple3D::drawCarousel(float x, float y, float z, float radius) {
    glDisable(GL_CULL_FACE);
    glPushMatrix();
    glTranslatef(x, y, z);
    
    // Base platform with wood texture
    if (woodTexture > 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, woodTexture);
        glColor3f(1.0f, 1.0f, 1.0f);
        drawCylinder(0, 0.2f, 0, radius, 0.4f, 1.0f, 1.0f, 1.0f);
        glDisable(GL_TEXTURE_2D);
    } else {
        glColor3f(0.8f, 0.6f, 0.4f);
        drawCylinder(0, 0.2f, 0, radius, 0.4f, 0.8f, 0.6f, 0.4f);
    }
    

    
    // Center pole with gold texture
    if (goldTexture > 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, goldTexture);
        glColor3f(1.0f, 1.0f, 1.0f);
        drawCylinder(0, 0.4f, 0, 0.3f, 4.0f, 1.0f, 1.0f, 1.0f);
        glDisable(GL_TEXTURE_2D);
    } else {
        glColor3f(0.7f, 0.7f, 0.2f);
        drawCylinder(0, 0.4f, 0, 0.3f, 4.0f, 0.7f, 0.7f, 0.2f);
    }
    
    // Roof structure with circus texture
    glPushMatrix();
    glTranslatef(0, 4.4f, 0);
    
    if (circusTexture > 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, circusTexture);
        glColor3f(1.0f, 1.0f, 1.0f);
    } else {
        glColor3f(0.9f, 0.2f, 0.2f);
    }
    
    int segments = 12;
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < segments; i++) {
        float angle1 = 2.0f * 3.1415926535f * i / segments;
        float angle2 = 2.0f * 3.1415926535f * (i + 1) / segments;
        
        float x1 = radius * 1.2f * cosf(angle1);
        float z1 = radius * 1.2f * sinf(angle1);
        float x2 = radius * 1.2f * cosf(angle2);
        float z2 = radius * 1.2f * sinf(angle2);
        
        glNormal3f(0, 1, 0);
        if (circusTexture > 0) {
            glTexCoord2f(0.5f, 1); glVertex3f(0, 2.0f, 0);
            glTexCoord2f(0, 0); glVertex3f(x1, 0, z1);
            glTexCoord2f(1, 0); glVertex3f(x2, 0, z2);
        } else {
            glVertex3f(0, 2.0f, 0);
            glVertex3f(x1, 0, z1);
            glVertex3f(x2, 0, z2);
        }
    }
    glEnd();
    
    if (circusTexture > 0) glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    // Horses with horse fur texture
    int numHorses = 8;
    for (int i = 0; i < numHorses; i++) {
        float angle = 2.0f * 3.1415926535f * i / numHorses;
        float horseX = radius * 0.7f * cosf(angle);
        float horseZ = radius * 0.7f * sinf(angle);
        
        // Support pole with gold texture
        if (goldTexture > 0) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, goldTexture);
            glColor3f(1.0f, 1.0f, 1.0f);
            drawCylinder(horseX, 1.5f, horseZ, 0.05f, 2.8f, 1.0f, 1.0f, 1.0f);
            glDisable(GL_TEXTURE_2D);
        } else {
            glColor3f(0.6f, 0.6f, 0.6f);
            drawCylinder(horseX, 1.5f, horseZ, 0.05f, 2.8f, 0.6f, 0.6f, 0.6f);
        }
        
        // Horse with horse fur texture
        glPushMatrix();
        glTranslatef(horseX, 1.8f, horseZ);
        glRotatef(angle * 180.0f / 3.1415926535f, 0, 1, 0);
        
        if (horseTexture > 0) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, horseTexture);
            glColor3f(1.0f, 1.0f, 1.0f);
        } else {
            glColor3f(0.6f, 0.4f, 0.2f);
        }
        
        // Horse body
        glPushMatrix();
        glScalef(0.8f, 0.4f, 0.3f);
        drawCube(0, 0, 0, 1.0f, 1.0f, 1.0f, 1.0f);
        glPopMatrix();
        
        // Horse head
        glPushMatrix();
        glTranslatef(0.5f, 0.3f, 0);
        glScalef(0.3f, 0.3f, 0.2f);
        drawCube(0, 0, 0, 1.0f, 1.0f, 1.0f, 1.0f);
        glPopMatrix();
        
        if (horseTexture > 0) glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
    
    // Decorative elements with gold pattern
    for (int i = 0; i < 16; i++) {
        float angle = 2.0f * 3.1415926535f * i / 16;
        float decorX = radius * 0.9f * cosf(angle);
        float decorZ = radius * 0.9f * sinf(angle);
        
        glPushMatrix();
        glTranslatef(decorX, 3.2f, decorZ);
        
        if (goldPatternTexture > 0) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, goldPatternTexture);
            glColor3f(1.0f, 1.0f, 1.0f);
            drawSphere(0, 0, 0, 0.1f, 1.0f, 1.0f, 1.0f);
            glDisable(GL_TEXTURE_2D);
        } else {
            glColor3f(1.0f, 0.8f, 0.2f);
            drawSphere(0, 0, 0, 0.1f, 1.0f, 0.8f, 0.2f);
        }
        
        glPopMatrix();
    }
    
    glPopMatrix();
    glEnable(GL_CULL_FACE);
}

void Simple3D::drawPath(float x1, float y1, float z1, float x2, float y2, float z2, float width, float r, float g, float b) {
    glEnable(GL_COLOR_MATERIAL);
    
    // Calculate direction vector
    float dx = x2 - x1;
    float dz = z2 - z1;
    float length = sqrtf(dx*dx + dz*dz);
    
    if (length < 0.001f) return;
    
    dx /= length;
    dz /= length;
    
    float px = -dz * width * 0.5f;
    float pz = dx * width * 0.5f;
    float height = 0.1f;
    
    // Apply brick texture if available
    if (brickTexture > 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, brickTexture);
        glColor3f(1.0f, 1.0f, 1.0f);
    } else {
        glDisable(GL_TEXTURE_2D);
        glColor3f(r, g, b);
    }
    
    glBegin(GL_QUADS);
    
    // Top face with texture
    glNormal3f(0, 1, 0);
    if (brickTexture > 0) {
        float texLength = length / 2.0f;
        glTexCoord2f(0, 0); glVertex3f(x1 + px, y1 + height, z1 + pz);
        glTexCoord2f(texLength, 0); glVertex3f(x2 + px, y2 + height, z2 + pz);
        glTexCoord2f(texLength, 1); glVertex3f(x2 - px, y2 + height, z2 - pz);
        glTexCoord2f(0, 1); glVertex3f(x1 - px, y1 + height, z1 - pz);
    } else {
        glVertex3f(x1 + px, y1 + height, z1 + pz);
        glVertex3f(x2 + px, y2 + height, z2 + pz);
        glVertex3f(x2 - px, y2 + height, z2 - pz);
        glVertex3f(x1 - px, y1 + height, z1 - pz);
    }
    
    // Bottom face
    glNormal3f(0, -1, 0);
    glVertex3f(x1 - px, y1, z1 - pz);
    glVertex3f(x2 - px, y2, z2 - pz);
    glVertex3f(x2 + px, y2, z2 + pz);
    glVertex3f(x1 + px, y1, z1 + pz);
    
    // Side 1
    glNormal3f(px, 0, pz);
    glVertex3f(x1 + px, y1, z1 + pz);
    glVertex3f(x2 + px, y2, z2 + pz);
    glVertex3f(x2 + px, y2 + height, z2 + pz);
    glVertex3f(x1 + px, y1 + height, z1 + pz);
    
    // Side 2
    glNormal3f(-px, 0, -pz);
    glVertex3f(x1 - px, y1 + height, z1 - pz);
    glVertex3f(x2 - px, y2 + height, z2 - pz);
    glVertex3f(x2 - px, y2, z2 - pz);
    glVertex3f(x1 - px, y1, z1 - pz);
    
    glEnd();
    
    if (brickTexture > 0) glDisable(GL_TEXTURE_2D);
    glDisable(GL_COLOR_MATERIAL);
    glColor3f(1.0f, 1.0f, 1.0f);
}

