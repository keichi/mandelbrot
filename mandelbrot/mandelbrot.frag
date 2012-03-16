#version 120

varying vec4 p;

uniform int level;
uniform vec4 start_color;
uniform vec4 end_color;

const float threshold = 2.0;

void main()
{
    vec2 z = vec2(0, 0);
    int count = -1;
    
    for (int i = 0; i < level; i++) {
        vec2 nz = vec2(0, 0);
        nz.x = z.x * z.x  - z.y * z.y + p.x;
        nz.y = 2.0 * z.x * z.y + p.y;
        if (length(nz) > threshold) {
            count = i;
            break;
        }
        z = nz;
    }
    
    if (count == 0) {
        gl_FragColor = end_color;
    } else if (count == -1) {
        gl_FragColor = vec4(0, 0, 0, 0);
    } else {
        float c = sqrt(float(count) / level);
        gl_FragColor = start_color * c + end_color * (1 - c);
    }
}
