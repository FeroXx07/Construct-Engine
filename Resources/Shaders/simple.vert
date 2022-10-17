#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 inColor;

out vec4 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 trans;

void main()
{
	// note that we read the multiplication from right to left
  //gl_Position = projection * view * model * vec4(position, 1.0);
    gl_Position = trans * vec4(position, 1.0);
	Color = vec4(inColor, 1.0f);
}