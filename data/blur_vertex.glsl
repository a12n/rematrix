#version 130

in vec2 position;

out vec2 fragTexCoords[11];

uniform bool isHoriz = true;
uniform ivec2 targetSize;

void
main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    vec2 t0 = position * 0.5 + 0.5;
    vec2 d = isHoriz ? vec2(1.0 / targetSize.x, 0.0) : vec2(0.0, 1.0 / targetSize.y);
    for (int i = 0; i < 11; ++i) {
        fragTexCoords[i] = t0 + (i - 5) * d;
    }
}
