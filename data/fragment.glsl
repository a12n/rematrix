#version 130

in vec2 fragTexCoord;
in float fragDepth;

uniform sampler2D sdfTex;

uniform vec3 charColor = vec3(0x45 / 255.0, 0x85 / 255.0, 0x88 / 255.0);
uniform vec3 feederColor = vec3(0x83 / 255.0, 0xa5 / 255.0, 0x98 / 255.0);

uniform bool enableWave = true;
uniform bool enableFog = false;

uniform int charPos = 1;
uniform float feederPos = 0.0;
uniform bool isErasing = false;
uniform bool isFeeder = false;
uniform int wavePos = 0;

const float PI = asin(1) * 2;

void
main()
{
    float s = texture(sdfTex, fragTexCoord).r;
    float c = 1.0 / fwidth(s);
    float d = (s - 0.5) * c;
    float alpha = clamp(d + 0.5, 0.0, 1.0);
    // gl_FragColor = vec4(charColor, min(alpha + 0.5, 1.0));
    float splashFactor = smoothstep(24, 23, fragDepth);
    float fogFactor = enableFog ? 0.2 + ((fragDepth / 35) + 0.5) * 0.8 : 1;
    float waveFactor;
    if (enableWave && ! isFeeder) {
        int waveFactorI = (charPos + (70 - wavePos)) % 22;
        waveFactor = 0.5 + 0.5 * sin((waveFactorI / (22.0 - 1.0)) * (PI / 2));
    } else {
        waveFactor = 1;
    }
    float eraseFactor = isErasing ? smoothstep(0, -1, feederPos - charPos) : 1;
    float appearFactor = isErasing ? 1 : smoothstep(0, 1, feederPos - charPos);
    gl_FragColor = waveFactor * fogFactor * vec4(isFeeder ? 1.5 * feederColor : charColor, eraseFactor * splashFactor * alpha);
}
