#pragma once
#include "tun/log.h"
#include "sokol_app.h"

enum class Key : int {
    q = SAPP_KEYCODE_Q,
    w = SAPP_KEYCODE_W,
    e = SAPP_KEYCODE_E,
    r = SAPP_KEYCODE_R,
    t = SAPP_KEYCODE_T,
    y = SAPP_KEYCODE_Y,
    u = SAPP_KEYCODE_U,
    i = SAPP_KEYCODE_I,
    o = SAPP_KEYCODE_O,
    p = SAPP_KEYCODE_P,

    a = SAPP_KEYCODE_A,
    s = SAPP_KEYCODE_S,
    d = SAPP_KEYCODE_D,
    f = SAPP_KEYCODE_F,
    g = SAPP_KEYCODE_G,
    h = SAPP_KEYCODE_H,
    j = SAPP_KEYCODE_J,
    k = SAPP_KEYCODE_K,
    l = SAPP_KEYCODE_L,

    z = SAPP_KEYCODE_Z,
    x = SAPP_KEYCODE_X,
    c = SAPP_KEYCODE_C,
    v = SAPP_KEYCODE_V,
    b = SAPP_KEYCODE_B,
    n = SAPP_KEYCODE_N,
    m = SAPP_KEYCODE_M,

    shift = SAPP_KEYCODE_LEFT_SHIFT,
    tab = SAPP_KEYCODE_TAB,
    space = SAPP_KEYCODE_SPACE,
    esc = SAPP_KEYCODE_ESCAPE,
    count = 349,
};

enum class MouseButton : int {
    left = SAPP_MOUSEBUTTON_LEFT,
    right = SAPP_MOUSEBUTTON_RIGHT,
    middle = SAPP_MOUSEBUTTON_MIDDLE,
    count,
};
