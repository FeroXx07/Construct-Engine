//#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif
#include "singleton.h"
ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled), title("")
{
}

ModuleEditor::~ModuleEditor()
{}

// Load assets
bool ModuleEditor::Start()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = &	ImGui::GetIO(); (void)io;
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init("#version 130");
//
//	// Load Fonts
//	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
//	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
//	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
//	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
//	// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
//	// - Read 'docs/FONTS.md' for more instructions and details.
//	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
//	//io.Fonts->AddFontDefault();
//	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
//	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
//	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
//	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
//	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
//	//IM_ASSERT(font != NULL);
//
	ImFont* font = io->Fonts->AddFontFromFileTTF("SourceCode/imgui/fonts/Roboto-Regular.ttf", 16.0f);
	IM_ASSERT(font != NULL);

	maxDataHistogram = 60;

	std::ifstream f("LICENSE"); //taking file as inputstream
	if (f) {
		std::ostringstream ss;
		ss << f.rdbuf(); // reading data
		licenseStr = ss.str();
	}

	show_demo_window = false;

	SetupImGuiStyle();
	return true;
}

void ModuleEditor::SetupImGuiStyle()
{
	// Dark Ruda style by Raikiri from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0;
	style.DisabledAlpha = 0.6000000238418579;
	style.WindowPadding = ImVec2(8.0, 8.0);
	style.WindowRounding = 0.0;
	style.WindowBorderSize = 1.0;
	style.WindowMinSize = ImVec2(32.0, 32.0);
	style.WindowTitleAlign = ImVec2(0.0, 0.5);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 0.0;
	style.ChildBorderSize = 1.0;
	style.PopupRounding = 0.0;
	style.PopupBorderSize = 1.0;
	style.FramePadding = ImVec2(4.0, 3.0);
	style.FrameRounding = 4.0;
	style.FrameBorderSize = 0.0;
	style.ItemSpacing = ImVec2(8.0, 4.0);
	style.ItemInnerSpacing = ImVec2(4.0, 4.0);
	style.CellPadding = ImVec2(4.0, 2.0);
	style.IndentSpacing = 21.0;
	style.ColumnsMinSpacing = 6.0;
	style.ScrollbarSize = 14.0;
	style.ScrollbarRounding = 9.0;
	style.GrabMinSize = 10.0;
	style.GrabRounding = 4.0;
	style.TabRounding = 4.0;
	style.TabBorderSize = 0.0;
	style.TabMinWidthForCloseButton = 0.0;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5, 0.5);
	style.SelectableTextAlign = ImVec2(0.0, 0.0);

	style.Colors[ImGuiCol_Text] = ImVec4(0.9490196108818054, 0.95686274766922, 0.9764705896377563, 1.0);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.3568627536296844, 0.4196078479290009, 0.4666666686534882, 1.0);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1098039224743843, 0.1490196138620377, 0.168627455830574, 1.0);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.1490196138620377, 0.1764705926179886, 0.2196078449487686, 1.0);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261, 0.0784313753247261, 0.0784313753247261, 0.9399999976158142);
	style.Colors[ImGuiCol_Border] = ImVec4(0.0784313753247261, 0.09803921729326248, 0.1176470592617989, 1.0);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0, 0.0, 0.0, 0.0);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2000000029802322, 0.2470588237047195, 0.2862745225429535, 1.0);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1176470592617989, 0.2000000029802322, 0.2784313857555389, 1.0);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.08627451211214066, 0.1176470592617989, 0.1372549086809158, 1.0);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.08627451211214066, 0.1176470592617989, 0.1372549086809158, 0.6499999761581421);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0784313753247261, 0.09803921729326248, 0.1176470592617989, 1.0);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0, 0.0, 0.0, 0.5099999904632568);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1490196138620377, 0.1764705926179886, 0.2196078449487686, 1.0);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.01960784383118153, 0.01960784383118153, 0.01960784383118153, 0.3899999856948853);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.2000000029802322, 0.2470588237047195, 0.2862745225429535, 1.0);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1764705926179886, 0.2196078449487686, 0.2470588237047195, 1.0);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.08627451211214066, 0.2078431397676468, 0.3098039329051971, 1.0);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.2784313857555389, 0.5568627715110779, 1.0, 1.0);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.2784313857555389, 0.5568627715110779, 1.0, 1.0);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.3686274588108063, 0.6078431606292725, 1.0, 1.0);
	style.Colors[ImGuiCol_Button] = ImVec4(0.2000000029802322, 0.2470588237047195, 0.2862745225429535, 1.0);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.2784313857555389, 0.5568627715110779, 1.0, 1.0);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.05882352963089943, 0.529411792755127, 0.9764705896377563, 1.0);
	style.Colors[ImGuiCol_Header] = ImVec4(0.2000000029802322, 0.2470588237047195, 0.2862745225429535, 0.550000011920929);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.2588235437870026, 0.5882353186607361, 0.9764705896377563, 0.800000011920929);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.2588235437870026, 0.5882353186607361, 0.9764705896377563, 1.0);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.2000000029802322, 0.2470588237047195, 0.2862745225429535, 1.0);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.09803921729326248, 0.4000000059604645, 0.7490196228027344, 0.7799999713897705);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.09803921729326248, 0.4000000059604645, 0.7490196228027344, 1.0);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.2588235437870026, 0.5882353186607361, 0.9764705896377563, 0.25);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.2588235437870026, 0.5882353186607361, 0.9764705896377563, 0.6700000166893005);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.2588235437870026, 0.5882353186607361, 0.9764705896377563, 0.949999988079071);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.1098039224743843, 0.1490196138620377, 0.168627455830574, 1.0);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.2588235437870026, 0.5882353186607361, 0.9764705896377563, 0.800000011920929);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.2000000029802322, 0.2470588237047195, 0.2862745225429535, 1.0);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.1098039224743843, 0.1490196138620377, 0.168627455830574, 1.0);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1098039224743843, 0.1490196138620377, 0.168627455830574, 1.0);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.6078431606292725, 0.6078431606292725, 0.6078431606292725, 1.0);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0, 0.4274509847164154, 0.3490196168422699, 1.0);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8980392217636108, 0.6980392336845398, 0.0, 1.0);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0, 0.6000000238418579, 0.0, 1.0);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104, 0.1882352977991104, 0.2000000029802322, 1.0);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971, 0.3098039329051971, 0.3490196168422699, 1.0);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832, 0.2274509817361832, 0.2470588237047195, 1.0);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0, 0.0, 0.0, 0.0);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0, 1.0, 1.0, 0.05999999865889549);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2588235437870026, 0.5882353186607361, 0.9764705896377563, 0.3499999940395355);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0, 1.0, 0.0, 0.8999999761581421);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026, 0.5882353186607361, 0.9764705896377563, 1.0);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0, 1.0, 1.0, 0.699999988079071);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929, 0.800000011920929, 0.800000011920929, 0.2000000029802322);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929, 0.800000011920929, 0.800000011920929, 0.3499999940395355);
}

