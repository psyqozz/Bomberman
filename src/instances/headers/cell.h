//
// Created by Sviatoslav Prylutsky on 2/5/19.
//

#ifndef BOMBERMAN_CELL_H
#define BOMBERMAN_CELL_H


#include "object.h"

void addObjToCell(Object *obj, int x, int y);
Object *getCell(int y, int x);
void newCell(int mapParam, int pY, int pX);
#endif //BOMBERMAN_CELL_H
