
#include "network/headers/server.h"
#include "network/headers/client.h"
#include "config.h"
#include "instances/headers/map.h"
#include "gui/headers/gui.h"
#include "instances/headers/player.h"
#include "instances/headers/cell.h"
#include "gui/headers/menu_gui.h"


void * runServer(void *args)
{
    ConnectionProps *param = (ConnectionProps*) args;
    initMutex();
    startServer(param->port);
    destroyMutex();
    return 0;
}

void runClient(ConnectionProps* param)
{
    gameInit();
    startClient(param->port, param->ip);
}

void startMenu()
{
    Menu *mainMenu = main_menu();
    ConnectionProps* param = malloc(sizeof(ConnectionProps));
    param = choiceMode(mainMenu);
    if(mainMenu->choice == 1)
    {
        SDL_HideWindow(mainMenu->Window);
        destroyMenu(mainMenu);
        runClient(param);
    }
    if (mainMenu->choice == 2)
    {
        SDL_HideWindow(mainMenu->Window);
        destroyMenu(mainMenu);
        pthread_t server;
        if (pthread_create(&server, NULL, runServer, param) != 0)
        {
            printf("main error: can't create sender thread \n");
        }
        runClient(param);
        pthread_join(server,NULL);
    }
    free(param);
}
int main()
{
    startMenu();
}


