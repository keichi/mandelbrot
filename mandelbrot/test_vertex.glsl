#version 120

varying vec4 p;

void main(void)
{
	gl_Position = gl_Vertex;
    p = (gl_Vertex - vec4(0.5, 0, 0, 0)) * vec4(4.0 / 3.0, 1.0, 1.0, 1.0) * 1.0;

}