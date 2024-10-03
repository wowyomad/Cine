#shader vertex
#version 330 core

layout (location=0) in vec3 a_Position;
layout (location=1) in vec4 a_Color;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform vec4 u_Color;
uniform int u_UseUniformColor;

out vec4 f_Color;

void main() 
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
	if(u_UseUniformColor > 0)
		f_Color = u_Color;
	else	
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
