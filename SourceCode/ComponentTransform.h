#ifndef _ComponentTransform_H
#define _ComponentTransform_H
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
using namespace glm;
class ComponentTransform : public Component
{
public:
	ComponentTransform(const glm::vec3& translation, const glm::vec3& scaling, const glm::quat& rotation);
	ComponentTransform();
	~ComponentTransform();

	void Update();
	void OnEditor();

private:
	void ComposeLocalMatrix();
	
public:
	void SetTranslate(const glm::vec3& translation);
	void SetRotation(const glm::vec3& rotation);
	void SetRotation(const glm::quat& rotation);
	void SetScale(const glm::vec3& scaling);
	void SetLocalMatrix(const glm::mat4x4& mat);
	void SetWorldMatrix(const glm::mat4x4& mat);
	glm::mat4x4 Combine(const glm::mat4x4& transform);

	const glm::vec3 GetTranslate();
	const glm::quat GetRotationQuat();
	const glm::vec3 GetRotationEuler();
	const glm::vec3 GetScaling();
	const glm::mat4x4 GetLocal();
	const glm::mat4x4 GetWorld();

	bool m_Dirty = false;
private:
	void SetDirtyChildren();

	glm::mat4x4 m_LocalMat;
	glm::mat4x4 m_WorldMat;

	glm::vec3 m_Translation, m_Scaling;
	glm::quat m_Rotation;

private:
	GuiInputData m_InputTrans;
	GuiInputData m_InputRot;
	GuiInputData m_InputScale;
	

	friend class ModelLoader;
	friend class GameObject;
};
#endif