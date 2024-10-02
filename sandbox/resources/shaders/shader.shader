#shader vertex
#version 330 core

layout (location=0) in vec3 a_Position;
layout (location=1) in vec4 a_Color;

uniform float brightness;
uniform float another_uniform;

out vec4 f_Color;

void main() 
{
	gl_Position = vec4(a_Position, 1.0);
	f_Color = a_Color;
}

#shader fragment
#version 330 core

in vec4 f_Color;
out vec4 screenColor;

void main()
{
	screenColor = f_Color;
}
