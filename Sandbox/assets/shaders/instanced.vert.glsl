#version 460 core

layout (location = 0) in vec3 a_Position;
layout (location = 2) in mat4 a_InstanceModelMatrix;

uniform mat4 u_View;		// Camera/View Space
uniform mat4 u_Projection;	// Perspective/Clip Space

void main()
{
	gl_Position = u_Projection * u_View * a_InstanceModelMatrix * vec4(a_Position, 1.0f);
}