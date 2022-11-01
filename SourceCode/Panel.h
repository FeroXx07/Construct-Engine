#ifndef _PANEL_H
#define _PANEL_H

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "imgui_stdlib.h"
class Panel
{
public:
	~Panel();

	virtual void Start();
	virtual void Update();

	bool isActive = false;
protected:
	Panel(bool isActive_ = false) : isActive(isActive_){}
};

Panel::~Panel()
{
}

inline void Panel::Start()
{
}

inline void Panel::Update()
{
	
}

#endif