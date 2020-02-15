#version 130

in vec2 position;
in vec2 texCoord;

out vec2 fragTexCoord;
out float fragDepth;

uniform mat4 model = mat4(1.0);
uniform mat4 projection = mat4(1.0);
uniform mat4 view = mat4(1.0);

void
main()
{
    vec4 ans = model * vec4(position, 0.0, 1.0);
    gl_Position = projection * view * ans;
    fragTexCoord = texCoord;
    fragDepth = ans.z;
}
