#include "gl.h"
#include "stb_image.h"
#include "sokol_app.h"
#include "sokol_log.h"
#include "sokol_gfx.h"
#include "basisu/sokol_basisu.h"
#include "grid.glsl.h"
#include "pbr.glsl.h"

static gl::GridMaterial CreateGridMaterial();
static gl::ColorMaterial CreateColorMaterial();
static gl::PBRMaterial CreatePBRMaterial();
static gl::DrawMaterial CreateDrawMaterial();

gl::RenderState& gl::State() {
    return gl::RenderState::Get();
}

void gl::SetupRenderer() {
    sg_desc desc {};
    sg_environment env {};
    env.defaults.color_format = (sg_pixel_format) sapp_color_format();
    env.defaults.depth_format = (sg_pixel_format) sapp_depth_format();
    env.defaults.sample_count = sapp_sample_count();
    env.metal.device = sapp_metal_get_device();
    env.d3d11.device = sapp_d3d11_get_device();
    env.d3d11.device_context = sapp_d3d11_get_device_context();
    env.wgpu.device = sapp_wgpu_get_device();
    desc.environment = env;
    desc.logger.func = slog_func;
    desc.buffer_pool_size = 2048;
    desc.uniform_buffer_size = 4 * 1024 * 1024;
    sg_setup(desc);

    auto& state = gl::State();
    state.gridMaterial = CreateGridMaterial();
    state.colorMaterial = CreateColorMaterial();
    state.pbrMaterial = CreatePBRMaterial();
    state.drawMaterial = CreateDrawMaterial();

    sbasisu_setup();

    simgui_desc_t simgui_desc {};
    simgui_desc.logger.func = slog_func;
    simgui_desc.no_default_font = true; 
    simgui_setup(&simgui_desc);
}

gl::Buffer gl::CreateVertexBuffer(const List<float>& vertices) {
    sg_buffer_desc desc {};
    desc.data = sg_range { vertices.data(), vertices.size() * sizeof(float) };
    return sg_make_buffer(desc);
}

gl::Buffer gl::CreateIndexBuffer(const List<uint16_t>& indices) {
    sg_buffer_desc desc {};
    desc.data = sg_range { indices.data(), indices.size() * sizeof(uint16_t) };
    desc.type = SG_BUFFERTYPE_INDEXBUFFER;
    return sg_make_buffer(desc);
}

gl::Image gl::CreateImage(const Bytes& data) {
    return sbasisu_make_image(sg_range {(void*)data.data(), data.size()});
}

static gl::GridMaterial CreateGridMaterial() {
    gl::GridMaterial material {};
    gl::Shader shader = sg_make_shader(grid_shader_desc(sg_query_backend()));
    sg_pipeline_desc desc {};
    desc.layout.attrs[ATTR_grid_pos].format = SG_VERTEXFORMAT_FLOAT3;
    desc.layout.attrs[ATTR_grid_uv].format = SG_VERTEXFORMAT_FLOAT2;
    desc.layout.buffers[0].stride = 20;
    desc.shader = shader;
    desc.index_type = SG_INDEXTYPE_UINT16;
    desc.cull_mode = SG_CULLMODE_BACK;
    desc.face_winding = sg_face_winding::SG_FACEWINDING_CCW;
    desc.depth.write_enabled = true;
    desc.depth.compare = SG_COMPAREFUNC_LESS_EQUAL;
    desc.label = "grid-pipeline";
    material.pipeline = sg_make_pipeline(desc);
    return material;
}

void gl::UseGridMaterial() {
    const auto& material = State().gridMaterial;
    sg_apply_pipeline(material.pipeline);
}

void gl::UpdateGridMaterial() {
    const auto& material = State().gridMaterial;
    sg_apply_uniforms(UB_grid_vs_params, SG_RANGE(material.vsParams));
    sg_apply_uniforms(UB_grid_fs_params, SG_RANGE(material.fsParams));
}

