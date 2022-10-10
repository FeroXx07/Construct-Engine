#version 330 core
layout (location = 0) in vec3 aPos;

uniform float gScale;
uniform mat4 gTranslation;
void main()
{
    gl_Position = gTranslation * vec4(gScale * aPos.x, gScale * aPos.y, aPos.z, 1.0);
}