#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "Module.h"
#include "Globals.h"
#include <vector>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>

class PanelConfig;
class PanelConsole;
class PanelHierarchy;
class PanelInspector;
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

	// Set ups the dark custom theme for ImGui
	void SetupImGuiStyle();
public:
	// Our state
	ImGuiIO* io = nullptr;
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 0.00f);
	
	std::string licenseStr;
	
	GameObject* m_CurrentSelectedNode = nullptr;

	// Updates the window of the configuration of the window options
	// Updates the window of the configuration of the hardware options
	PanelConfig* m_PanelConfig = nullptr;

	// Updates the window of the configuration of the console logs
	PanelConsole* m_PanelConsole = nullptr;

	// Updates the window of the nodes displayer
	PanelHierarchy* m_PanelHierarchy = nullptr;

	// Updates the window of the components of the selected node
	PanelInspector* m_PanelInspector = nullptr;

	void MenuBarUpdate();
};

#endif