static gl::ColorMaterial CreateColorMaterial() {
    gl::ColorMaterial material {};
    gl::Shader shader = sg_make_shader(color_shader_desc(sg_query_backend()));
    sg_pipeline_desc desc {};
    desc.layout.attrs[ATTR_color_pos].format = SG_VERTEXFORMAT_FLOAT3;
    desc.layout.attrs[ATTR_color_uv].format = SG_VERTEXFORMAT_FLOAT2;
    desc.layout.buffers[0].stride = 20;
    desc.shader = shader;
    desc.index_type = SG_INDEXTYPE_UINT16;
    desc.cull_mode = SG_CULLMODE_NONE;
    desc.depth.write_enabled = true;
    desc.depth.compare = SG_COMPAREFUNC_LESS_EQUAL;
    desc.label = "color-pipeline";
    material.pipeline = sg_make_pipeline(desc);
    return material;
}

void gl::UseColorMaterial() {
    const auto& material = State().colorMaterial;
    sg_apply_pipeline(material.pipeline);
}

void gl::UpdateColorMaterial() {
    const auto& material = State().colorMaterial;
    sg_apply_uniforms(UB_color_vs_params, SG_RANGE(material.vsParams));
    sg_apply_uniforms(UB_color_fs_params, SG_RANGE(material.fsParams));
}

static gl::PBRMaterial CreatePBRMaterial() {
    gl::PBRMaterial material {};
    gl::Shader shader = sg_make_shader(pbr_shader_desc(sg_query_backend()));
    sg_pipeline_desc desc {};
    desc.layout.attrs[ATTR_pbr_vertexPos].format = SG_VERTEXFORMAT_FLOAT3;
    desc.layout.attrs[ATTR_pbr_vertexNormal].format = SG_VERTEXFORMAT_FLOAT3;
    desc.layout.attrs[ATTR_pbr_vertexTexCoord].format = SG_VERTEXFORMAT_FLOAT2;
    desc.layout.attrs[ATTR_pbr_vertexTangent].format = SG_VERTEXFORMAT_FLOAT4;
    desc.layout.buffers[0].stride = 48;
    desc.shader = shader;
    desc.index_type = SG_INDEXTYPE_UINT16;
    desc.cull_mode = SG_CULLMODE_BACK;
    desc.depth.write_enabled = true;
    desc.depth.compare = SG_COMPAREFUNC_LESS_EQUAL;
    desc.label = "pbr-pipeline";
    desc.face_winding = sg_face_winding::SG_FACEWINDING_CCW;
    material.pipeline = sg_make_pipeline(desc);
    sg_sampler_desc smp_desc {};
    smp_desc.min_filter = sg_filter::SG_FILTER_NEAREST;
    smp_desc.mag_filter = sg_filter::SG_FILTER_NEAREST;
    smp_desc.wrap_u = sg_wrap::SG_WRAP_REPEAT;
    smp_desc.wrap_v = sg_wrap::SG_WRAP_REPEAT;
    material.sampler = sg_make_sampler(smp_desc);
    return material;
}

void gl::UsePBRMaterial() {
    const auto& material = State().pbrMaterial;
    sg_apply_pipeline(material.pipeline);
}

void gl::UpdatePBRMaterial() {
    const auto& material = State().pbrMaterial;
    sg_apply_uniforms(UB_pbr_vs_params, SG_RANGE(material.vsParams));
    sg_apply_uniforms(UB_pbr_fs_params, SG_RANGE(material.fsParams));
}

static gl::DrawMaterial CreateDrawMaterial() {
    gl::DrawMaterial material {};
    gl::Shader shader = sg_make_shader(draw_shader_desc(sg_query_backend()));
    sg_pipeline_desc desc {};
    desc.layout.attrs[ATTR_draw_position].format = SG_VERTEXFORMAT_FLOAT3;
    desc.layout.attrs[ATTR_draw_texcoord0].format = SG_VERTEXFORMAT_FLOAT2;
    desc.layout.buffers[0].stride = 20;
    desc.shader = shader;
    desc.index_type = SG_INDEXTYPE_UINT16;
    desc.cull_mode = SG_CULLMODE_NONE;
    desc.depth.write_enabled = true;
    desc.depth.compare = SG_COMPAREFUNC_ALWAYS;
    desc.label = "draw-pipeline";
    desc.face_winding = sg_face_winding::SG_FACEWINDING_CCW;

    desc.colors[0].write_mask = SG_COLORMASK_RGBA;
    desc.colors[0].blend.enabled = true;
    desc.colors[0].blend.src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA;
    desc.colors[0].blend.dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;

    material.pipeline = sg_make_pipeline(desc);
    return material;
}

