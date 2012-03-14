#version 120

varying vec4 p;
const int level = 100;
const float threshold = 2.0;

void main()
{
    vec2 z = vec2(0, 0);
    int count = 0;
    
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
        gl_FragColor = vec4(0, 0, 0, 1);
    } else {
        float c = sqrt(float(count) / level);
        gl_FragColor = vec4(c, c, 1, 1);
    }
}
