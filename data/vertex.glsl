#version 130

in vec2 position;
in vec2 tex_coord;

out vec2 tex_coord2;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void
main()
{
    gl_Position = projection * view * model * vec4(position, 0.0, 1.0);
    tex_coord2 = tex_coord;
}
