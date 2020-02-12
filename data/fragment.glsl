#version 130

in vec2 tex_coord;

uniform sampler2D tex;

const float fill = 0.5;
const float falloff = 0.03;

void
main()
{
    float dist = 1.0 - texture(tex, tex_coord).r;
    float alpha = 1.0 - smoothstep(fill, fill + falloff, dist);
    gl_FragColor = vec4(1, 1, 1, alpha);
}
