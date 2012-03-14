#version 120

uniform vec4 center;
uniform float zoom;
uniform float aspect_ratio;

varying vec4 p;

void main(void)
{
	gl_Position = gl_Vertex;
    p = (gl_Vertex * zoom + center) / aspect_ratio * vec4(aspect_ratio, 1.0, 1.0, 1.0);
}