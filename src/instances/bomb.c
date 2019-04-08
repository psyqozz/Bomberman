//
// Created by Sviatoslav Prylutsky on 2/5/19.
//

#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include "headers/bomb.h"
#include "headers/object.h"
#include "headers/cell.h"
#include "headers/map.h"
#include "../network/headers/server.h"
#include "../gui/headers/gui.h"


pthread_mutex_t exploseBombMutex;

void generateBonus(Object *obj);

void * playerPlaintTheBomb(void *args)
{
    struct timespec animation;
    animation.tv_sec = 0;
    animation.tv_nsec = 225000000;
    Mix_PlayChannel(-1, getGame()->explosion, 0);
    Object *player = (Object*) args;
    if(player->bombsCnt>0)
    {
        return 0;
    }
    player->bombsCnt+=1;
    Object **bombs = calloc(5, sizeof(Object*));
    int showParams[5] = {3,21,22,23,24};
    int positionYParams[5] = {0,-1,1,0,0};
    int positionXParams[5] = {0,0,0,-1,1};

    for(int i=0;i<5;i++)
    {
        bombs[i] = generateNewObject(showParams[i],player->posY+positionYParams[i],player->posX+positionXParams[i]);
    }
    addObjToCell(bombs[0],bombs[0]->posY,bombs[0]->posX);
    sleep(1);
    bombs[0]->textureId = 20;
    getCell(bombs[0]->posY,bombs[0]->posX)->last = getProritaryAppairanceByObject(getCell(bombs[0]->posY,bombs[0]->posX));
    for(int i=1;i<5;i++)
    {
        if(bombs[i]!=NULL)
        {
            addObjToCell(bombs[i],bombs[i]->posY,bombs[i]->posX);
        }
    }
    printMaps();
    notificateAllClients();
    nanosleep(&animation , NULL);
    //sleep(1);
    for(int i=0;i<5;i++)
    {
        if(bombs[i]!=NULL)
        {
            explose(bombs[i], player);
            removeObjFromCell(bombs[i], bombs[i]->posY, bombs[i]->posX);
            free(bombs[i]);
        }
    }
    free(bombs);
    notificateAllClients();
    printMaps();
    player->bombsCnt-=1;
    return 0;
}

void explose(Object *explosion, Object *bombOwner)
{
    Object *targetCell = getCell(explosion->posY,explosion->posX);
    pthread_mutex_lock(&exploseBombMutex);
    Object *current = targetCell;
    if(current->next)
    {
        while (current->next != NULL)
        {
            Object *tmp = current->next;
            if(current->type != BLOCK && current->type != CELL && current != explosion && current->type != EXPLOSION && current->type != BONUS && current->type != BOMB)
            {
                if(current->type == WALL)
                {
                    generateBonus(current);
                }

                removeObjFromCell(current,current->posY,current->posX);

                if(current->type == PLAYER)
                {
                    bombOwner->score++;
                    current->alive = 0;
                    checkGameOver();
                }
                else
                {
                    free(current);
                }

            }
            current = tmp;
        }
    }
   pthread_mutex_unlock(&exploseBombMutex);
}

void generateBonus(Object *obj) {
    int r = rand() % 100;

    if(r<40) {
        Object *bonus  =  generateNewObject(5, obj->posY, obj->posX);
        addObjToCell(bonus, bonus->posY, bonus->posX);
    }
}