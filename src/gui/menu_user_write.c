//
// Created by dy on 05/03/19.
//
#include "headers/menu_gui.h"

char* userWrite(Menu* menu)
{
    SDL_Texture* txt = NULL;
    char* str;
    if ((str = malloc(32 * sizeof(char))) == NULL)
    {
        exit(0);
    }
    int boucle = 0;
    int check = 1;
    int str_size = 0;
    SDL_Event e;
    strcpy(str, "");
    SDL_StartTextInput();

    while (boucle != 1)
    {
        if(SDL_PollEvent(&e))
        {
            switch (e.type)
            {
                case SDL_QUIT: //quitter sdl
                    exit(0);
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            exit(0);
                            break;
                        case SDLK_RETURN: //enter
                            if (str_size > 0)
                            {
                                boucle = 1;
                            }
                            break;
                        case SDLK_BACKSPACE: //supp un char
                            if (str_size)
                            {
                                str[str_size - 1] = '\0';
                                str_size--;
                                SDL_RenderClear(menu->Renderer);
                                showMenu(menu);
                            }
                            break;
                        case SDLK_m :
                            menu->choice = 0;
                            menu->error = 0;
                            boucle = 1;
                            check = 0;
                            break;
                    }
                    break;
                case SDL_TEXTINPUT: //saisi texte
                    if (str)
                    {
                        if (str_size < 31)
                        {
                            strcat(str, e.text.text);
                            str_size += 1;
                        }
                    }
                    break;
            }
        }
        if(str_size && menu->ifIP == 0)
        {
            showText(txt, menu->Renderer, str, 200, 100);
        }
        else if(str_size && menu->ifIP == 1)
        {
            showText(txt, menu->Renderer, str, 200, 170);
        }
        SDL_RenderPresent(menu->Renderer);
    }
    SDL_StopTextInput();
    if (check == 0)
    {
        str = "";
    }
    return str;
}

void destroyMenu(Menu* menu)
{
    if(menu->menuTilset)
    {
        SDL_DestroyTexture(menu->menuTilset);
    }
    if(menu->cursorBomb)
    {
        SDL_DestroyTexture(menu->cursorBomb);
    }
    if(menu->Renderer)
    {
        SDL_DestroyRenderer(menu->Renderer);
    }
    if(menu->Window)
    {
        SDL_DestroyWindow(menu->Window);
    }
    if(menu->musique)
    {
        Mix_FreeMusic(menu->musique);
    }
    if(menu->laser)
    {
        Mix_FreeChunk(menu->laser);
    }
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
    free(menu);
}

int initParam(Menu* menu, ConnectionProps* param)
{
    int result = 0;
    if(menu->choice == 0)
    {
        return 0;
    }
    //choice 1 = rejoindre une game, choice 2 = héberger
    if(menu->choice == 1 && param->ip != NULL)
    {
        param->ip = userWrite(menu);
        if(menu->choice == 1)
        {
            menu->error = 0;
            menu->ifIP = 1;
            param->port = userWrite(menu);
            if(strlen(param->port) == 4)
            {
                result = 1;
            } else if (menu->choice == 1)
            {
                menu->error = 2;
                menu->ifIP = 0;//remet l'ip à 0, oblige l'utilisateur à re taper l'ip.
            }
        } else if (menu->choice == 1)
        {
            menu->error = 1;
        }
    } else if(menu->choice == 2)
    {
        param->ip = "127.0.0.1";
        menu->ifIP = 1; // l'ip existe et est validé
        param->port = userWrite(menu);
        if(strlen(param->port) == 4)
        {
            result = 1;
        } else if (menu->choice == 2)
        {
            menu->error = 2;
        }
    }
    showMenu(menu);
    return result;
}
