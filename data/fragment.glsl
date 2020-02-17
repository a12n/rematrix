#version 130

in vec2 fragTexCoord;
in float fragDepth;

uniform sampler2D sdfTex;
uniform vec3 fillColor = vec3(0x45 / 255.0, 0x85 / 255.0, 0x88 / 255.0);

const vec3 feederColor = vec3(0x83 / 255.0, 0xa5 / 255.0, 0x98 / 255.0);

uniform int charPos = 1;
uniform float feederPos = 0.0;
uniform bool isErasing = false;
uniform bool isFeeder = false;
uniform int wavePos = 0;

void
main()
{
    float s = texture(sdfTex, fragTexCoord).r;
    float c = 1.0 / fwidth(s);
    float d = (s - 0.5) * c;
    float alpha = clamp(d + 0.5, 0.0, 1.0);
    // gl_FragColor = vec4(fillColor, min(alpha + 0.5, 1.0));
    float splashFactor = smoothstep(24, 23, fragDepth);
    float fogFactor = 0.2 + ((fragDepth / 35) + 0.5) * 0.8;
    float eraseFactor = isErasing ? smoothstep(0, -1, feederPos - charPos) : 1;
    float appearFactor = isErasing ? 1 : smoothstep(0, 1, feederPos - charPos);
    gl_FragColor = fogFactor * vec4(isFeeder ? feederColor : fillColor, eraseFactor * splashFactor * alpha);
}
