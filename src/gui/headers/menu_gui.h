//
// Created by dy on 04/03/19.
//

#include "gui.h"
#include "../../instances/headers/player.h"
#include "../../instances/headers/map.h"
#include "../../instances/headers/cell.h"
#include "../../instances/headers/object.h"
#include "../../instances/headers/menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {

    int error;
    int choice;
    int ifIP;
    SDL_Window *Window;
    SDL_Renderer *Renderer;

    SDL_Texture *menuTilset;
    SDL_Texture *cursorBomb;

    SDL_Rect MenuSize;
    SDL_Rect cursor;

    Mix_Music *musique;
    Mix_Chunk *laser;

} Menu;

void showText(SDL_Texture* txt, SDL_Renderer* Renderer, char* mess, int x, int y);
ConnectionProps* choiceMode(Menu* menu);
int initParam(Menu* menu, ConnectionProps* param);
void destroyMenu(Menu* menu);
Menu* main_menu();
void showMenu(Menu* menu);
char* userWrite(Menu* menu);
void startMenu();

#ifndef BOMBERMAN_MENU_GUI_H
#define BOMBERMAN_MENU_GUI_H

#endif //BOMBERMAN_MENU_GUI_H
