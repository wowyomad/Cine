#ifdef VERTEX

layout (location=0) in vec3 a_Position;

layout (location=1) uniform mat4 u_ViewProjection;
layout (location=2) uniform mat4 u_Transform;
layout (location=3) uniform vec4 u_Color;

out vec4 f_Color;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
	f_Color = u_Color;
}
#endif

#ifdef FRAGMENT

in vec4 f_Color;

out vec4 screenColor;

void main()
{
	screenColor = f_Color;
}

#endif