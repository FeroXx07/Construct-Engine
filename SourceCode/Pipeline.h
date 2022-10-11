#ifndef __Pipeline_H__
#define __Pipeline_H__
#include "MathGeoLib/MathGeoLib.h"

using namespace math;
class Pipeline
{
public:
    Pipeline();
    ~Pipeline();
    void Scale(const float ScaleX, const float ScaleY, const float ScaleZ);
    void WorldPos(const float x, const float y, const float z);
    void Rotate(const float RotateX, const float RotateY, const float RotateZ);
    const float4x4* GetTrans();
private:
    float3 m_scale;
    float3 m_worldPos;
    float3 m_rotateInfo;
    float4x4 m_transformation;
};

#endif