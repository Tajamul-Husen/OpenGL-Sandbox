#version 460 core

layout (location = 0) in vec2 a_Position;
//layout (location = 1) in vec2 a_Offset;
layout (location = 2) in mat4 a_ModelMatrix;

uniform mat4 u_Model;		// World Space
uniform mat4 u_View;		// Camera/View Space
uniform mat4 u_Projection;	// Perspective/Clip Space

void main()
{
	//gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 0.0f, 1.0f);
	// gl_Position = u_Projection * u_View * vec4(a_Position + a_Offset, 0.0f, 1.0f);
	gl_Position = u_Projection * u_View * a_ModelMatrix * vec4(a_Position, 0.0f, 1.0f);
}