//
// Created by Sviatoslav Prylutsky on 2/6/19.
//

#ifndef BOMBERMAN_NETWORK_H
#define BOMBERMAN_NETWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "../../instances/headers/object.h"
#include "../../instances/headers/player.h"
#include "../../instances/headers/map.h"

struct{
    int allowedClientsCount;
}serverConfig;

enum NETWORK_STATUS{
    CONNECTED,
    DISCONNECTED

}network_status;

typedef struct client_s {
    int id;
    Object *player;
    int connected;
    int socket;
    struct sockaddr_in client_address;
}Client;

typedef struct game_info_s {
    int score;
    char map[MAPY][MAPX];
    int notifaction;

} game_info_t;

int startServer(char* port);
void notificateAllClients();
void setCellInServerMap(int y , int x, char ch);
void initPlayerScore(Object* player);
void checkGameOver();

#endif //BOMBERMAN_NETWORK_H