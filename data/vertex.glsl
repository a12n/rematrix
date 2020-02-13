#version 130

in vec2 position;
in vec2 texCoord;

out vec2 texCoord2;

uniform mat4 model = mat4(1.0);
uniform mat4 projection = mat4(1.0);
uniform mat4 view = mat4(1.0);

void
main()
{
    gl_Position = projection * view * model * vec4(position, 0.0, 1.0);
    texCoord2 = texCoord;
}
