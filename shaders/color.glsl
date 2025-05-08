@ctype mat4 Matrix
@ctype vec2 glm::vec2
@ctype vec3 glm::vec3
@ctype vec4 glm::vec4

@vs color_vs
layout(binding=0) uniform color_vs_params {
    mat4 mvp;
};

layout (location = 0) in vec4 pos;
layout (location = 1) in vec2 uv;

out vec2 uv0;

void main() {
    gl_Position = mvp * pos;
    uv0 = uv;
}
@end

@fs color_fs
layout(binding=1) uniform color_fs_params {
    vec4 color;
};

in vec2 uv0;

out vec4 fragColor;

void main() {
    fragColor = color;
}
@end

@program color color_vs color_fs
