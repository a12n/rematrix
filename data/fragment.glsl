#version 130

in vec2 tex_coord;

uniform sampler2D tex;

void
main()
{
    gl_FragColor = vec4(1, 1, 1, texture(tex, tex_coord).r);
}
