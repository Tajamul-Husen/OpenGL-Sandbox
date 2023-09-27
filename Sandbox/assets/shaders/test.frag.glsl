#version 460 core

layout (location = 0) out vec4 frag_Color;

uniform vec4 u_Color;

void main()
{
	frag_Color = u_Color;
}