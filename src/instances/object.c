//
// Created by Sviatoslav Prylutsky on 2/15/19.
//

#include <malloc.h>
#include <zconf.h>
#include <pthread.h>
#include "headers/object.h"
#include "headers/cell.h"
#include "headers/map.h"
#include "../network/headers/server.h"

pthread_mutex_t addObjMutex;
pthread_mutex_t rmvObjMutex;


void initMutex()
{
    pthread_mutex_init(&addObjMutex, NULL);
    pthread_mutex_init(&rmvObjMutex, NULL);
};

void destroyMutex()
{
    pthread_mutex_destroy(&addObjMutex);
    pthread_mutex_destroy(&rmvObjMutex);
};

Object *generateNewObject(int typeId, int y, int x)
{
    if(y < 0 || y > getMapInstance()->mapSizeY || x < 0 || x > getMapInstance()->mapSizeX)
    {
        return NULL;
    }
    Object *obj = (Object*)malloc(sizeof(Object));
    obj->textureId = typeId;
    obj->posX = x;
    obj->posY = y;
    obj->last = obj;
    obj->size = 0;
    obj->next = NULL;
    obj->alive = 1;
    switch (typeId)
    {
        case 0:
            obj->type = CELL;
            obj->size = 0;
            break;

        case 1:
            obj->type = BLOCK;
            obj->textureId = 99;
            break;

        case 2:
            obj->type = WALL;
            break;

        case 3:
            obj->type = BOMB;
            break;

        case 5:
            obj->type = BONUS;
            break;

        case 11:
            obj->type = PLAYER;
            obj->bombsCnt = 0;
            obj->id = 1;
            break;
        case 12:
            obj->type = PLAYER;
            obj->bombsCnt = 0;
            obj->id = 2;
            break;
        case 13:
            obj->type = PLAYER;
            obj->bombsCnt = 0;
            obj->id = 3;
            break;
        case 14:
            obj->type = PLAYER;
            obj->bombsCnt = 0;
            obj->id = 4;
            break;

        case 21:
            obj->type = EXPLOSION;
            obj->id = typeId;
            break;
        case 22:
            obj->type = EXPLOSION;
            obj->id = typeId;
            break;
        case 23:
            obj->type = EXPLOSION;
            obj->id = typeId;
            break;
        case 24:
            obj->type = EXPLOSION;
            obj->id = typeId;
            break;

        default:
            obj->type = OBJECT;
            obj->textureId =-1;
            break;
    }
    return obj;
}


void addObjToCell(Object *obj,int y, int x)
{
    Object *targetCell = getCell(y, x);
    pthread_mutex_lock(&addObjMutex);
    Object *current = targetCell->next;
    obj->posY = targetCell->posY;
    obj->posX = targetCell->posX;
    if(current!=NULL)
    {
        while(current->next!=NULL)
        {
            Object *tmp = current->next;
            if(tmp->textureId < obj ->textureId)
            {
                break;
            }
            current = tmp;
        }
        if(current!=NULL)
        {
            Object *tmp = current->next;
            current->next = obj;
            obj->next = tmp;

        }
    } else
    {
        targetCell->next = obj;
    }

    targetCell->last=getProritaryAppairanceByObject(targetCell);
    targetCell->size +=1;
    pthread_mutex_unlock(&addObjMutex);
}

Object * getProritaryAppairanceByObject(Object *cell){
    Object *highter = cell;
    Object *current = cell->next;
    while(current)
    {
        if(current->textureId > highter->textureId)
        {
            highter = current;
        }
        Object *tmp = current->next;
        current = tmp;
    }
    setCellInServerMap( highter->posY, highter->posX , getCharFromInt(highter->textureId));
    return highter;
}

void removeObjFromCell(Object *obj,int y, int x)
{
    if(obj==NULL)
    {
        return;
    }
    Object *targetCell = getCell(y, x);
    pthread_mutex_lock(&rmvObjMutex);
    Object *current = targetCell;
    if(current->next)
    {
        while (current->next != NULL)
        {
            Object *tmp = current->next;
            if (tmp == obj)
            {
                if (tmp->next != NULL)
                {
                    current->next = obj->next;
                } else
                {
                    current->next = NULL;
                }
                obj->next = NULL;
                obj->posY = -1;
                obj->posX = -1;
            }
            current = tmp;
        }
    }
    targetCell->last=getProritaryAppairanceByObject(targetCell);
    targetCell->size -=1;
    pthread_mutex_unlock(&rmvObjMutex);
}