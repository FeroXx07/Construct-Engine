
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

class ModuleUI : public Module
{
public:
	ModuleUI(Application* app, bool start_enabled = true);
	~ModuleUI();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void ConfigWindowUpdate();
	void ConfigHardwareUpdate();
	void ConsoleWindowUpdate();

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

