#define RAYGUI_IMPLEMENTATION
#include "../include/creact.h"

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
    if(creact) {
        return creact;
    }
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

    creactApp(creact);

    EndTextureMode();

    creact->currentFrame = LoadImageFromTexture(creact->target.texture);

    return (unsigned char*)creact->currentFrame.data;
}

void downloadSucceeded(emscripten_fetch_t *fetch) {
    requestStruct* req = (requestStruct*)fetch->userData;
    snprintf(req->response, BUFFER_SIZE, "%.*s", (int)fetch->numBytes, fetch->data);
    req->creact->isDownloading = false;
    EM_ASM({
            if (window.wakeUpCreact) {
                window.wakeUpCreact();
            }
        });
    free(req);
    emscripten_fetch_close(fetch);
}

void downloadFailed(emscripten_fetch_t *fetch) {
    requestStruct* req = (requestStruct*)fetch->userData;
    snprintf(req->response, BUFFER_SIZE, "Erreur HTTP : %d", fetch->status);
    req->creact->isDownloading = false;
    EM_ASM({
            if (window.wakeUpCreact) {
                window.wakeUpCreact();
            }
        });
    free(req);
    emscripten_fetch_close(fetch);
}

void get(Creact* creact, char* url, char response[BUFFER_SIZE]){

    requestStruct* req = (requestStruct*)malloc(sizeof(requestStruct));
    req->creact = creact;
    req->response = response;
    if (req == NULL) return;

    if (creact->isDownloading) return;
    creact->isDownloading = true;

    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);

    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.userData = req;
    attr.onsuccess = downloadSucceeded;
    attr.onerror = downloadFailed;

    emscripten_fetch(&attr, url);
}


void post(Creact* creact, char* url, char* postData, char response[BUFFER_SIZE]){

    if (creact->isDownloading) return;

    requestStruct* req = (requestStruct*)malloc(sizeof(requestStruct));
    if (req == NULL) return;

    req->creact = creact;
    req->response = response;

    creact->isDownloading = true;

    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);

    strcpy(attr.requestMethod, "POST");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;

    const char* headers[] = {"Content-Type", "application/x-www-form-urlencoded", NULL};
    attr.requestHeaders = headers;

    attr.requestData = postData;
    attr.requestDataSize = strlen(postData);

    attr.userData = req;
    attr.onsuccess = downloadSucceeded;
    attr.onerror = downloadFailed;

    emscripten_fetch(&attr, url);
}
