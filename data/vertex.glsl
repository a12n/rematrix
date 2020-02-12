#version 130

in vec3 position;

out vec2 tex_coord;

void
main()
{
    gl_Position = vec4(position, 1.0);
    tex_coord = vec2(position.x < 0 ? 0 : 1, position.y < 0 ? 1 : 0);
}
