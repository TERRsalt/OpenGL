#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "debugUi.hpp"
#include "camera.hpp"
#include "time.hpp"

namespace debugUi {
    void init(GLFWwindow *window) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsLight();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    void remove() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void debug() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Debug");

        ImGui::Text("FPS: %.1f", 1.0f / gameTime.deltaTime);

        ImGui::Text("Camera position: %.2f, %.2f, %.2f", camera.position.x, camera.position.y, camera.position.z);
        ImGui::Text("Camera front: %.2f, %.2f, %.2f", camera.front.x, camera.front.y, camera.front.z);

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}