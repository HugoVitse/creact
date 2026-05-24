#pragma once
#include "creact.h"

typedef struct Globals {

    int w;
    int h;
    int panelW;
    int panelH;
    int px;
    int py;
    int currentScreen;

} Globals;


void indexView(Creact* creact);