// Load assets
bool ModuleEditor::CleanUp()
{

	return true;
}

void ModuleEditor::ConfigWindowUpdate()
{
	Uint32 treeFlags = ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Framed;
	ImGui::Begin("Configuration");
	if (ImGui::TreeNodeEx("Application", treeFlags | ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Max range limit: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), std::to_string(maxDataHistogram).c_str());
		sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
		ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
		sprintf_s(title, 25, "Milliseconds %.1f", ms_log[ms_log.size() - 1]);
		ImGui::PlotHistogram("##milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
		ImGui::TreePop();
	}
	if (ImGui::TreeNodeEx("Window", treeFlags))
	{
		int height, width;
		App->window->GetScreenSize(width, height);
		float b = App->window->GetBrightness();

		ImGui::SliderFloat("Brightness", &b, 0.0f, 1.0f, "%.1f");
		if (b != App->window->GetBrightness())
			App->window->SetBrightness(b);

		ImGui::SliderInt("Width", &width, 640, 1920);
		ImGui::SliderInt("Height", &height, 480, 1080);
		if (width != App->window->GetScreenWidth() || height != App->window->GetScreenHeight())
			App->window->SetScreenSize(width, height);

		ImGui::Text("Refresh rate: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), std::to_string(ms_log[ms_log.size() - 1]).c_str());

		bool fullscreen = App->window->GetFullScreen();
		bool resizable = App->window->GetResizable();
		bool borderless = App->window->GetBorderless();
		bool fullscreenDesktop = App->window->GetFullScreen_Desktop();

		if (ImGui::Checkbox("Fullscreen", &fullscreen)) 
			App->window->SetFullScreen(fullscreen);

		ImGui::SameLine();
		if (ImGui::Checkbox("Resizable", &resizable))
			App->window->SetResizable(resizable);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Restart to apply");

		if (ImGui::Checkbox("Borderless", &borderless))
			App->window->SetBorderless(borderless);

		ImGui::SameLine();
		if (ImGui::Checkbox("Fullscreen Desktop", &fullscreenDesktop))
			App->window->SetFullScreen_Desktop(fullscreenDesktop);
		ImGui::TreePop();
	}
	//ImGui::End();
	
}

void ModuleEditor::ConfigHardwareUpdate()
{
	char sdlVer[25];
	SDL_version ver; SDL_GetVersion(&ver);
	
	Uint32 treeFlags = ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Framed;
	if (ImGui::TreeNodeEx("Hardware", treeFlags))
	{
		ImGui::Text("SDL Version: "); 
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d.%d.%d", ver.major, ver.minor, ver.patch);
		ImGui::Separator();

		ImGui::Text("CPUs: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d (Cache: %dkb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
		ImGui::Separator();

		ImGui::Text("System RAM: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%dGb", SDL_GetSystemRAM()/1000);

		ImGui::TreePop();
	}
	ImGui::End();
}

void ModuleEditor::ConsoleWindowUpdate()
{
	ImGui::Begin("Console");
	std::queue<std::string> temp = logger.GetInstance()->queue;
	while (!temp.empty())
	{
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "[LOG]");
		//ImGui::SameLine();
		ImGui::TextWrapped(temp.front().c_str());
		ImGui::Separator();
		temp.pop();
	}
	ImGui::End();
}

update_status ModuleEditor::PreUpdate(float dt)
{
	fps_log.push_back(io->Framerate);
	ms_log.push_back(io->Framerate);
	if (fps_log.size() >= maxDataHistogram)
		fps_log.erase(fps_log.begin());
	if (ms_log.size() >= maxDataHistogram)
		ms_log.erase(ms_log.begin());
	//ImGui::PlotHistogram()

	return UPDATE_CONTINUE;
}

// Update
update_status ModuleEditor::Update(float dt)
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	//// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	//{
	//	static float f = 0.0f;
	//	static int counter = 0;

	//	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	//	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	//	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	//	ImGui::Checkbox("Another Window", &show_another_window);

	//	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	//	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	//	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	//		counter++;
	//	ImGui::SameLine();
	//	ImGui::Text("counter = %d", counter);

	//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//	ImGui::End();
	//}

	//// 3. Show another simple window.
	//if (show_another_window)
	//{
	//	ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	//	ImGui::Text("Hello from another window!");
	//	if (ImGui::Button("Close Me"))
	//		show_another_window = false;
	//	ImGui::End();
	//}

	// Top Menu Bar
	ImGui::BeginMainMenuBar();
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), dockspace_flags);
	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("Gui Demo"))
			show_demo_window = !show_demo_window;

		if (ImGui::MenuItem("Web Page"))
			App->RequestBrowser("https://github.com/FeroXx07/Game-Engine");

		//if (ImGui::MenuItem("About"))
		//	ImGui::OpenPopup("About"); // Set pop up id

		//// Always center this window when appearing
		//ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		//ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		//// Pop up id required
		//if (ImGui::BeginPopupModal("About", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		//{
		//	ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");
		//	ImGui::Separator();

		//	//static int unused_i = 0;
		//	//ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

		//	static bool dont_ask_me_next_time = false;
		//	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		//	ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
		//	ImGui::PopStyleVar();

		//	if (ImGui::Button("Got it", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		//	ImGui::SetItemDefaultFocus();
		//	//ImGui::SameLine();
		//	//if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		//	//ImGui::EndPopup();
		//}
		if (ImGui::Button("About"))
			ImGui::OpenPopup("About...");

		// Always center this window when appearing
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginPopupModal("About...", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("The engine currently doesn't have a name\n");
			ImGui::Text("The sure thing is that it will be 3D Game Engine\n");
			ImGui::Text("Make sure to visit the github page: https://github.com/FeroXx07/Game-Engine\n");
			ImGui::Text(licenseStr.c_str());
			
			ImGui::Separator();

			//static int unused_i = 0;
			//ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

			/*static bool dont_ask_me_next_time = false;
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
			ImGui::PopStyleVar();*/

			if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		/*	ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();*/
		}
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();

	ConfigWindowUpdate();
	ConfigHardwareUpdate();
	ConsoleWindowUpdate();
	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(float dt)
{
	// Rendering
	ImGui::Render();
	//io = ImGui::GetIO(); (void)io;
	glViewport(0, 0, (GLsizei)io->DisplaySize.x, (GLsizei)io->DisplaySize.y);
	/*glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);*/
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
	if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}
	return UPDATE_CONTINUE;
}