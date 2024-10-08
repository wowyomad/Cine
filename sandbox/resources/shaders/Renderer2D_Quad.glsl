#ifdef VERTEX

layout (location=0) in vec4 a_Position;

layout (location=1) uniform vec4 u_Color;
layout (location=2) uniform mat4 u_Transform;
layout (location=3) uniform mat4 u_ViewProjection;

out vec4 f_Color;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * a_Position;
	f_Color = u_Color;
}

#endif

#ifdef FRAGMENT

in vec4 f_Color;

out vec4 outColor;

void main()
{
	outColor = f_Color;
}

#endif