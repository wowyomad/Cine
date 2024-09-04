#shader vertex
#version 330 core

layout (location=0) in vec3 vertexPosition;
out vec4 vertexColor;

uniform float time;

void main()
{
	float freq = 4.0;

    float r = 0.3 + 0.5 * (sin(time * freq) + 1.0) * 0.5; 
    float g = 0; 
    float b = 0;

	vertexColor = vec4(r, g, b, 1);
	gl_Position = vec4(vertexPosition, 1.0);
}

#shader fragment
#version 330 core

in vec4 vertexColor;
out vec4 FragColor;

void main()
{
	FragColor = vertexColor;
}