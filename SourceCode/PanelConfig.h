#ifndef _PanerlConfig_H
#define _PanerlConfig_H
#include "Panel.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include <vector>

class PanelConfig : public Panel
{
public:
	PanelConfig(ModuleWindow* window, ModuleRenderer3D* renderer, int maxDataHistogram = 60);
	~PanelConfig();

	void Start();
	void Update();

	std::vector<float> m_FpsLog;
	std::vector<float> m_MsLog;

	int m_MaxDataHistogram = 0;
	char m_Title[25];

	ModuleWindow* m_Window;
	ModuleRenderer3D* m_Renderer;
};

inline PanelConfig::PanelConfig(ModuleWindow* window, ModuleRenderer3D* renderer, int maxDataHistogram) : Panel(true), m_Title("")
{
	this->m_Window = window;
	this->m_Renderer = renderer;
	this->m_MaxDataHistogram = maxDataHistogram;
}

inline PanelConfig::~PanelConfig()
{
	m_FpsLog.clear();
	m_MsLog.clear();
	m_Window = nullptr;
	m_Renderer = nullptr;
}



inline void PanelConfig::Start()
{
}

inline void PanelConfig::Update()
{
	ImGuiIO* io = &ImGui::GetIO();
	m_FpsLog.push_back(io->Framerate);
	m_MsLog.push_back(io->Framerate);
	if (m_FpsLog.size() >= m_MaxDataHistogram)
		m_FpsLog.erase(m_FpsLog.begin());
	if (m_MsLog.size() >= m_MaxDataHistogram)
		m_MsLog.erase(m_MsLog.begin());

	Uint32 treeFlags = ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Framed;
	ImGui::Begin("Configuration");
	if (ImGui::TreeNodeEx("Application", treeFlags | ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Max range limit: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), std::to_string(m_MaxDataHistogram).c_str());
		sprintf_s(m_Title, 25, "Framerate %.1f", m_FpsLog[m_FpsLog.size() - 1]);
		ImGui::PlotHistogram("##framerate", &m_FpsLog[0], m_FpsLog.size(), 0, m_Title, 0.0f, 100.0f, ImVec2(310, 100));
		sprintf_s(m_Title, 25, "Milliseconds %.1f", m_MsLog[m_MsLog.size() - 1]);
		ImGui::PlotHistogram("##milliseconds", &m_MsLog[0], m_MsLog.size(), 0, m_Title, 0.0f, 100.0f, ImVec2(310, 100));
		ImGui::TreePop();
	}
	if (ImGui::TreeNodeEx("Window", treeFlags))
	{
		int height, width;
		m_Window->GetScreenSize(width, height);
		float b = m_Window->GetBrightness();

		ImGui::SliderFloat("Brightness", &b, 0.0f, 1.0f, "%.1f");
		if (b != m_Window->GetBrightness())
			m_Window->SetBrightness(b);

		ImGui::SliderInt("Width", &width, 640, 1920);
		ImGui::SliderInt("Height", &height, 480, 1080);
		if (width != m_Window->GetScreenWidth() || height != m_Window->GetScreenHeight())
			m_Window->SetScreenSize(width, height);

		ImGui::Text("Refresh rate: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), std::to_string(m_MsLog[m_MsLog.size() - 1]).c_str());

		bool fullscreen = m_Window->GetFullScreen();
		bool resizable = m_Window->GetResizable();
		bool borderless = m_Window->GetBorderless();
		bool fullscreenDesktop = m_Window->GetFullScreen_Desktop();

		if (ImGui::Checkbox("Fullscreen", &fullscreen))
			m_Window->SetFullScreen(fullscreen);

		ImGui::SameLine();
		if (ImGui::Checkbox("Resizable", &resizable))
			m_Window->SetResizable(resizable);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Restart to apply");

		if (ImGui::Checkbox("Borderless", &borderless))
			m_Window->SetBorderless(borderless);

		ImGui::SameLine();
		if (ImGui::Checkbox("Fullscreen Desktop", &fullscreenDesktop))
			m_Window->SetFullScreen_Desktop(fullscreenDesktop);
		ImGui::TreePop();
	}

	char sdlVer[25];
	SDL_version ver; SDL_GetVersion(&ver);

	treeFlags = ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Framed;
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
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%dGb", SDL_GetSystemRAM() / 1000);
		ImGui::Separator();

		ImGui::Text("Glew Version: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), m_Renderer->glewVersion.c_str());
		ImGui::Separator();

		ImGui::Text("Vendor : ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), m_Renderer->vendor.c_str());
		ImGui::Separator();

		ImGui::Text("Renderer : ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), m_Renderer->rendInfo.c_str());
		ImGui::Separator();

		ImGui::Text("Supported OpenGL version : ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), m_Renderer->supportedOpenGLversion.c_str());
		ImGui::Separator();

		ImGui::Text("GLSL version : ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), m_Renderer->GLSL.c_str());
		ImGui::Separator();

		ImGui::TreePop();

	}
	ImGui::End();
}

#endif