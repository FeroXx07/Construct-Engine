#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec4 inColor;

uniform mat4 gWorld;
out vec4 Color;

void main()
{
   gl_Position = gWorld * vec4(Position, 1.0);
   Color = inColor;
}