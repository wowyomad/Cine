#shader vertex
#version 460 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTexCoord;

out vec2 v_TexCoord;

uniform mat4 uMVP;

void main()
{
	gl_Position = uMVP * vec4(aPos, 1.0);
	v_TexCoord = aTexCoord;

}

#shader fragment
#version 460 core

in vec2 v_TexCoord;

out vec4 color;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor;
}

