#include "ComponentTransform.h"
#include "GameObject.h"

ComponentTransform::ComponentTransform(glm::vec3 translation, glm::vec3 scaling, glm::quat rotation) : m_Translation(translation), m_Scaling(scaling), m_Rotation(rotation)
{
	m_LocalMat = glm::mat4x4(1.0f);
	m_Dirty = false;

	m_WorldTranslation = glm::vec3(0.0f, 0.0f, 0.0f);
	m_WorldScaling = glm::vec3(0.0f, 0.0f, 0.0f);
	m_WorldRotation = quat(0.0f, 0.0f, 0.0f, 0.0f);
	m_WorldMat = glm::mat4x4(1.0f);

	// In code we first translate and then later rotate, the actual transformations first apply a rotation and then a translation.
	m_LocalMat = glm::translate(m_LocalMat, m_Translation);
	m_LocalMat = glm::scale(m_LocalMat, m_Scaling);
	m_LocalMat = glm::toMat4(m_Rotation);
}

ComponentTransform::ComponentTransform() : Component(ComponentType::TRANSFORM)
{
	m_Translation = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Scaling = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Rotation = quat(0.0f, 0.0f, 0.0f, 0.0f);
	m_LocalMat = glm::mat4x4(1.0f);
	
	m_WorldTranslation = glm::vec3(0.0f, 0.0f, 0.0f);
	m_WorldScaling = glm::vec3(0.0f, 0.0f, 0.0f);
	m_WorldRotation = quat(0.0f, 0.0f, 0.0f, 0.0f);
	m_WorldMat = glm::mat4x4(1.0f);

	// In code we first translate and then later rotate, the actual transformations first apply a rotation and then a translation.
	m_LocalMat = glm::translate(m_LocalMat, m_Translation);
	m_LocalMat = glm::scale(m_LocalMat, m_Scaling);
	m_LocalMat = glm::toMat4(m_Rotation);
	m_Dirty = false;
}

ComponentTransform::~ComponentTransform()
{
	
}

void ComponentTransform::Update()
{
	ComposeLocalMatrix();
	if (m_hasGameObject)
	{
		GameObject* parent = GetGameObject().GetParent();
		if (parent != nullptr && parent->GetTransform() != nullptr)
		{
			m_WorldMat = parent->GetTransform()->m_WorldMat * m_LocalMat;
		}
	}
	DecomposeWorldMatrix();
}

void ComponentTransform::OnEditor()
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
	ImGuiInputTextFlags iflags = ImGuiInputTextFlags_ReadOnly;
	if (ImGui::CollapsingHeader("Local Transformation", flags))
	{
		ImGui::InputFloat3("Position", &m_Translation[0], "%.3f", iflags);
		glm::vec3 euler;
		extractEulerAngleXYZ(m_LocalMat, euler.x, euler.y, euler.z);
		euler = glm::degrees(euler);
		ImGui::InputFloat3("Rotation", &euler[0], "%.3f", iflags);
		ImGui::InputFloat3("Scaling", &m_Scaling[0], "%.3f", iflags);
	}
	if (ImGui::CollapsingHeader("World Transformation", flags))
	{
		ImGui::InputFloat3("Position", &m_WorldTranslation[0], "%.3f", iflags);
		glm::vec3 euler;
		extractEulerAngleXYZ(m_WorldMat, euler.x, euler.y, euler.z);
		euler = glm::degrees(euler);
		ImGui::InputFloat3("Rotation", &euler[0], "%.3f", iflags);
		ImGui::InputFloat3("Scaling", &m_WorldScaling[0], "%.3f", iflags);
	}
}

void ComponentTransform::ComposeLocalMatrix()
{
	// In code we first translate and then later rotate, the actual transformations first apply a rotation and then a translation.
	m_LocalMat = glm::translate(m_LocalMat, m_Translation);
	m_LocalMat = glm::scale(m_LocalMat, m_Scaling);
	m_LocalMat = m_LocalMat * glm::toMat4(m_Rotation);
}

void ComponentTransform::DecomposeWorldMatrix()
{
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(m_WorldMat, m_WorldScaling, m_WorldRotation, m_WorldTranslation, skew, perspective);
}

glm::mat4x4 ComponentTransform::GetCombination(glm::mat4x4 transform)
{
	glm::mat4x4 comb = m_LocalMat * transform;
	return comb;
}
