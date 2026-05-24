#include "views.h"

void indexView(Creact* creact){
    GuiPanel((Rectangle){ creact->globals->px, creact->globals->py, creact->globals->panelW, creact->globals->panelH }, "View 1");
}
