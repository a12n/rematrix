#version 130

in vec2 fragTexCoords[11];

out vec4 fragColor;

uniform sampler2D tex;

const float weights[11] = float[11](
    0.000003,
    0.000229,
    0.005977,
    0.060598,
    0.24173,
    0.382925,
    0.24173,
    0.060598,
    0.005977,
    0.000229,
    0.000003
    );

void
main()
{
    fragColor = vec4(0.0);
    for (int i = 0; i < 11; ++i) {
        fragColor += texture(tex, fragTexCoords[i]) * weights[i];
    }
}
