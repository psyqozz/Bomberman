#include "headers/menu_gui.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define WIDHT_TILE 100
#define HEIGHT_TILE 100
#define CELL_TILE_SIZE 100
#define WIDTH_CELL_CNT = 10
#define HEIGHT_CELL_CNT = 10

Menu* main_menu()
{
    Menu *menu = (Menu*)malloc(sizeof(Menu));
    menu->choice = 0;
    menu->ifIP = 0;
    menu->error = 0;
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    menu->Window = SDL_CreateWindow("Menu",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
    if(menu->Window)
    {
        menu->Renderer = SDL_CreateRenderer(menu->Window, -1, SDL_RENDERER_ACCELERATED);
        if(!menu->Renderer)
        {
            fprintf(stderr, "Impossible de créer le renderer SDL : %s\n", SDL_GetError());
            destroyMenu(menu);
            exit(1);
        }
    } else
    {
        fprintf(stderr, "Impossible de créer le fenetre SDL : %s\n", SDL_GetError());
        destroyMenu(menu);
        exit(1);
    }
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    menu->musique = Mix_LoadMUS("./images/BGSound.mp3");
    menu->laser = Mix_LoadWAV("./images/LaserEffect.wav");
    menu->MenuSize.x = 0;
    menu->MenuSize.y = 0;
    menu->MenuSize.w = SCREEN_WIDTH;
    menu->MenuSize.h = SCREEN_HEIGHT;

    menu->cursor.x = 200;
    menu->cursor.y = 100;
    menu->cursor.w = 45;
    menu->cursor.h = 45;

    SDL_Surface *mainImg = IMG_Load("./images/main_menu.bmp");
    SDL_Surface* surface_cursor = IMG_Load("./images/bombe.png");
    if(!mainImg || !surface_cursor)
    {
        fprintf(stderr, "Fail of load *tillset.bmp : %s\n", SDL_GetError());
        destroyMenu(menu);
    }

    menu->menuTilset = SDL_CreateTextureFromSurface(menu->Renderer,mainImg);
    menu->cursorBomb = SDL_CreateTextureFromSurface(menu->Renderer, surface_cursor);
    return menu;
}

void showMenu(Menu* menu) {
    SDL_Texture *txt = NULL;

    SDL_RenderClear(menu->Renderer);
    SDL_RenderCopy(menu->Renderer, menu->menuTilset, NULL, &menu->MenuSize);
    showText(txt, menu->Renderer, "Welcome to the Bomberman", 200, 30);
    showText(txt, menu->Renderer, "Press m to return to the main menu", 200, 300);
    showText(txt, menu->Renderer, "escape to quit", 10, 0);
    if (menu->choice == 0) // le menu principal
    {
        showText(txt, menu->Renderer, "Join a game", 40, 100);
        showText(txt, menu->Renderer, "Host a game", 40, 170);
        SDL_RenderCopy(menu->Renderer, menu->cursorBomb, NULL, &menu->cursor);
    } else if (menu->choice == 1)// rejoindre une game
    {
        showText(txt, menu->Renderer, "IP address : ", 40, 100);
        showText(txt, menu->Renderer, "Port : ", 40, 170);
        if (menu->ifIP == 1) { // correctif d'un affichage bug de l'ip
            showText(txt, menu->Renderer, "127.0.0.1", 200, 100);
        }
    } else {
        showText(txt, menu->Renderer, "Port : ", 40, 170);
    }
    //gestion erreur
    if (menu->error == 1)
    {
        showText(txt, menu->Renderer, "Mauvais IP", 0, 350);
    } else if (menu->error == 2)
    {
        showText(txt, menu->Renderer, "Mauvais Port", 0, 350);
    }
    SDL_RenderPresent(menu->Renderer);
}

void showText(SDL_Texture* txt, SDL_Renderer* Renderer, char* mess, int x, int y)
{
    TTF_Init();
    SDL_Color normalColor = {255, 255, 255, 255};
    int font_size = 24;
    SDL_Surface* surface_text;
    SDL_Rect position;

    TTF_Font* police = TTF_OpenFont("./images/OpenSans-Regular.ttf", font_size);
    surface_text = TTF_RenderText_Solid(police, mess, normalColor);
    if(!surface_text)
    {
        fprintf(stdout,"Échec de chargement du texte (%s)\n",SDL_GetError());
        gameDestroy();
    }
    TTF_CloseFont(police);

    txt = SDL_CreateTextureFromSurface(Renderer, surface_text);
    if(!txt)
    {
        fprintf(stdout,"Échec de création de la texture texte - %s\n",SDL_GetError());
        gameDestroy();
    }

    int txtWidth, txtHeigth;
    // Récupère la longueur et hauteur de la texture
    SDL_QueryTexture(txt, NULL, NULL, &txtWidth, &txtHeigth);

    position.x = x;
    position.y = y;
    position.w = txtWidth;
    position.h = txtHeigth;

    SDL_FreeSurface(surface_text);
    SDL_RenderCopy(Renderer, txt, NULL, &position);
}

ConnectionProps* choiceMode(Menu* menu)
{
    ConnectionProps* param = (ConnectionProps*)malloc(sizeof(ConnectionProps));
    int result = 0;
    SDL_Event e;
    showMenu(menu);
    Mix_PlayMusic(menu->musique, -1);
    while(result != 1)
    {
        if(SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                exit(0);
            } else if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                    case SDLK_ESCAPE :
                        exit(0);
                        break;
                    case SDLK_RETURN :
                        Mix_PlayChannel(-1, menu->laser, 0);
                        if (menu->cursor.y == 100)
                        {
                            menu->choice = 1;
                            menu->ifIP = 0;
                        } else if (menu->cursor.y == 170)
                        {
                            menu->choice = 2;
                            menu->ifIP = 1;
                        }
                        break;
                    case SDLK_UP :
                        menu->cursor.y = 100;
                        break;
                    case SDLK_DOWN :
                        menu->cursor.y = 170;
                        break;
                }
            }
        }
        SDL_Delay(20);
        showMenu(menu);
        if(menu->choice != 0)
        {
            result = initParam(menu, param);
        }
    }
    return param;
}

