#include "ComponentTransform.h"

ComponentTransform::ComponentTransform(glm::vec3 translation, glm::vec3 scaling, glm::quat rotation) : m_Translation(translation), m_Scaling(scaling), m_Rotation(rotation)
{
	m_TransMat = mat4x4(1.0f);
	m_World = mat4x4(1.0f);
	m_Dirty = false;

	// In code we first translate and then later rotate, the actual transformations first apply a rotation and then a translation.
	m_TransMat = glm::translate(m_TransMat, m_Translation);
	m_TransMat = glm::scale(m_TransMat, m_Scaling);
	m_TransMat = glm::toMat4(m_Rotation);
}

ComponentTransform::ComponentTransform() : Component(ComponentType::TRANSFORM)
{
	m_Translation = vec3(0.0f, 0.0f, 0.0f);
	m_Scaling = vec3(0.0f, 0.0f, 0.0f);
	m_Rotation = quat(0.0f, 0.0f, 0.0f, 0.0f);
	m_TransMat = mat4x4(1.0f);

	// In code we first translate and then later rotate, the actual transformations first apply a rotation and then a translation.
	m_TransMat = glm::translate(m_TransMat, m_Translation);
	m_TransMat = glm::scale(m_TransMat, m_Scaling);
	m_TransMat = glm::toMat4(m_Rotation);
	m_World = mat4x4(1.0f);
	m_Dirty = false;
}

ComponentTransform::~ComponentTransform()
{
	
}

void ComponentTransform::Update()
{
	CalculateLocal();
}

void ComponentTransform::OnEditor()
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
	ImGuiInputTextFlags iflags = ImGuiInputTextFlags_ReadOnly;
	if (ImGui::CollapsingHeader("Local Transformation", flags))
	{
		ImGui::InputFloat3("Position", &m_Translation[0], "%.3f", iflags);
		glm::vec3 euler;
		extractEulerAngleXYZ(m_TransMat, euler.x, euler.y, euler.z);
		euler = glm::degrees(euler);
		ImGui::InputFloat3("Rotation", &euler[0], "%.3f", iflags);
		ImGui::InputFloat3("Scaling", &m_Scaling[0], "%.3f", iflags);
	}
}

void ComponentTransform::CalculateLocal()
{
	// In code we first translate and then later rotate, the actual transformations first apply a rotation and then a translation.
	m_TransMat = glm::translate(m_TransMat, m_Translation);
	m_TransMat = glm::scale(m_TransMat, m_Scaling);
	m_TransMat = glm::toMat4(m_Rotation);
}

glm::mat4x4 ComponentTransform::GetCombination(glm::mat4x4 transform)
{
	glm::mat4x4 comb = m_TransMat * transform;
	return comb;
}
