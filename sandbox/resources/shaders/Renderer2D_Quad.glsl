#ifdef VERTEX

in vec4 a_Position;
in vec2 a_TexCoord;

uniform vec4 u_Color;
uniform mat4 u_Transform;
uniform mat4 u_ViewProjection;

out vec2 f_TexCoord;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * a_Position;
	f_TexCoord = a_TexCoord;
}

#endif

#ifdef FRAGMENT

in vec2 f_TexCoord;

uniform sampler2D u_Texture;
uniform vec4 u_Color;


out vec4 outColor;

void main()
{
	vec4 texColor = texture(u_Texture, f_TexCoord);
	outColor = texColor * u_Color;
}

#endif