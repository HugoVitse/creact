#include "creact.h"
#include "views.h"

void initGlobals(Creact* creact) {

    creact->globals = malloc(sizeof(Globals));
    creact->initGlobals = true;
}

void creactApp(Creact* creact) {
    if(creact->initGlobals == false) initGlobals(creact);

    creact->globals->w = GetScreenWidth();
    creact->globals->h = GetScreenHeight();

    creact->globals->panelW = 320;
    creact->globals->panelH = 400;
    creact->globals->px = creact->globals->w / 2 - creact->globals->panelW / 2;
    creact->globals->py = creact->globals->h / 2 - creact->globals->panelH / 2;

    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

    switch (creact->globals->currentScreen) {
        case 0:
            indexView(creact);
            break;
        default:
            indexView(creact);
            break;
    }
}
