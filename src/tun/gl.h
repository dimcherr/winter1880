#pragma once
#include "tun/list.h"
#include "tun/math.h"
#include "tun/file.h"
#include "sokol_gfx.h"
#include "sokol_app.h"
#include "imgui/imgui.h"
#include "sokol_imgui.h"
#include "grid.glsl.h"
#include "pbr.glsl.h"
#include "color.glsl.h"
#include "draw.glsl.h"

namespace gl {

using Image = sg_image;
using Buffer = sg_buffer;
using Sampler = sg_sampler;
using Shader = sg_shader;
using Pipeline = sg_pipeline;
using Image = sg_image;

struct GridMaterial {
    Pipeline pipeline {};
    grid_vs_params_t vsParams {};
    grid_fs_params_t fsParams {};
};
void UseGridMaterial();
void UpdateGridMaterial();

struct ColorMaterial {
    Pipeline pipeline {};
    color_vs_params_t vsParams {};
    color_fs_params_t fsParams {};
};
void UseColorMaterial();
void UpdateColorMaterial();

struct PBRMaterial {
    Pipeline pipeline {};
    pbr_vs_params_t vsParams {};
    pbr_fs_params_t fsParams {};
    Sampler sampler {};
};
void UsePBRMaterial();
void UpdatePBRMaterial();

struct DrawMaterial {
    Pipeline pipeline {};
    draw_vs_params_t vsParams {};
    draw_fs_params_t fsParams {};
};
void UseDrawMaterial();
void UpdateDrawMaterial();

void UseMesh(Buffer vertexBuffer, Buffer indexBuffer, int elementCount);

void SetupRenderer();
Buffer CreateVertexBuffer(const List<float>& vertices);
Buffer CreateIndexBuffer(const List<uint16_t>& indices);
Image CreateImage(const Bytes& data);
void BeginRenderPass(Color clearColor);
void UpdateBindings();
void Draw();
void EndRenderPass();
void EndDrawing();
void Shutdown();
bool BeginDrawUI();
void EndDrawUI();
void RenderUIBuffer();
void HandleUIEvent(const sapp_event* event);
ImFont* LoadFont(StringView path, float fontSize);

class RenderState {
public:
    int elementCount {};
    GridMaterial gridMaterial {};
    PBRMaterial pbrMaterial {};
    ColorMaterial colorMaterial {};
    DrawMaterial drawMaterial {};
    sg_bindings bindings {};

    static RenderState& Get() {
        static RenderState state;
        return state;
    }
    RenderState(const RenderState&) = delete;
    RenderState& operator=(const RenderState&) = delete;
private:
    RenderState() {}
};

RenderState& State();

}

template <>
struct std::formatter<gl::Buffer> : std::formatter<std::string> {
    auto format(gl::Buffer buffer, format_context& ctx) const {
        return formatter<string>::format(std::format("{}", buffer.id), ctx);
    }
};
