#version 120

varying vec4 p;

void main(void)
{
	gl_Position = gl_Vertex;
    p = gl_Position * vec4(16.0 / 9.0, 1.0, 1.0, 1.0);
}