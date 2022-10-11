
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#pragma once
#include "Module.h"
#include "Globals.h"
#include <vector>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>

class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	// Updates the window of the configuration of the window options
	void WindConfigWind();
	// Updates the window of the configuration of the hardware options
	void WindHardware();
	// Updates the window of the configuration of the console logs
	void WindConsole();
	// Set ups the dark custom theme for ImGui
	void SetupImGuiStyle();
public:
	// Our state
	ImGuiIO* io = nullptr;
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 0.00f);

private:
	std::vector<float> fps_log;
	std::vector<float> ms_log;
	char title[25];
	int maxDataHistogram = 0;

	std::string licenseStr;
	
};

