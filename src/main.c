#include "creact.h"
#include <stdio.h>
#include <string.h>


static int currentMenu = 0;
static float volume = 50.0f;
static bool notifications = true;
static bool showLogoutModal = false;
static char searchBox[64] = "Rechercher...";
static bool searchBoxEditMode = false;

char serverData[BUFFER_SIZE];
char* postdata = "";


typedef struct Globals {

    int x;

} Globals;



void creactApp(Creact* creact) {
    // On récupère la taille dynamique de ton canvas généré par le JS !
    if(creact->initGlobals == false){
        creact->globals = malloc(sizeof(Globals));
        creact->initGlobals = true;
    }

    creact->globals->x = 1;
    int w = GetScreenWidth();
    int h = GetScreenHeight();
    printf("[DEBUG] search : %s\n", serverData);

    // 1. FOND DE LA PAGE
    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

    // ==========================================
    // 2. NAVBAR (Barre de navigation en haut)
    // ==========================================
    GuiPanel((Rectangle){ 0, 0, w, 60 }, NULL);
    GuiLabel((Rectangle){ 20, 15, 200, 30 }, "#198# CREACT.JS ADMIN");

    // Barre de recherche centrée
    if (GuiTextBox((Rectangle){ w / 2 - 150, 15, 300, 30 }, serverData, 64, searchBoxEditMode)) {
        searchBoxEditMode = !searchBoxEditMode;
    }

    // Bouton de déconnexion à droite
    if (GuiButton((Rectangle){ w - 120, 15, 100, 30 }, "#159# Logout")) {
        post(creact, "http://localhost:8080/verb", postdata, serverData,NULL);
    }

    // ==========================================
    // 3. SIDEBAR (Menu latéral à gauche)
    // ==========================================
    GuiPanel((Rectangle){ 0, 60, 200, h - 60 }, NULL);

    // Un menu avec des onglets qui met à jour la variable "currentMenu"
    GuiToggleGroup((Rectangle){ 20, 80, 160, 30 }, "Dashboard\nParametres\nUtilisateurs\nStatistiques", &currentMenu);

    // ==========================================
    // 4. MAIN CONTENT (Zone de contenu principal)
    // ==========================================
    // Un panel qui prend le reste de l'écran
    GuiPanel((Rectangle){ 220, 80, w - 240, h - 100 }, "Contenu Principal");

    // Rendu conditionnel selon l'onglet actif
    if (currentMenu == 0) {
        GuiLabel((Rectangle){ 240, 120, 300, 30 }, "Bienvenue sur ton application WebAssembly !");

        GuiCheckBox((Rectangle){ 240, 170, 20, 20 }, "Activer les notifications", &notifications);

        GuiLabel((Rectangle){ 240, 220, 100, 30 }, "Volume Serveur :");
        GuiSlider((Rectangle){ 350, 225, 200, 20 }, "0%", "100%", &volume, 0.0f, 100.0f);

    } else if (currentMenu == 1) {
        GuiLabel((Rectangle){ 240, 120, 200, 30 }, "Ecran des parametres (En construction)");
    } else {
        GuiLabel((Rectangle){ 240, 120, 200, 30 }, "Pas de donnees pour cet onglet.");
    }

    // ==========================================
    // 5. MODAL OVERLAY (La popup de déconnexion)
    // ==========================================
    // Doit toujours être appelée en dernier pour se dessiner PAR-DESSUS le reste
    if (showLogoutModal) {
        // Petit hack sympa : on dessine un rectangle noir semi-transparent sur tout l'écran
        // pour assombrir le site derrière la popup !
        DrawRectangle(0, 0, w, h, Fade(BLACK, 0.6f));

        int result = GuiMessageBox(
            (Rectangle){ w / 2 - 125, h / 2 - 60, 250, 120 },
            "#159# Deconnexion",
            "Es-tu sur de vouloir quitter ?",
            "Oui;Annuler"
        );

        // Gestion des clics sur la modale
        if (result == 1) {
            // L'utilisateur a cliqué sur "Oui"
            showLogoutModal = false;
        } else if (result == 0 || result == 2) {
            // L'utilisateur a cliqué sur "Annuler" ou sur la croix
            showLogoutModal = false;
        }
    }
}
