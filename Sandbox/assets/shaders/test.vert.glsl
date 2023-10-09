#version 460 core

layout (location = 0) in vec3 a_Position;

uniform mat4 u_Model;		// World Space
uniform mat4 u_View;		// Camera/View Space
uniform mat4 u_Projection;	// Perspective/Clip Space

void main()
{
	//gl_Position = vec4(a_Position, 1.0f);
	//gl_Position = u_Projection * u_View * vec4(a_Position, 1.0f);
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0f);
}