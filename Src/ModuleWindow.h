#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include <SDL.h>
#include <string>

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	bool RestartWindow();

	// Json methods
	void SaveJson();
	bool LoadJson();
public:
	//The window we'll be rendering to
	SDL_Window* window;

private:
	//The surface contained by the window
	SDL_Surface* screen_surface;
	int width, height;
	int screenSizeMultiplier = 1;
	float brightness = 1.0f;

	// Boolean for flags
	bool isFullScreen = false;
	bool isResizable = true;
	bool isBorderless = false;
	bool isFullScreen_Desktop = false;
	bool isVsync = true;
	std::string title;

public:
	// Setters 

	void SetTitle(const char* title);
	void SetScreenSize(int width, int height);
	void SetFullScreen(bool value);
	void SetResizable(bool value);
	void SetBorderless(bool value);
	void SetFullScreen_Desktop(bool value);
	void SetVsync(bool value);
	void SetBrightness(float value);

	// Getters

	const char* GetTitle();
	void GetScreenSize(int& width, int& height);
	int GetScreenWidth();
	int GetScreenHeight();
	bool GetFullScreen();
	bool GetResizable();
	bool GetBorderless();
	bool GetFullScreen_Desktop();
	bool GetVsync();
	int GetScreenSizeMultiplier();
	float GetBrightness();
};

#endif // __ModuleWindow_H__