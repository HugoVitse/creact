# Creact

Creact is a tiny frontend framework in C that renders to the browser with WebAssembly.
It uses raylib + raygui for immediate-mode UI, and a small JavaScript bridge to
present frames on a browser canvas and forward input events.

## Features

- Write UI in C with raygui widgets
- Build to WebAssembly with Emscripten
- Simple JS bridge for rendering and input
- Optional HTTP GET/POST helpers via Emscripten fetch
- Example app included in myApp/

## How It Works

1. Your app implements creactApp(Creact* creact).
2. The C runtime renders into a raylib render texture.
3. JavaScript pulls raw RGBA pixels from WASM memory and draws them to a canvas.
4. Mouse events are forwarded back into the C runtime.

## Project Layout

- include/creact.h: public API and bridge functions
- src/creact.c: runtime implementation
- static/creact_draw.js: JS renderer and event bridge
- static/index.html: minimal HTML loader
- myApp/: example application using the library

## Build

Build the main demo in the repository root:

```sh
make creact
```

Artifacts land in release/ (HTML + JS + WASM). Serve that folder with a static
server to run in the browser.

Build the reusable library bundle for the example app:

```sh
make creact-library
```

## Minimal App Example

```c
#include "creact.h"

void creactApp(Creact* creact) {
    int w = GetScreenWidth();
    int h = GetScreenHeight();

    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
    GuiLabel((Rectangle){ 20, 20, w - 40, 30 }, "Hello from Creact");
}
```

## Quickstart

You can start developping quickly by downloading a prebuilt app :

```sh
curl -sSL https://raw.githubusercontent.com/HugoVitse/creact/refs/heads/master/install.sh | bash
cd myApp
make -b
```

Static files are located in `release` folder and ready to be served
