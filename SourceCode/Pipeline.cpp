#include "Pipeline.h"

Pipeline::Pipeline()
{
	this->m_scale = float3::zero;
	this->m_worldPos = float3::zero;
	this->m_rotateInfo = float3::zero;
}

Pipeline::~Pipeline()
{
}

void Pipeline::Scale(const float ScaleX, const float ScaleY, const float ScaleZ)
{
	this->m_scale = float3(ScaleX, ScaleY, ScaleZ);
}

void Pipeline::WorldPos(const float x, const float y, const float z)
{
	this->m_worldPos = float3(x, y, z);
}

void Pipeline::Rotate(const float RotateX, const float RotateY, const float RotateZ)
{
	this->m_rotateInfo = float3(RotateX, RotateY, RotateZ);

}

const float4x4* Pipeline::GetTrans()
{
	float4x4 ScaleTrans, RotateTrans, TranslationTrans;
	ScaleTrans = RotateTrans = TranslationTrans = float4x4::identity;

	ScaleTrans = float4x4::Scale(m_scale).ToFloat4x4();
	RotateTrans = float4x4::RotateX(m_rotateInfo.x).RotateY(m_rotateInfo.y).RotateZ(m_rotateInfo.z);
	TranslationTrans = float4x4::Translate(m_worldPos);

	m_transformation = TranslationTrans * RotateTrans * ScaleTrans;
	return &m_transformation;
}
