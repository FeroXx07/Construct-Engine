#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
//#if defined(IMGUI_IMPL_OPENGL_ES2)
//#include <SDL_opengles2.h>
//#else
//#include <SDL_opengl.h>
//#endif
#include <gl/glew.h>
#include <gl/GLU.h>
#include <gl/GL.h>
//#include <gl/GLU.h>


#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "glew32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "glew32s.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled), context(NULL)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if(ret == true)
	{
		//Use Vsync
		if(App->window->GetVsync() && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
		
		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		LOG("Using Glew %s", glewGetString(GLEW_VERSION));
		// Should be 2.0
		LOG("Vendor: %s", glGetString(GL_VENDOR));
		LOG("Renderer: %s", glGetString(GL_RENDERER));
		const char * ab = LOGr("OpenGL version supported %s", glGetString(GL_VERSION));
		LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}

	// Projection matrix for
	int height, width;
	App->window->GetScreenSize(width, height);
	OnResize(width, height);

	//InitImGui();
	//App->uiManager->EnableAndInit();
	return ret;
}

//void ModuleRenderer3D::InitImGui()
//{
//	// Decide GL+GLSL versions
//#if defined(IMGUI_IMPL_OPENGL_ES2)
//	// GL ES 2.0 + GLSL 100
//	const char* glsl_version = "#version 100";
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
//#elif defined(__APPLE__)
//	// GL 3.2 Core + GLSL 150
//	const char* glsl_version = "#version 150";
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
//#else
//	//// GL 3.0 + GLSL 130
//	//const char* glsl_version = "#version 130";
//	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
//	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
//	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
//	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
//#endif
//
//	// Create window with graphics context
//	//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
//	//SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
//	//SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
//	//SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
//	//SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
//	//SDL_GLContext gl_context = SDL_GL_CreateContext(App->window->window);
//	//SDL_GL_MakeCurrent(App->window->window, gl_context);
//	//SDL_GL_SetSwapInterval(1); // Enable vsync
//
//	// Setup Dear ImGui context
//	IMGUI_CHECKVERSION();
//	ImGui::CreateContext();
//	io = &	ImGui::GetIO(); (void)io;
//	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
//	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
//	io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
//	io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
//	//io.ConfigViewportsNoAutoMerge = true;
//	//io.ConfigViewportsNoTaskBarIcon = true;
//
//	// Setup Dear ImGui style
//	ImGui::StyleColorsDark();
//	//ImGui::StyleColorsLight();
//
//	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
//	ImGuiStyle& style = ImGui::GetStyle();
//	if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
//	{
//		style.WindowRounding = 0.0f;
//		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
//	}
//
//	// Setup Platform/Renderer backends
//	ImGui_ImplSDL2_InitForOpenGL(App->window->window, SDL_GL_GetCurrentContext());
//	ImGui_ImplOpenGL3_Init("#version 130");
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
//}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();



	return UPDATE_CONTINUE;
}

//void ModuleRenderer3D::UpdateImGui()
//{
//	// Start the Dear ImGui frame
//	ImGui_ImplOpenGL3_NewFrame();
//	ImGui_ImplSDL2_NewFrame();
//	ImGui::NewFrame();
//
//	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
//	if (show_demo_window)
//		ImGui::ShowDemoWindow(&show_demo_window);
//
//	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
//	{
//		static float f = 0.0f;
//		static int counter = 0;
//
//		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
//
//		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
//		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
//		ImGui::Checkbox("Another Window", &show_another_window);
//
//		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
//		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
//
//		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
//			counter++;
//		ImGui::SameLine();
//		ImGui::Text("counter = %d", counter);
//
//		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//		ImGui::End();
//	}
//
//	// 3. Show another simple window.
//	if (show_another_window)
//	{
//		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
//		ImGui::Text("Hello from another window!");
//		if (ImGui::Button("Close Me"))
//			show_another_window = false;
//		ImGui::End();
//	}
//
//	// Rendering
//	ImGui::Render();
//	//io = ImGui::GetIO(); (void)io;
//	glViewport(0, 0, io->DisplaySize.x, io->DisplaySize.y);
//	/*glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
//	glClear(GL_COLOR_BUFFER_BIT);*/
//	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//	// Update and Render additional Platform Windows
//	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
//	//  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
//	if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
//	{
//		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
//		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
//		ImGui::UpdatePlatformWindows();
//		ImGui::RenderPlatformWindowsDefault();
//		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
//	}
//}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	//UpdateImGui();

	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	//// Cleanup
	//ImGui_ImplOpenGL3_Shutdown();
	//ImGui_ImplSDL2_Shutdown();
	//ImGui::DestroyContext();

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
