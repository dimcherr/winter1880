@ctype mat4 Matrix
@ctype vec2 glm::vec2
@ctype vec3 glm::vec3
@ctype vec4 glm::vec4

@vs grid_vs
layout(binding=0) uniform grid_vs_params {
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

@fs grid_fs
layout(binding=1) uniform grid_fs_params {
    int segmentCount;
    float segmentSize;
    vec3 color;
};

in vec2 uv0;

out vec4 fragColor;

void main() {
    if (mod(uv0.x * segmentCount * segmentSize, segmentSize) < segmentSize * 0.03 || mod(uv0.y * segmentCount * segmentSize, segmentSize) < segmentSize * 0.03) {
        fragColor = vec4(color, 1.0);
    } else {
        discard;
    }
}
@end

@program grid grid_vs grid_fs
