#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
	screenSizeMultiplier = 1;
	width = 1280;
	height = 1024;
	title = "Game Engine";
	isVsync = false;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	LoadJson();

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		width = width * screenSizeMultiplier;
		height = height * screenSizeMultiplier;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		// GL 3.0 + GLSL 130
		const char* glsl_version = "#version 130";
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

		// Create window with graphics context
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

		SDL_GLContext gl_context = SDL_GL_CreateContext(App->window->window);
		SDL_GL_MakeCurrent(App->window->window, gl_context);
		SDL_GL_SetSwapInterval(1); // Enable vsync
		////Use OpenGL 2.1
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(isFullScreen == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(isResizable == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(isBorderless == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(isFullScreen_Desktop == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		flags |= SDL_WINDOW_OPENGL;
		flags |= SDL_WINDOW_ALLOW_HIGHDPI;

		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
			SDL_SetWindowBrightness(window, brightness);
		}
	}

	SaveJson();

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}


void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::SetScreenSize(int width, int height)
{
	this->width = width;
	this->height = height;
	SaveJson();
}

void ModuleWindow::SetFullScreen(bool value)
{
	isFullScreen = value;
	Uint32 currentFlags = SDL_GetWindowFlags(window);
	if (isFullScreen)
		currentFlags |= SDL_WINDOW_BORDERLESS;
	else
		currentFlags &= ~SDL_WINDOW_BORDERLESS;
	
	int a = SDL_SetWindowFullscreen(window, currentFlags);
	SaveJson();
}

void ModuleWindow::SetResizable(bool value)
{
	isResizable = value;
	SaveJson();
}

void ModuleWindow::SetBorderless(bool value)
{
	isBorderless = value;
	SaveJson();
}

void ModuleWindow::SetFullScreen_Desktop(bool value)
{
	isFullScreen_Desktop = value;
	SaveJson();
}

void ModuleWindow::SetVsync(bool value)
{
	isVsync = value;
	SaveJson();
}

void ModuleWindow::SetBrightness(float value)
{
	brightness = value;
	SDL_SetWindowBrightness(window, brightness);
	SaveJson();
}

const char* ModuleWindow::GetTitle()
{
	return title.c_str();
}

void ModuleWindow::GetScreenSize(int& width, int& height)
{
	width = this->width;
	height = this->height;
}

int ModuleWindow::GetScreenWidth() { return width; }

int ModuleWindow::GetScreenHeight() { return height; }

bool ModuleWindow::GetFullScreen() { 
	return isFullScreen; 
}

bool ModuleWindow::GetResizable() { return isResizable; }

bool ModuleWindow::GetBorderless() { return isBorderless; }

bool ModuleWindow::GetFullScreen_Desktop() { return isFullScreen_Desktop; }

bool ModuleWindow::GetVsync() { return isVsync; }

int ModuleWindow::GetScreenSizeMultiplier() { return screenSizeMultiplier; }

float ModuleWindow::GetBrightness()
{
	return brightness;
}

void ModuleWindow::SaveJson()
{
	// Create null value
	json jSettings;

	// STL container of an object {"key":"value"}
	std::unordered_map<std::string, bool> mapSettings;
	mapSettings.insert({ getName(isFullScreen), isFullScreen});
	mapSettings.insert({ getName(isResizable), isResizable });
	mapSettings.insert({ getName(isBorderless), isBorderless });
	mapSettings.insert({ getName(isFullScreen_Desktop), isFullScreen_Desktop });
	mapSettings.insert({ getName(isVsync), isVsync });

	// automatic transformation of STL containers in nlohmann Json
	json j_Object(mapSettings);
	jSettings = j_Object;

	// add via member function
	jSettings.push_back({ "title", title.c_str() });
	jSettings.emplace( getName(brightness), brightness);

	// add via += operator
	jSettings += {getName(width), width};

	// add via key
	jSettings[getName(height)] = height;

	// default serialization 
	auto s1 = jSettings.dump();
	std::cout << jSettings << std::endl;
	
	std::ofstream file("jSettings.json");
	file << jSettings;
}

void ModuleWindow::LoadJson()
{
	std::ifstream f("jSettings.json");
	if (f.is_open())
	{
		json data = json::parse(f);
		isFullScreen = data[getName(isFullScreen)];
		isResizable = data[getName(isResizable)];
		isBorderless = data[getName(isBorderless)];
		isFullScreen_Desktop = data[getName(isFullScreen_Desktop)];
		isVsync = data[getName(isVsync)];
		width = data[getName(width)];
		height = data[getName(height)];
		brightness = data[getName(brightness)];
	}
}
