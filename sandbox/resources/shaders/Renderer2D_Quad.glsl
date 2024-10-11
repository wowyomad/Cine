#ifdef VERTEX

in layout (location=0) vec3 a_Position;
in layout (location=1) vec4 a_Color;
in layout (location=2) vec2 a_TexCoord;
in layout (location=4) float a_TexIndex;
in layout (location=3) float a_Tiling;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out float v_Tiling;

void main()
{
	v_TexCoord = a_TexCoord;
	v_Color = a_Color;
	v_TexIndex = a_TexIndex;
	v_Tiling = a_Tiling;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#endif

#ifdef FRAGMENT

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in float v_Tiling;

uniform sampler2D u_Textures[32];

out vec4 o_Color;

void main()
{
	o_Color = texture(u_Textures[int(v_TexIndex)], v_TexCoord * v_Tiling) * v_Color;
}

#endif