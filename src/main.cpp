#include "Game.h"
#include "sokol_app.h"
#include "sokol_log.h"

sapp_desc sokol_main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    sapp_desc desc {};
    desc.init_cb = &game::Create;
    desc.frame_cb = &game::Update;
    desc.cleanup_cb = &game::Destroy;
    desc.event_cb = &game::OnEvent;
    desc.width = 1920;
    desc.height = 1080;
    desc.sample_count = 4;
    desc.fullscreen = false;
    desc.window_title = "Winter 1880";
    desc.icon.sokol_default = true;
    desc.logger.func = slog_func;
    desc.html5_bubble_mouse_events = true;
    return desc;
}