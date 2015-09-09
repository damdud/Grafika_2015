#version 130

in vec4 s_vPosition;
in vec4 s_vColor;
out vec4 color;
//uniform mat4 s_mM;
//uniform mat4 s_mV;
//uniform mat4 s_mP;

void main ()
{
	//gl_Position = s_mP*s_mV*s_mM*s_vPosition;
	color = s_vColor;
	gl_Position = s_vPosition;
}