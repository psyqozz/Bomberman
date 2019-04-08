//
// Created by Sviatoslav Prylutsky on 2/5/19.
//

#ifndef BOMBERMAN_MAP_H
#define BOMBERMAN_MAP_H

#include "../../instances/headers/object.h"

#define MAPX 15
#define MAPY 15

typedef struct  {
    int mapSizeY;
    int mapSizeX;
    Object **cells;
}Map;

void newCell(int mapParam,  int pY ,int pX);
char getCharFromInt(int intValue);
char **getCharMap();
void setCellInCharMap(int y , int x, char ch);
void printMaps();
void initMapByObjects();
void printConsoleMap();
Map *getMapInstance();
void removeObjFromCell(Object *obj,int y, int x);
#endif //BOMBERMAN_MAP_H

