#include "imgui_setup.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include <iostream>

void setupImGui(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    ImGui::StyleColorsDark();
    
    // Initialize ImGui with manual callback setup
    ImGui_ImplGlfw_InitForOpenGL(window, false);
    ImGui_ImplOpenGL2_Init();
    
    // Manually install callbacks
    glfwSetMouseButtonCallback(window, ImGui_ImplGlfw_MouseButtonCallback);
    glfwSetScrollCallback(window, ImGui_ImplGlfw_ScrollCallback);
    glfwSetKeyCallback(window, ImGui_ImplGlfw_KeyCallback);
    glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);
    
    // Ensure proper OpenGL state
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void cleanupImGui() {
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void renderImGui() {
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

bool showInputWindow(int& numMerry, int& numFerris, int& numStalls, int& numPonds, bool& autoGenerate) {
    static bool showWindow = true;
    static bool inputComplete = false;
    
    if (!showWindow) return inputComplete;
    
    // Center the window
    ImGui::SetNextWindowPos(ImVec2(400, 200), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Always);
    
    if (ImGui::Begin("Theme Park Builder", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
        ImGui::Text("Configure your theme park:");
        ImGui::Separator();
        
        ImGui::Checkbox("Auto Generate", &autoGenerate);
        ImGui::Separator();
        
        if (!autoGenerate) {
            if (numMerry < 0) numMerry = 0;
            if (numFerris < 0) numFerris = 0;
            if (numStalls < 0) numStalls = 0;
            if (numPonds < 0) numPonds = 0;
            
            ImGui::InputInt("Merry-Go-Rounds", &numMerry);
            ImGui::InputInt("Ferris Wheels", &numFerris);
            ImGui::InputInt("Stalls", &numStalls);
            ImGui::InputInt("Ponds", &numPonds);
        } else {
            numMerry = 2; numFerris = 1; numStalls = 3; numPonds = 2;
            ImGui::Text("Auto: 2 Carousels, 1 Ferris, 3 Stalls, 2 Ponds");
        }
        
        ImGui::Separator();
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 150) * 0.5f);
        if (ImGui::Button("Generate Park", ImVec2(150, 40))) {
            std::cout << "Button clicked!" << std::endl;
            inputComplete = true;
            showWindow = false;
        }
        
        // Debug: Show if button is hovered
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Click to generate your park!");
        }
        
        // Debug: Show mouse position
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Text("Mouse: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
        ImGui::Text("Mouse clicked: %s", io.MouseClicked[0] ? "YES" : "NO");
    }
    ImGui::End();
    
    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    
    return inputComplete;
}