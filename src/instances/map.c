//
// Created by Sviatoslav Prylutsky on 2/5/19.
//



#include "headers/map.h"
#include "../gui/headers/gui.h"
#include "headers/cell.h"
#include "../network/headers/server.h"

int configMap[MAPY][MAPX] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 2, 0, 2, 0, 0, 2, 2, 2, 0, 0, 1},
        {1, 0, 1, 2, 0, 1, 0, 1, 2, 1, 2, 0, 1, 0, 1},
        {1, 2, 0, 0, 1, 0, 2, 0, 2, 0, 2, 0, 2, 0, 1},
        {1, 0, 1, 0, 2, 2, 0, 1, 2, 0, 1, 2, 1, 0, 1},
        {1, 2, 0, 2, 2, 1, 2, 0, 0, 1, 2, 0, 2, 2, 1},
        {1, 0, 2, 1, 2, 2, 0, 1, 0, 0, 2, 1, 0, 0, 1},
        {1, 2, 1, 0, 0, 1, 0, 2, 2, 1, 0, 2, 1, 2, 1},
        {1, 2, 0, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 0, 1},
        {1, 0, 0, 2, 1, 2, 2, 0, 1, 2, 2, 0, 0, 2, 1},
        {1, 2, 1, 0, 2, 0, 2, 1, 2, 1, 0, 0, 1, 2, 1},
        {1, 2, 0, 1, 2, 2, 1, 0, 2, 0, 2, 1, 0, 0, 1},
        {1, 0, 1, 2, 0, 1, 2, 1, 0, 1, 0, 2, 1, 0, 1},
        {1, 0, 0, 0, 2, 0, 2, 0, 2, 2, 2, 2, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

Map *getMapInstance()
{
    static Map *gameMap = NULL;
    if(gameMap == NULL)
    {
        gameMap = malloc(sizeof(Map));
        gameMap -> mapSizeY = MAPY;
        gameMap -> mapSizeX = MAPX;
        gameMap -> cells = calloc(gameMap -> mapSizeX, sizeof(Object**)); // 10 == X dimension
        for(int y = 0; y<gameMap -> mapSizeX; y++)
        {
            gameMap -> cells[y] = calloc(gameMap -> mapSizeY,sizeof(Object));
        }
    }
    return gameMap;
}

void initMapByObjects()
{
    for(int y = 0; y < MAPY; y++)
    {
        for(int x = 0; x < MAPX; x++)
        {
            newCell(0,y,x);
            if(configMap[y][x]>0)
            {
                Object *tmp =  generateNewObject(configMap[y][x], y,x);
                addObjToCell(tmp,y,x);
            }
        }
    }
}

void printMaps()
{
   // printConsoleMap();
   // remapMap();
   // notificateAllClients();
}

char getCharFromInt(int intValue)
{
    char charValue = 'z'; //z undefined
    switch (intValue)
    {
        case 0:
            charValue = '.';
            break;

        case 2:
            charValue = '#';
            break;

        case 3:
            charValue = '@';
            break;

        case 4:
            charValue = 'z';
            break;

        case 5:
            charValue = '?';
            break;

        case 11:
            charValue = 'A';
            break;

        case 12:
            charValue = 'B';
            break;

        case 13:
            charValue = 'C';
            break;

        case 14:
            charValue = 'D';
            break;

        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
            charValue = '*';
            break;

        case 99:
            charValue = 'X';
            break;

        default:
            charValue = 'z';
            break;
    }
    return charValue;
}

