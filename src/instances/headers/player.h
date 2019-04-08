//
// Created by Sviatoslav Prylutsky on 2/5/19.
//

#ifndef BOMBERMAN_PLAYER_H
#define BOMBERMAN_PLAYER_H


#include <stdint.h>
#include <pthread.h>
#include <stdio.h>
#include "object.h"

int canPlayerMoveToCell(Object *player, int y, int x);
void movePlayerToCell(Object *player,int y, int x);
void playerInterfaceController(Object *player , char key);
#endif //BOMBERMAN_PLAYER_H
