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
	vec4 texColor = v_Color;

	//Doesn't work correctly on AMD, apparently. AMD, eat shit.
	//texColor *= texture(u_Textures[int(v_TexIndex)], v_TexCoord); 

	switch(int(v_TexIndex))
	{
		case  0: texColor *= texture(u_Textures[ 0], v_TexCoord); break;
		case  1: texColor *= texture(u_Textures[ 1], v_TexCoord); break;
		case  2: texColor *= texture(u_Textures[ 2], v_TexCoord); break;
		case  3: texColor *= texture(u_Textures[ 3], v_TexCoord); break;
		case  4: texColor *= texture(u_Textures[ 4], v_TexCoord); break;
		case  5: texColor *= texture(u_Textures[ 5], v_TexCoord); break;
		case  6: texColor *= texture(u_Textures[ 6], v_TexCoord); break;
		case  7: texColor *= texture(u_Textures[ 7], v_TexCoord); break;
		case  8: texColor *= texture(u_Textures[ 8], v_TexCoord); break;
		case  9: texColor *= texture(u_Textures[ 9], v_TexCoord); break;
		case 10: texColor *= texture(u_Textures[10], v_TexCoord); break;
		case 11: texColor *= texture(u_Textures[11], v_TexCoord); break;
		case 12: texColor *= texture(u_Textures[12], v_TexCoord); break;
		case 13: texColor *= texture(u_Textures[13], v_TexCoord); break;
		case 14: texColor *= texture(u_Textures[14], v_TexCoord); break;
		case 15: texColor *= texture(u_Textures[15], v_TexCoord); break;
		case 16: texColor *= texture(u_Textures[16], v_TexCoord); break;
		case 17: texColor *= texture(u_Textures[17], v_TexCoord); break;
		case 18: texColor *= texture(u_Textures[18], v_TexCoord); break;
		case 19: texColor *= texture(u_Textures[19], v_TexCoord); break;
		case 20: texColor *= texture(u_Textures[20], v_TexCoord); break;
		case 21: texColor *= texture(u_Textures[21], v_TexCoord); break;
		case 22: texColor *= texture(u_Textures[22], v_TexCoord); break;
		case 23: texColor *= texture(u_Textures[23], v_TexCoord); break;
		case 24: texColor *= texture(u_Textures[24], v_TexCoord); break;
		case 25: texColor *= texture(u_Textures[25], v_TexCoord); break;
		case 26: texColor *= texture(u_Textures[26], v_TexCoord); break;
		case 27: texColor *= texture(u_Textures[27], v_TexCoord); break;
		case 28: texColor *= texture(u_Textures[28], v_TexCoord); break;
		case 29: texColor *= texture(u_Textures[29], v_TexCoord); break;
		case 30: texColor *= texture(u_Textures[30], v_TexCoord); break;
		case 31: texColor *= texture(u_Textures[31], v_TexCoord); break;
	}

	if (texColor.a == 0.0)
		discard;

	o_Color = texColor;
}

#endif