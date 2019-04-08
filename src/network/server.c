//
// Created by Sviatoslav Prylutsky on 2/5/19.
//

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
#include "headers/server.h"
#include "../instances/headers/map.h"
#include "../instances/headers/cell.h"
#include "../instances/headers/player.h"

static Client connected_clients[4];
static game_info_t infoGame;
int gameOver = 0;
int canMove = 0;
int minClientsToStart = 2;

Object * getPlayerBySocket(int sock)
{
    for (int i = 0; i < serverConfig.allowedClientsCount; i++)
    {
        if (connected_clients[i].connected == CONNECTED && connected_clients[i].socket == sock)
        {
            return connected_clients[i].player;
        }
    }
    printf("error Player not found \n");
    return NULL;
}
void  notificateAllClients()
{
    for (int i = 0; i < serverConfig.allowedClientsCount; i++)
    {
        if (connected_clients[i].connected == CONNECTED)
        {
            int tmpNotifaction = infoGame.notifaction;
            if(getPlayerBySocket(connected_clients[i].socket)->alive==0)
            {
                infoGame.notifaction = 3;
                write(connected_clients[i].socket,&infoGame, sizeof(infoGame));
                infoGame.notifaction = tmpNotifaction;
            } else
            {
                if(gameOver == 1)
                {
                    infoGame.notifaction = 4;
                }
                initPlayerScore(getPlayerBySocket(connected_clients[i].socket));
                write(connected_clients[i].socket,&infoGame, sizeof(infoGame));
                infoGame.score = 0;
            }
        }
    }
}

void checkGameOver()
{
    int aliveCnt = 0;
    for (int i = 0; i < serverConfig.allowedClientsCount; i++)
    {
        if (connected_clients[i].connected == CONNECTED && getPlayerBySocket(connected_clients[i].socket)->alive==1)
        {
            aliveCnt +=1;

        }
    }
    if(aliveCnt == 1)
    {
        gameOver = 1;
    }
}

void setCellInServerMap(int y , int x, char ch)
{
    infoGame.map[x][y] = ch;
}

void initPlayerScore(Object* player)
{
    infoGame.score = player->score;
}

void initServerConfigs()
{
    serverConfig.allowedClientsCount = 4;
}

void initClients(Client *connected_clients)
{
    for(int i = 0; i < serverConfig.allowedClientsCount;i++)
    {
        connected_clients[i].id = i+1;
        connected_clients[i].socket = -1;
        connected_clients[i].connected = DISCONNECTED;
        switch(i)
        {
            case 0:
                connected_clients[i].player = generateNewObject(11,1,1);
                break;
            case 1:
                connected_clients[i].player = generateNewObject(12,13,13);
                break;
            case 2:
                connected_clients[i].player = generateNewObject(13,1,13);
                break;
            case 3:
                connected_clients[i].player = generateNewObject(14,13,1);
                break;

            default:
                break;
        }
    }
}

int itsNewClient(Client *connected_clients,int new_client)
{
    if (new_client < 0)
    {
        return 0;
    }
    for(int i = 0; i < serverConfig.allowedClientsCount;i++)
    {
        if(connected_clients[i].socket == new_client && connected_clients[i].connected == CONNECTED)
        {
            return 0;
        }
    }
    return 1;
}

int acceptNewClient( Client *connected_clients,int new_client , struct sockaddr_in client_address , int *connected_clients_cnt)
{
    if(*connected_clients_cnt == serverConfig.allowedClientsCount)
    {
        return 0;
    }
    for(int i = 0; i < serverConfig.allowedClientsCount;i++)
    {
        if(connected_clients[i].socket == -1 && connected_clients[i].connected == DISCONNECTED)
        {
            connected_clients[i].socket = new_client;
            connected_clients[i].connected = CONNECTED;
            connected_clients[i].client_address = client_address;
            *connected_clients_cnt+=1;
            return 1;
        }
    }
    return 0;
}

void initListeners(Client *connected_clients,fd_set *file_discriptor , struct timeval waiting_time)
{
    int sockSum = 0;
    for (int i = 0; i < serverConfig.allowedClientsCount; i++)
    {
        if (connected_clients[i].connected == CONNECTED)
        {
            FD_SET(connected_clients[i].socket, file_discriptor);
            sockSum += connected_clients[i].socket;
        }
    }
    select(sockSum + 1, file_discriptor, NULL, NULL, &waiting_time);
}

