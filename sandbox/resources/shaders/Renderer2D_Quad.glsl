#ifdef VERTEX

in layout (location=0) vec3 a_Position;
in layout (location=1) vec4 a_Color;
in layout (location=2) vec2 a_TexCoord;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;

void main()
{
	v_TexCoord = a_TexCoord;
	v_Color = a_Color;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#endif

#ifdef FRAGMENT

in vec4 v_Color;
in vec2 v_TexCoord;

out vec4 o_Color;

void main()
{
	// vec4 texColor = texture(u_Texture, f_TexCoord);
	o_Color = v_Color;
}

#endif