void gl::UseDrawMaterial() {
    const auto& material = State().drawMaterial;
    sg_apply_pipeline(material.pipeline);
}

void gl::UpdateDrawMaterial() {
    const auto& material = State().drawMaterial;
    sg_apply_uniforms(UB_draw_vs_params, SG_RANGE(material.vsParams));
    sg_apply_uniforms(UB_draw_fs_params, SG_RANGE(material.fsParams));
}

void gl::UseMesh(Buffer vertexBuffer, Buffer indexBuffer, int elementCount)
{
    auto& state = State();
    state.bindings.vertex_buffers[0] = vertexBuffer;
    state.bindings.index_buffer = indexBuffer;
    state.elementCount = elementCount;
}

void gl::BeginRenderPass(Color clearColor) {
    sg_pass pass {};
    pass.action.colors[0].load_action = SG_LOADACTION_CLEAR;
    pass.action.colors[0].clear_value = {clearColor.r, clearColor.g, clearColor.b, 1.f};
    sg_swapchain swapchain {};
    swapchain.width = sapp_width();
    swapchain.height = sapp_height();
    swapchain.sample_count = sapp_sample_count();
    swapchain.color_format = (sg_pixel_format)sapp_color_format();
    swapchain.depth_format = (sg_pixel_format)sapp_depth_format();
    swapchain.metal.current_drawable = sapp_metal_get_current_drawable();
    swapchain.metal.depth_stencil_texture = sapp_metal_get_depth_stencil_texture();
    swapchain.metal.msaa_color_texture = sapp_metal_get_msaa_color_texture();
    swapchain.d3d11.render_view = sapp_d3d11_get_render_view();
    swapchain.d3d11.resolve_view = sapp_d3d11_get_resolve_view();
    swapchain.d3d11.depth_stencil_view = sapp_d3d11_get_depth_stencil_view();
    swapchain.wgpu.render_view = sapp_wgpu_get_render_view();
    swapchain.wgpu.resolve_view = sapp_wgpu_get_resolve_view();
    swapchain.wgpu.depth_stencil_view = sapp_wgpu_get_depth_stencil_view();
    swapchain.gl.framebuffer = sapp_gl_get_framebuffer();
    pass.swapchain = swapchain;
    sg_begin_pass(pass);
}

void gl::Draw() {
    const auto& state = State();
    sg_apply_bindings(&state.bindings);
    sg_draw(0, state.elementCount, 1);
}

void gl::EndRenderPass() {
    sg_end_pass();
}

void gl::EndDrawing() {
    sg_commit();
}

void gl::Shutdown() {
    sbasisu_shutdown();
    simgui_shutdown();
    sg_shutdown();
}

bool gl::BeginDrawUI() {
    const int width = sapp_width();
    const int height = sapp_height();
    simgui_new_frame({ width, height, sapp_frame_duration(), sapp_dpi_scale() });

    static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    bool open = true;
    return ImGui::Begin("tundra", &open, flags);
}

void gl::EndDrawUI() {
    ImGui::End();
}

void gl::RenderUIBuffer() {
    simgui_render();
}

void gl::HandleUIEvent(const sapp_event* event) {
    simgui_handle_event(event);
}

ImFont* gl::LoadFont(StringView path, float fontSize) {
    auto& io = ImGui::GetIO();
    ImFontConfig fontCfg;
    fontCfg.FontDataOwnedByAtlas = false;
    fontCfg.OversampleH = 2;
    fontCfg.OversampleV = 2;
    fontCfg.RasterizerMultiply = 1.5f;
    ImFont* font = io.Fonts->AddFontFromFileTTF(path.data(), fontSize, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    simgui_font_tex_desc_t font_texture_desc = {};
    font_texture_desc.min_filter = SG_FILTER_LINEAR;
    font_texture_desc.mag_filter = SG_FILTER_LINEAR;
    simgui_create_fonts_texture(&font_texture_desc);
    return font;
}
