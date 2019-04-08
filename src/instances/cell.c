//
// Created by Sviatoslav Prylutsky on 2/5/19.
//

#include "headers/cell.h"
#include "headers/map.h"
#include "../network/headers/server.h"

Object *getCell(int y, int x)
{
    Map *map = getMapInstance();
    Object *cell = &map->cells[y][x];
    return cell;
}

void newCell(int mapParam, int pY, int pX)
{
    getMapInstance()->cells[pY][pX] = *generateNewObject( mapParam,  pY,  pX);
    setCellInServerMap( pY, pX , getCharFromInt(0));
}




