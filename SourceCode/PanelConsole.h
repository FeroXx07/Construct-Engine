#ifndef _PANELCONSOLE_H
#define _PANELCONSOLE_H
#include "Panel.h"
#include <string>
#include <queue>
#include "singleton.h"
class PanelConsole : public Panel
{
public:
	PanelConsole();
	~PanelConsole();

	void Start();
	void Update();
};

#endif

inline PanelConsole::PanelConsole()
{
}

inline PanelConsole::~PanelConsole()
{
}

inline void PanelConsole::Start()
{
}

inline void PanelConsole::Update()
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
