#pragma once

#include "raylib.h"
#include "stdlib.h"

static int custom_mouse_x = 0;
static int custom_mouse_y = 0;
static bool custom_mouse_down = false;
static bool custom_mouse_pressed = false;
static bool custom_mouse_released = false;
static bool previous_mouse_down = false;

Vector2 CustomGetMousePosition(void);
bool CustomIsMouseButtonDown(int button);
bool CustomIsMouseButtonPressed(int button);
bool CustomIsMouseButtonReleased(int button);

#define GetMousePosition CustomGetMousePosition
#define IsMouseButtonDown CustomIsMouseButtonDown
#define IsMouseButtonPressed CustomIsMouseButtonPressed
#define IsMouseButtonReleased CustomIsMouseButtonReleased

#include "raygui.h"

#include <emscripten.h>

typedef struct Creact {

    RenderTexture2D target;
    Image currentFrame;

} Creact;

EMSCRIPTEN_KEEPALIVE
void update_mouse(int x, int y, int is_down);


EMSCRIPTEN_KEEPALIVE
void init_headless(int width, int height, Creact* creact);

EMSCRIPTEN_KEEPALIVE
void resize_frame(int width, int height, Creact* creact);

EMSCRIPTEN_KEEPALIVE
unsigned char* render_frame(Creact* creact);

EMSCRIPTEN_KEEPALIVE
Creact* initCreact();

EMSCRIPTEN_KEEPALIVE
void destroyCreact(Creact* creact);


void creactApp();
