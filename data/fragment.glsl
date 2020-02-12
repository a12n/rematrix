#version 130

in vec2 tex_coord;

uniform sampler2D tex;

const float smoothing = 0.75 / 16.0;

void
main()
{
    float dist = texture(tex, tex_coord).r;
    float alpha = smoothstep(0.5 - smoothing, 0.5 + smoothing, dist);
    gl_FragColor = vec4(1, 1, 1, alpha);
}
