#include "ComponentTransform.h"
#include "GameObject.h"

ComponentTransform::ComponentTransform(const glm::vec3& translation, const glm::vec3& scaling, const glm::quat& rotation) :Component(ComponentType::TRANSFORM), m_Translation(translation), m_Scaling(scaling), m_Rotation(rotation)
{
	m_LocalMat = glm::mat4x4(1.0f);
	m_WorldMat = glm::mat4x4(1.0f);
	m_Dirty = false;

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
}

void ComponentTransform::OnEditor()
{
	ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen;
	m_InputTrans.SetData(m_Translation);
	m_InputScale.SetData(m_Scaling);
	glm::vec3 eulerL = glm::eulerAngles(m_Rotation);
	//extractEulerAngleXYZ(m_LocalMat, eulerL.x, eulerL.y, eulerL.z);
	eulerL = glm::degrees(eulerL);
	m_InputRot.SetData(eulerL);

	ImGuiInputTextFlags lflag = ImGuiInputTextFlags_AllowTabInput;
	if (ImGui::TreeNodeEx("Local Transformation", treeFlags))
	{
		ImGui::DragFloat3("Position", &m_Translation[0], 0.05f, 0.0f, 0.0f, "%.3f", lflag);

		ImGui::DragFloat3("Rotation", &eulerL[0], 0.05f, 0.0f, 0.0f, "%.3f", lflag);
		if (ImGui::IsItemEdited())
			m_Rotation = glm::quat(glm::radians(eulerL));

		ImGui::DragFloat3("Scaling", &m_Scaling[0], 0.01f, 0.0f, 0.0f, "%.3f", lflag);

		ImGui::TreePop();
	}


	glm::vec3 skew;
	glm::vec4 perspective;
	glm::vec3 m_WorldScaling;
	glm::quat m_WorldRotation;
	glm::vec3 m_WorldTranslation;
	glm::decompose(m_WorldMat, m_WorldScaling, m_WorldRotation, m_WorldTranslation, skew, perspective);

	glm::vec3 eulerW;
	extractEulerAngleXYZ(m_WorldMat, eulerW.x, eulerW.y, eulerW.z);
	eulerW = glm::degrees(eulerW);

	ImGuiInputTextFlags wflag = ImGuiInputTextFlags_ReadOnly;
	if (ImGui::TreeNodeEx("World Transformation (Read Only)", treeFlags))
	{
		ImGui::InputFloat3("Position", &m_WorldTranslation[0], "%.3f", wflag);
		ImGui::InputFloat3("Rotation", &eulerW[0], "%.3f", wflag);
		ImGui::InputFloat3("Scaling", &m_WorldScaling[0], "%.3f", wflag);
		ImGui::TreePop();
	}
}

void ComponentTransform::ComposeLocalMatrix()
{
	// In code we first translate and then later rotate, the actual transformations first apply a rotation and then a translation.
	glm::mat4 t = glm::translate(glm::mat4(1.0f), m_Translation);
	glm::mat4 s = glm::scale(glm::mat4(1.0f), m_Scaling);
	glm::mat4 r = glm::mat4(1.0f) * glm::toMat4(m_Rotation);
	m_LocalMat = t * s * r;
}

void ComponentTransform::SetTranslate(const glm::vec3& translation)
{
	m_Translation = translation;
	m_InputTrans.m_OpHasBeenEdited = true;
}

void ComponentTransform::SetRotation(const glm::vec3& rotation)
{
	m_Rotation = glm::quat(rotation);
	m_InputRot.m_OpHasBeenEdited = true;
}

void ComponentTransform::SetRotation(const glm::quat& rotation)
{
	m_Rotation = rotation;
	m_InputRot.m_OpHasBeenEdited = true;
}

void ComponentTransform::SetScale(const glm::vec3& scaling)
{
	m_Scaling = scaling;
	m_InputScale.m_OpHasBeenEdited = true;
}

void ComponentTransform::SetLocalMatrix(const glm::mat4x4& mat)
{
	m_LocalMat = mat;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(m_LocalMat, m_Scaling, m_Rotation, m_Translation, skew, perspective);
}

void ComponentTransform::SetWorldMatrix(const glm::mat4x4& mat)
{
	m_WorldMat = mat;
}

void ComponentTransform::SetDirtyChildren()
{
	if (m_hasGameObject)
	{
		GameObject* go = &GetGameObject();
		// Traverse all the children 
		if (go != nullptr)
		{
			for (auto c : go->m_Children)
			{
				if (c->m_HasComponentTransform)
				{
					c->GetTransform()->SetDirtyChildren();
				}
			}
		}
	}
	m_Dirty = true;
}


glm::mat4x4 ComponentTransform::Combine(const glm::mat4x4& transform)
{
	glm::mat4x4 comb = transform * m_LocalMat;
	return comb;
}

const glm::vec3 ComponentTransform::GetTranslate()
{
	return m_Translation;
}

const glm::quat ComponentTransform::GetRotationQuat()
{
	return m_Rotation;
}

const glm::vec3 ComponentTransform::GetRotationEuler()
{
	glm::vec3 eulerL;
	extractEulerAngleXYZ(m_LocalMat, eulerL.x, eulerL.y, eulerL.z);
	eulerL = glm::degrees(eulerL);
	return eulerL;
}

const glm::vec3 ComponentTransform::GetScaling()
{
	return m_Scaling;
}

const glm::mat4x4 ComponentTransform::GetLocal()
{
	return m_LocalMat;
}

const glm::mat4x4 ComponentTransform::GetWorld()
{
	return m_WorldMat;
}