int read_client(int client)
{
    char buff[1];

    if (client == -1)
    {
        return 1;
    }
    memset(buff, '\n', sizeof(buff));
    if(recv(client, buff, sizeof(buff), 0)<=0)
    {
        printf("client disconected\n");
        return -1;
    }
    if (gameOver == 0 && canMove == 1)
    {
        playerInterfaceController(getPlayerBySocket(client), buff[0]);
        infoGame.notifaction = 0;
    }    
    notificateAllClients();
    memset(buff, '\n', sizeof(buff));
    return (0);
}

void checkMessages(Client *connected_clients,fd_set *file_discriptor, int *connected_clients_cnt)
{
    for (int i = 0; i < serverConfig.allowedClientsCount; i++)
    {
        if (connected_clients[i].connected == CONNECTED)
        {
            if(FD_ISSET(connected_clients[i].socket, file_discriptor))
            {
                if (read_client(connected_clients[i].socket) == -1)
                {
                    if (*connected_clients_cnt != 0)
                    {
                        *connected_clients_cnt -= 1;
                    }
                    printf("Player:(%d)%s:%d disconnected\n", connected_clients[i].id,
                           inet_ntoa(connected_clients[i].client_address.sin_addr),
                           ntohs(connected_clients[i].client_address.sin_port));
                    connected_clients[i].connected = DISCONNECTED;
                    close(connected_clients[i].socket);
                    connected_clients[i].socket = -1;
                }
            }
        }
    }
}

int startServer(char* port)
{
    initServerConfigs();
    int server_socket;// ret;
    struct sockaddr_in serverAddr;
    struct timeval waiting_time;
    int connected_client = 0;
    struct sockaddr_in newAddr;
    socklen_t addr_size;
    fd_set file_discriptor;
    int connected_clients_cnt = 0;
    initMapByObjects();
    initClients(connected_clients);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(server_socket, F_SETFL, O_NONBLOCK);
    if(server_socket < 0)
    {
        perror("socket()");
        exit(1);
    }
    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(port));

    if(bind(server_socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("bind()");
        exit(1);
    }
    if(listen(server_socket, serverConfig.allowedClientsCount) == 0)
    {
        printf("waiting clients....\n");
    } else
    {
        perror("listen()");
    }

    waiting_time.tv_sec = 1;
    waiting_time.tv_usec = 0;
    while(1)
    {
        connected_client = accept(server_socket, (struct sockaddr *) &newAddr, &addr_size);
        FD_ZERO(&file_discriptor);
        if (connected_client >= 0)
        {
            printf("Connection attempt of %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
            if(itsNewClient(connected_clients,connected_client) && acceptNewClient(connected_clients,connected_client, newAddr , &connected_clients_cnt))
            {
                printf("New connection %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
                printf("%d Client connected\n", connected_clients_cnt);
                addObjToCell(getPlayerBySocket(connected_client), getPlayerBySocket(connected_client)->posY, getPlayerBySocket(connected_client)->posX);
                if (connected_clients_cnt < minClientsToStart)
                {
                    infoGame.notifaction = 1;
                } else if(connected_clients_cnt == minClientsToStart)
                {
                    infoGame.notifaction = 2;
                    canMove = 1;

                } else
                {
                    infoGame.notifaction = 0;
                }
                notificateAllClients();
                if(connected_clients_cnt < serverConfig.allowedClientsCount)
                {
                    printf("%d Slot still available\n", serverConfig.allowedClientsCount-connected_clients_cnt);
                } else if(connected_clients_cnt == 4)
                {
                    printf("no more slot available\n");
                }
            } else
            {
                printf("Connection of %s:%d refused! Server is full\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
                write(connected_client, "[!] full server\n", 15);
                close(connected_client);
            }
        }
        initListeners(connected_clients,&file_discriptor , waiting_time);
        checkMessages(connected_clients,&file_discriptor , &connected_clients_cnt);
        connected_client = -1;
    }
    return 0;
}


