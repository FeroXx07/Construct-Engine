#include "ComponentCamera.h"
#include "GameObject.h"
ComponentCamera::ComponentCamera(const char* name, glm::vec3 position) : Component(ComponentType::CAMERA), m_Name(name)
{
	m_Camera = new Camera(position);
}

ComponentCamera::ComponentCamera(const char* name, Camera* camera_) : Component(ComponentType::CAMERA), m_Name(name), m_Camera(camera_)
{
}

ComponentCamera::~ComponentCamera()
{
	if (m_Camera != nullptr)
	{
		delete m_Camera;
	}

	m_Camera = nullptr;
	glDeleteVertexArrays(1, &m_QuadVAO);
	glDeleteBuffers(1, &m_QuadVBO);
}

void ComponentCamera::GenerateBuffers(Shader* screenShader)
{
	if (screenShader != nullptr)
		m_ScreenShader = screenShader;

	// screen quad VAO
	glGenVertexArrays(1, &m_QuadVAO);
	glGenBuffers(1, &m_QuadVBO);
	glBindVertexArray(m_QuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_QuadVertices), &m_QuadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// framebuffer configuration
	// -------------------------
	glGenFramebuffers(1, &m_Framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);

	// create a color attachment texture
	glGenTextures(1, &m_TextureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, m_TextureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureColorbuffer, 0);

	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	glGenRenderbuffers(1, &m_Rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_Rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1920, 1080); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_Rbo); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LOG("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ComponentCamera::Update()
{
	if (this->m_hasGameObject)
	{
		m_Camera->Position = this->m_GameObject->GetTransformConst()->GetTranslate();
		m_Camera->Yaw = this->m_GameObject->GetTransformConst()->GetRotationEuler().y;
		m_Camera->Pitch = this->m_GameObject->GetTransformConst()->GetRotationEuler().x;
	}
	m_Camera->UpdateCameraVectors();
}

void ComponentCamera::OnEditor()
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
	ImGuiInputTextFlags iflags = ImGuiInputTextFlags_ReadOnly;
	if (ImGui::CollapsingHeader("Camera Information", flags))
	{
		ImGui::Text("Name: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", m_Name.c_str());
		ImGui::Separator();

		ImGui::Text("Zoom: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%f", m_Camera->Zoom);
		ImGui::Separator();

		ImGui::Text("Position: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%f %f %f", m_Camera->Position.x, m_Camera->Position.y, m_Camera->Position.z);
		ImGui::Separator();

		// Enabled?
		ImGui::Checkbox("Show window", &m_isActive);
	}
}

void ComponentCamera::RenderWindow()
{
	if (m_isActive && m_ScreenShader != nullptr)
	{
		ImGui::Begin(m_Name.c_str());
		// now draw the mirror quad with screen texture
		// --------------------------------------------
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
		m_ScreenShader->use();
		glBindVertexArray(m_QuadVAO);
		glBindTexture(GL_TEXTURE_2D, m_TextureColorbuffer);	// use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);

		ImGuiIO& io = ImGui::GetIO();
		ImTextureID my_tex_id = (void*)(intptr_t)m_TextureColorbuffer;
		ImVec2 size = ImGui::GetItemRectSize();
		ImVec2 available = ImGui::GetContentRegionMax();
		float my_tex_w = (float)available.x;
		float my_tex_h = (float)available.y;

		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
		ImVec2 uv_max = ImVec2(1.0f, -1.0f);                 // Lower-right
		ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
		ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
		//// Normal size
		//ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);
		// Half size, same contents
		ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);

		ImGui::End();
	}
}

void ComponentCamera::BindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
}
