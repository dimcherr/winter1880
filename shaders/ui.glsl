@ctype mat4 Matrix
@ctype vec2 glm::vec2
@ctype vec3 glm::vec3
@ctype vec4 glm::vec4

@vs ui_vs
layout(binding=0) uniform vs_params {
    vec2 disp_size;
};
in vec2 position;
in vec2 texcoord0;
in vec4 color0;
in vec4 offset; // x - xoffset, y - yoffset, z - xpivot, w - ypivot
in vec4 deform; // x - speed, y - amplitude, z - time, w - scaleAmplitude
out vec2 uv;
out vec4 color;
out vec2 fromCenter0;
void main() {
    vec2 fromCenter = position - vec2(offset.z, offset.w);
    float animStrength = max(0.f, 100.f - length(fromCenter)) * 0.01f;
    float waveAmp = deform.y;
    float scaleAmp = deform.w * animStrength;

    vec2 normFromCenter = normalize(fromCenter);

    vec2 finalPos = position + normFromCenter * sin(deform.z * deform.x) * scaleAmp;
    finalPos = finalPos + vec2(offset.x, offset.y);

    vec4 pos = vec4(((finalPos/disp_size)-0.5)*vec2(2.0,-2.0), 0.5, 1.0);
    gl_Position = vec4(pos.x, pos.y + sin(deform.z * deform.x + texcoord0.x * 5.f) * waveAmp, pos.z, pos.w);
    uv = texcoord0;
    color = color0;
    fromCenter0 = fromCenter;
}
@end

@fs ui_fs
layout(binding=0) uniform texture2D tex;
layout(binding=0) uniform sampler smp;
in vec2 uv;
in vec4 color;
in vec2 fromCenter0;
out vec4 frag_color;
void main() {
    vec4 hoverTint = mix(vec4(0.f, 0.f, 0.f, 0.f), vec4(0.5f, 0.5f, 0.5f, 0.f), max(0.f, 100.f - length(fromCenter0)) * 0.01f);
    frag_color = texture(sampler2D(tex, smp), uv) * (color + hoverTint);
}
@end

@program ui ui_vs ui_fs