#ifndef __Pipeline_H__
#define __Pipeline_H__
#include "MathGeoLib/MathGeoLib.h"

using namespace math;
class Pipeline
{
public:
    Pipeline();
    ~Pipeline();
    void Scale(float ScaleX, float ScaleY, float ScaleZ);
    void WorldPos(float x, float y, float z);
    void Rotate(float RotateX, float RotateY, float RotateZ);
    const float4x4* GetTrans();
private:
    float3 m_scale;
    float3 m_worldPos;
    float3 m_rotateInfo;
    float4x4 m_transformation;
};

#endif