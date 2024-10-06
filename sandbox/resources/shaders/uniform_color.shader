#type vertex
#version 450 core

layout (location=0) in vec3 a_Position;
layout (location=1) in vec2 a_TexCoord;

layout (location=2) uniform mat4 u_ViewProjection;
layout (location=3) uniform mat4 u_Transform;
layout (location=4) uniform vec4 u_Color;

out vec4 f_Color;
out vec2 f_Texture;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
	f_Color = u_Color;
	f_Texture = a_TexCoord;
}

#type fragment
#version 450 core

in vec4 f_Color;
in vec2 f_Texture;

layout (binding=0) uniform sampler2D u_Texture;

out vec4 screenColor;

void main()
{
	vec4 textureColor = texture(u_Texture, f_Texture);
	screenColor = textureColor * f_Color;
}