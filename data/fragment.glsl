#version 130

in vec2 texCoord2;

uniform sampler2D sdfTex;
uniform vec3 fillColor = vec3(0x45 / 255.0, 0x85 / 255.0, 0x88 / 255.0);

const float smoothing = 0.001;

void
main()
{
    float d = texture(sdfTex, texCoord2).r;
    float a = smoothstep(0.5 - smoothing, 0.5 + smoothing, d);
    gl_FragColor = vec4(fillColor, a);
}
