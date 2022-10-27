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
	ComponentTransform(glm::vec3 translation, glm::vec3 scaling, glm::quat rotation);
	ComponentTransform();
	~ComponentTransform();

	void Update();
	void OnEditor();

	void ComposeLocalMatrix();
	void DecomposeWorldMatrix();

	glm::mat4x4 GetCombination(glm::mat4x4 transform);
	bool m_Dirty = false;
	glm::mat4x4 m_WorldMat;
	glm::mat4x4 m_LocalMat;

	glm::vec3 m_Translation, m_Scaling;
	glm::quat m_Rotation;

	glm::vec3 m_WorldTranslation, m_WorldScaling;
	glm::quat m_WorldRotation;
	
};
#endif