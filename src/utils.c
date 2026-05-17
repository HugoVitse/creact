#define RAYGUI_IMPLEMENTATION
#include "../include/utils.h"

Vector2 CustomGetMousePosition(void) { return (Vector2){ (float)custom_mouse_x, (float)custom_mouse_y }; }
bool CustomIsMouseButtonDown(int button) { return (button == MOUSE_BUTTON_LEFT) && custom_mouse_down; }
bool CustomIsMouseButtonPressed(int button) { return (button == MOUSE_BUTTON_LEFT) && custom_mouse_pressed; }
bool CustomIsMouseButtonReleased(int button) { return (button == MOUSE_BUTTON_LEFT) && custom_mouse_released; }


EMSCRIPTEN_KEEPALIVE
void update_mouse(int x, int y, int is_down) {
    custom_mouse_x = x;
    custom_mouse_y = y;
    custom_mouse_down = (is_down != 0);
}



EMSCRIPTEN_KEEPALIVE
void init_headless(int width, int height, Creact* creact) {
    SetConfigFlags(FLAG_WINDOW_HIDDEN);
    InitWindow(width, height, "raygui - full browser");
    creact->target = LoadRenderTexture(width, height);
}

EMSCRIPTEN_KEEPALIVE
void resize_frame(int width, int height, Creact* creact) {
    UnloadRenderTexture(creact->target);
    SetWindowSize(width, height);
    creact->target = LoadRenderTexture(width, height);
}

EMSCRIPTEN_KEEPALIVE
Creact* initCreact() {
    Creact* creact = calloc(1,sizeof(Creact));
    if(creact) return creact;
    else return NULL;
}

EMSCRIPTEN_KEEPALIVE
void destroyCreact(Creact* creact) {
    free(creact);
}

EMSCRIPTEN_KEEPALIVE
unsigned char* render_frame(Creact* creact) {
    custom_mouse_pressed = (custom_mouse_down && !previous_mouse_down);
    custom_mouse_released = (!custom_mouse_down && previous_mouse_down);
    previous_mouse_down = custom_mouse_down;
    UnloadImage(creact->currentFrame);


    BeginTextureMode(creact->target);

    creactApp();

    EndTextureMode();

    creact->currentFrame = LoadImageFromTexture(creact->target.texture);

    return (unsigned char*)creact->currentFrame.data;
}
