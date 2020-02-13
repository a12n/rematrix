#version 130

const vec3 neutral_blue = vec3(0x45 / 255.0, 0x85 / 255.0, 0x88 / 255.0);
const vec3 faded_blue   = vec3(0x07 / 255.0, 0x66 / 255.0, 0x78 / 255.0);

in vec2 tex_coord2;

uniform sampler2D tex;
uniform vec3 fill_color = neutral_blue;
uniform vec3 fill_color2 = faded_blue;

const float fill = 0.5;
const float falloff = 0.03;

const float fill2 = 0.2;
const float falloff2 = 0.8;

void
main()
{
    float dist = 1.0 - texture(tex, tex_coord2).r;
    float alpha = 1.0 - smoothstep(fill, fill + falloff, dist);
    float alpha2 = 1.0 - smoothstep(fill2, fill2 + falloff2, dist);
    float alpha_ans = alpha + (1.0 - alpha) * alpha2;
    vec3 color_ans = mix(fill_color2, fill_color, alpha / alpha_ans);
    gl_FragColor = vec4(color_ans, alpha_ans);
}
