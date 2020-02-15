#version 130

in vec2 fragTexCoord;
in float fragDepth;

uniform sampler2D sdfTex;
uniform vec3 fillColor = vec3(0x45 / 255.0, 0x85 / 255.0, 0x88 / 255.0);

void
main()
{
    float s = texture(sdfTex, fragTexCoord).r;
    float c = 1.0 / fwidth(s);
    float d = (s - 0.5) * c;
    float alpha = clamp(d + 0.5, 0.0, 1.0);
    // gl_FragColor = vec4(fillColor, min(alpha + 0.5, 1.0));
    gl_FragColor = vec4(fillColor, smoothstep(24, 23, fragDepth) * alpha);
}
