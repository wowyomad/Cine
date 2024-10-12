#ifdef VERTEX

in layout (location=0) vec3 a_Position;
in layout (location=1) vec4 a_Color;
in layout (location=2) vec2 a_TexCoord;
in layout (location=4) float a_TexIndex;
in layout (location=3) float a_Tiling;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;
out flat float v_TexIndex;

void main()
{
	v_TexCoord = a_TexCoord * a_Tiling;
	v_Color = a_Color;
	v_TexIndex = a_TexIndex;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#endif

#ifdef FRAGMENT

in vec4 v_Color;
in vec2 v_TexCoord;
in flat float v_TexIndex;

uniform sampler2D u_Textures[32];

out vec4 o_Color;

void main()
{
	o_Color = texture(u_Textures[int(v_TexIndex)], v_TexCoord) * v_Color;
}

#endif