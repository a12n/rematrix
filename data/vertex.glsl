#version 130

in vec3 position;
in vec2 tex_coord;

out vec2 tex_coord2;

void
main()
{
    gl_Position = vec4(position, 1.0);
    tex_coord2 = tex_coord;
}
