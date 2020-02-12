#version 130

const vec3 neutral_blue = vec3(0x45 / 255.0, 0x85 / 255.0, 0x88 / 255.0);
const vec3 faded_blue   = vec3(0x07 / 255.0, 0x66 / 255.0, 0x78 / 255.0);

in vec2 tex_coord;

uniform sampler2D tex;
uniform vec3 fill_color = neutral_blue;

const float fill = 0.5;
const float falloff = 0.03;

void
main()
{
    float dist = 1.0 - texture(tex, tex_coord).r;
    float alpha = 1.0 - smoothstep(fill, fill + falloff, dist);
    gl_FragColor = vec4(fill_color, alpha);
}
