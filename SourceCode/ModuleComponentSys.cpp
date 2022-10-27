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

//void ModuleComponentSys::DrawGameObject(Shader& shader, GameObject* node, bool dirty)
//{
//	//bool isRoot = false;
//	//if (node->GetParent() == nullptr) { isRoot = true; };
//	//
//	//if (!isRoot)
//	//{	
//	//	ComponentTransform* transform = node->GetTransform();
//
//	//	dirty = transform->m_Dirty;
//	//	if (dirty)
//	//	{
//	//		// world_ = local_.combine(parentWorld);
//	//		transform->m_World = transform->GetCombination(node->GetParent()->GetTransform()->m_World);
//	//		transform->m_Dirty = false;
//	//	}
//
//	//	if (node->m_HasComponentMesh)
//	//	{
//	//		ComponentMesh* mesh = node->GetMesh();
//	//		shader.setMat4("model", transform->m_World);
//	//		mesh->GetMesh()->RenderMesh(shader);
//	//	}
//	//}
//	
//	
//	
//}

void ModuleComponentSys::DrawGameObject(Shader& shader, GameObject* node, glm::mat4x4 worldTransform)
{
	bool isRoot = false;
	if (node->GetParent() == nullptr)
		isRoot = true;
	glm::mat4x4 world = worldTransform;
	if (!isRoot)
	{
		ComponentTransform* transform = node->GetTransform();
		world = transform->GetCombination(node->GetParent()->GetTransform()->m_World);
		if (node->m_HasComponentMesh)
		{
			ComponentMesh* mesh = node->GetMesh();
			shader.setMat4("model", world);
			mesh->GetMesh()->RenderMesh(shader);

			if (mesh->m_DisplayNormals)
			{
				glm::mat4 view = App->camera->currentCamera->GetViewMatrix();
				glMatrixMode(GL_PROJECTION);
				glLoadMatrixf((const GLfloat*)&App->scene->projection[0]);
				glMatrixMode(GL_MODELVIEW);
				glm::mat4 MV = view * transform->m_TransMat;
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
				/*for (int i = 0; i < mesh->GetMesh()->indices.size(); i++) {
					glm::vec3 p = mesh->GetMesh()->vertices[mesh->GetMesh()->indices]];
					glVertex3fv(&p.x);
					glm::vec3 o = glm::normalize(indexed_normals[indices[i]]);
					p += o * 0.1f;
					glVertex3fv(&p.x);
				}*/
				glEnd();
				glLineWidth(1.0f);
			}
			shader.use();
		}
	}
	for (auto c : node->m_Children)
	{
		DrawGameObject(shader, c, world);
	}
	
}


bool ModuleComponentSys::CleanUp()
{
	/*if (m_MeshSystem != nullptr)
	{
		delete m_MeshSystem;
		m_MeshSystem = nullptr;
	}*/
	return true;
}

