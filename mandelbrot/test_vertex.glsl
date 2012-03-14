#version 120

uniform vec4 center;
uniform float zoom;

varying vec4 p;

void main(void)
{
	gl_Position = gl_Vertex;
    p = (gl_Vertex - center) * vec4(4.0 / 3.0, 1.0, 1.0, 1.0) * zoom;

}