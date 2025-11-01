

#include <iostream>
#include <string>

#include <libs/glad.h>
#include <GLFW/glfw3.h>

#include "libs/imgui/imgui.h"
#include "libs/imgui/imgui_impl_glfw.h"
#include "libs/imgui/imgui_impl_opengl3.h"

#include <editor/editor.hpp>
#include <utils/log.hpp>

#define GUI_SCALE ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor())

namespace Editor
{
    
    GLFWwindow *g_main_window;
    
    void destroy() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	return;
    }
    
    void render() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	ImGui::Begin("DearImGui window example");
	
	// ImGui::SliderFloat("Float slider", &f, 0.0f, 1.0f);
	ImGuiIO &io = ImGui::GetIO(); (void)io;
	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	
	ImGui::End();
	
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	return;
    }
    
    void init(GLFWwindow *window) {
	g_main_window = window;
	
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	
	ImGuiIO &io = ImGui::GetIO(); (void)io;
	
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	
	ImGui::StyleColorsDark();
	
	ImGuiStyle &style = ImGui::GetStyle();
	style.ScaleAllSizes(GUI_SCALE);
	style.FontScaleDpi = GUI_SCALE;
	
	ImGui_ImplGlfw_InitForOpenGL(g_main_window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core"); // TODO: add support for more glsl versions.
	
    }
    
}
