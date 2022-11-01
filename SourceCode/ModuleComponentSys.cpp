#include "Application.h"
#include "ModuleComponentSys.h"
#include "GameObject.h"
#include "ModuleCamera3D.h"

ModuleComponentSys::ModuleComponentSys(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	//m_MeshSystem = nullptr;
}

ModuleComponentSys::~ModuleComponentSys()
{
}

bool ModuleComponentSys::Start()
{
	//m_MeshSystem = new MeshSystem();
	return true;
}

update_status ModuleComponentSys::Update(float dt)
{
	return UPDATE_CONTINUE;
}


void ModuleComponentSys::DrawGameObject(Shader& shader, GameObject* node, glm::mat4x4 parentWorld)
{
	bool isRoot = false;
	if (node->GetParent() == nullptr)
		isRoot = true;
	glm::mat4x4 world = parentWorld;
	if (!isRoot)
	{
		ComponentTransform* local = node->GetTransform();
		local->Update();
		world = local->Combine(parentWorld);
		local->SetWorldMatrix(world);
		if (node->m_Name == "Cube")
			LOG("Cube");
		
		if (node->m_HasComponentMesh)
		{
			ComponentMesh* mesh = node->GetMesh();
			shader.setMat4("model", world);
			if (node->m_HasComponentMaterial)
			{
				ComponentMaterial* material = node->GetMaterial();
				mesh->GetMesh()->RenderMesh(shader, material);
			}

			DrawNormals(mesh, local);
			shader.use();
		}
	}
	for (auto c : node->m_Children)
	{
		DrawGameObject(shader, c, world);
	}
	
}

void ModuleComponentSys::AddTextureToGameObject(string const& path)
{
	if (App->uiManager->m_CurrentSelectedNode != nullptr)
		App->scene->m_ModelLoader->LoadTextureIntoGameObject(path, App->uiManager->m_CurrentSelectedNode);
}


void ModuleComponentSys::DrawNormals(ComponentMesh* mesh, ComponentTransform* transform)
{
	if (mesh->m_DisplayNormals)
	{
		glm::mat4 view = App->camera->currentCamera->GetViewMatrix();
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf((const GLfloat*)&App->scene->projection[0]);
		glMatrixMode(GL_MODELVIEW);
		glm::mat4 MV = view * transform->GetWorld();
		glLoadMatrixf((const GLfloat*)&MV[0]);
		glUseProgram(0);
		glColor3f(0, 0, 1);
		glLineWidth(2.0f);
		glBegin(GL_LINES);
		for (auto index : mesh->GetMesh()->indices)
		{
			glm::vec3 p = mesh->GetMesh()->vertices[index].m_Position;
			glVertex3f(p.x, p.y, p.z);
			glm::vec3 o = glm::normalize(mesh->GetMesh()->vertices[index].m_Normal);
			p += o * 0.1f;
			glVertex3f(p.x, p.y, p.z);
		}
		glEnd();
		glLineWidth(1.0f);
	}
}

void ModuleComponentSys::UpdateAllTransforms(GameObject* rootNode)
{
}



bool ModuleComponentSys::CleanUp()
{
	return true;
}

