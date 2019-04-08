//
// Created by Sviatoslav Prylutsky on 2/5/19.
//


#include "headers/player.h"
#include "headers/map.h"
#include "headers/bomb.h"
#include "headers/cell.h"
#include "../network/headers/server.h"

void playerInterfaceController(Object *player , char key)
{
    if(player->alive==0)
    {
        return;
    }
    pthread_t thread;
    switch(key)
    {
        case 'w':
            movePlayerToCell(player,player->posY-1 , player->posX);
            break;
        case 's':
            movePlayerToCell(player,player->posY+1 , player->posX);
            break;
        case 'd':
            movePlayerToCell(player,player->posY , player->posX+1);
            break;
        case 'a':
            movePlayerToCell(player,player->posY , player->posX-1);
            break;
        case 'b':
            if (pthread_create(&thread, NULL, playerPlaintTheBomb, player) != 0)
            {
                printf("main error: can't create thread \n");
            }
            break;
        default:
            printf("\n Wrong key %c \n",key);
            break;
    }

}

void movePlayerToCell(Object *player,int y, int x)
{
    if(canPlayerMoveToCell(player,y,x))
    {
        removeObjFromCell(player,player->posY,player->posX);
        addObjToCell(player,y,x);
    }

}

int canPlayerMoveToCell(Object *player, int y, int x)
{
    if(!player)
    {
        return 0;
    }
    Object *currentCell = getCell(y,x);
    Object *currentObject = currentCell;
    while(currentObject->next)
    {
        Object *tmp = currentObject->next;
        if(tmp->type == WALL || tmp->type == BLOCK || tmp->type == BOMB)
        {
            return 0;
        }
        if(tmp->type == BONUS)
        {
            player->bombsCnt=-1;
            removeObjFromCell(tmp,tmp->posY,tmp->posX);
            return 1;
        }
        currentObject = tmp;
    }
    return 1;
}