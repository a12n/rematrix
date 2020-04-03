#version 130

in vec2 fragTexCoord;

out vec4 fragColor;

uniform sampler2D colorTex;
uniform sampler2D highlightTex;

void
main()
{
    fragColor = texture(colorTex, fragTexCoord) + texture(highlightTex, fragTexCoord) * 0.001;
}
