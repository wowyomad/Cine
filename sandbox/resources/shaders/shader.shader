#shader vertex
#version 330 core

layout (location=0) in vec3 vertexPos;
layout (location=1) in vec3 vertexColor;

uniform float brightness;
uniform float another_uniform;

out vec3 fragmentColor;

void main() 
{
	gl_Position = vec4(vertexPos, 1.0);
	fragmentColor = vertexPos + 0.5;
}

#shader fragment
#version 330 core

in vec3 fragmentColor;
out vec4 screenColor;

void main()
{
	screenColor = vec4(fragmentColor, 1.0);
}
