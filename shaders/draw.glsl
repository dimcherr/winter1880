@ctype mat4 Matrix
@ctype vec2 glm::vec2
@ctype vec3 glm::vec3
@ctype vec4 glm::vec4

@vs draw_vs
layout(binding=0) uniform draw_vs_params {
    mat4 mvp;
    float displaceFactor;
};
in vec3 position;
in vec2 texcoord0;
out vec2 uv;
void main() {
    //vec4 pos = vec4(((vec2(position.x, position.y)/disp_size)-0.5)*vec2(2.0,-2.0), 0.5, 1.0);
    vec4 displace = mix(vec4(1.f, 1.f, 1.f, 1.f), vec4(texcoord0.x, texcoord0.y, texcoord0.x, 1.f), displaceFactor);
    gl_Position = mvp * vec4(position, 1.f) * displace;
    uv = texcoord0;
}
@end

@fs draw_fs
layout(binding=1) uniform draw_fs_params {
    vec4 color;
};
in vec2 uv;
out vec4 frag_color;
void main() {
    frag_color = color;
}
@end

@program draw draw_vs draw_fs