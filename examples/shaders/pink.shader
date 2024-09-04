#shader vertex
#version 330 core

layout (location=0) in vec3 vertexPosition;
out vec4 vertexColor;

void main()
{
	gl_Position = vec4(vertexPosition, 1.0);
	vertexColor = vec4(0.9, 0.1, 0.7, 1.0);
}

#shader fragment
#version 330 core

in vec4 vertexColor;
out vec4 FragColor;

void main()
{
	FragColor = vertexColor;
}