#ifdef VERTEX

layout (location=0) in vec4 a_Position;
layout (location=1) in vec2 a_TexCoord;

layout (location=2) uniform mat4 u_ViewProjection;
layout (location=3) uniform mat4 u_Transform;
layout (location=4) uniform vec4 u_Color;

out vec4 f_Color;
out vec2 f_TexCoord;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * a_Position;
	f_Color = u_Color;
	f_TexCoord = a_TexCoord;
}
#endif

#ifdef FRAGMENT
in vec4 f_Color;
in vec2 f_TexCoord;

layout (binding=0) uniform sampler2D u_Texture;

out vec4 screenColor;

void main()
{
	vec4 textureColor = texture(u_Texture, f_TexCoord);
	screenColor = textureColor * f_Color;
}
#endif