#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "helper.h"

/*
*	This function does the following:
*	* Initialize SDL with error checking
*	* Create an SDL_Window with error checking
*	* Create an SDL_Renderer checking
*
*	* Returns a 'setup' struct *
*	Example use: setup *app = sdl_setup("App", 100, 100, 640, 480);
*/
setup* sdl_setup(char *title, int posX, int posY, int resX, int resY)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        logSDLError("SDL_Init");
        return NULL;
    }

    SDL_Window *window = SDL_CreateWindow(title, posX, posY, resX, resY, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        logSDLError("SDL_CreateWindow");
        SDL_Quit();
        return NULL;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        logSDLError("SDL_CreateRenderer");
        cleanup("w", window);
        SDL_Quit();
        return NULL;
    }

    setup *tmp = (setup*)malloc(sizeof(setup));
    tmp->window = window;
    tmp->renderer = renderer;
    return tmp;
}

void renderHard(SDL_Renderer *ren, SDL_Rect *container, SDL_Rect *box, SDL_Rect *g, SDL_Rect *enemy, SDL_Rect *enemy2, int n)
{
    SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren, 0, 0, 0x11, 0xff);
    SDL_RenderFillRect(ren, container);
    SDL_SetRenderDrawColor(ren, 0, 0xff, 0, 0xff);
    SDL_RenderFillRect(ren, box);
    SDL_SetRenderDrawColor(ren, 0, 0, 0xff, 0xff);
    for (int i = 0; i < n; i++)
    {
        SDL_RenderFillRect(ren, enemy + i);
    }
    if (enemy2 != NULL)
    {
//        SDL_SetRenderDrawColor(ren, 0, 0xaa, 0xaa, 0xff);
        for (int i = 0; i < n; i++)
        {
            SDL_RenderFillRect(ren, enemy2 + i);
        }
    }
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0xff);
    SDL_RenderFillRect(ren, g);
    SDL_SetRenderDrawColor(ren, 0x99, 0, 0, 0xff);
    SDL_RenderPresent(ren);
}

SDL_Texture* renderText(SDL_Renderer *ren, const string msg, const string f_name, unsigned int f_size, SDL_Color color)
{
    TTF_Font *font = TTF_OpenFont(f_name, f_size);
    if (font == NULL)
    {
        logSDLError("TTF_OpenFont");
        return NULL;
    }
    SDL_Surface *tmp = TTF_RenderText_Blended(font, msg, color);
    if (tmp == NULL)
    {
        logSDLError("TTF_RenderText_Blended");
        TTF_CloseFont(font);
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, tmp);
    if (texture == NULL)
    {
        logSDLError("SDL_CreateTextureFromSurface");
        TTF_CloseFont(font);
        SDL_FreeSurface(tmp);
        return NULL;
    }
    TTF_CloseFont(font);
    SDL_FreeSurface(tmp);
    return texture;
}

void renderTexture(SDL_Renderer *ren, SDL_Texture *tex, int x, int y, SDL_Rect *clip)
{
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    if (clip != NULL)
    {
        dest.h = clip->h;
        dest.w = clip->w;
    }
    else
    {
        SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    }
    SDL_RenderCopy(ren, tex, clip, &dest);
}

/*
 *	Detects collision between 2 SDL_Rect objects
 */
bool contains(SDL_Rect *bound, SDL_Rect *obj)
{
    int objTop		= obj->y;
    int objBottom	= obj->y + obj->h;
    int objLeft		= obj->x;
    int objRight	= obj->x + obj->w;

    int boundTop	= bound->y;
    int boundBottom	= bound->y + bound->h;
    int boundLeft	= bound->x;
    int boundRight	= bound->x + bound->w;

    if (objLeft < boundLeft || objTop < boundTop ||
            objRight > boundRight || objBottom > boundBottom)
    {
        return true;
    }
    return false;
}

bool collision(SDL_Rect *box1, SDL_Rect *box2)
{
    int b1Top		= box1->y;
    int b1Bottom	= box1->y + box1->h;
    int b1Left		= box1->x;
    int b1Right		= box1->x + box1->w;

    int b2Top		= box2->y;
    int b2Bottom	= box2->y + box2->h;
    int b2Left		= box2->x;
    int b2Right		= box2->x + box2->w;

    if (b1Left >= b2Right)
        return false;
    if (b1Right <= b2Left)
        return false;
    if (b1Top >= b2Bottom)
        return false;
    if (b1Bottom <= b2Top)
        return false;
    return true;
}

bool enemyCollision(SDL_Rect *player, SDL_Rect *enemy, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (collision(enemy + i, player))
        {
            return true;
        }
    }
    return false;
}
/*
*	A function that prints formatted Error msg with SDL_GetError.
*	Argument is an additional msg to be shown with SDL_GetError.
*	Example use: log("SDL_CreateWindow");
*/
void logSDLError(char *msg)
{
    printf("Error: %s: %s\n", msg, SDL_GetError());
}


/*
*	A variable argument function that takes in SDL_CreateXXX objects
*	and destroys them.
*	Type of object: 't' = SDL_Texture *;
*					'r' = SDL_Renderer *;
*					'w' = SDL_Window *;
*	Example use: cleanup("ttrw", background, foreground, renderer, window);
*/
void cleanup(char *type, ...)
{
    va_list objects;
    va_start(objects, type);

    while (*type != '\0')
    {
        if (*type == 't')
        {
            SDL_DestroyTexture(va_arg(objects, SDL_Texture *));
        }
        else if (*type == 'r')
        {
            SDL_DestroyRenderer(va_arg(objects, SDL_Renderer *));
        }
        else if (*type == 'w')
        {
            SDL_DestroyWindow(va_arg(objects, SDL_Window *));
        }
        type++;
    }
    va_end(objects);
}

// fn related to linked list
SDL_Rect* addEnemy(int n)
{
    SDL_Rect *tmp = (SDL_Rect*)malloc(n * sizeof(SDL_Rect));
    for (int i = 0; i < n; i++)
    {
        tmp[i].x = 60 + i*20;
        tmp[i].y = rand() % 440;
        tmp[i].w = 20;
        tmp[i].h = 20;
        printf("Success! %d\n", i+1);
    }
    return tmp;
}

void randomMove(SDL_Rect *enemy, int n, int speed)
{
    /*	int speed = 4;*/
    for (int i = 0; i < n; i++)
    {
        if (i % 2 == 0)
        {
            enemy[i].y += (i + 1) % 3 * speed; //(i + 1) / speed + (i + 1) % speed;
            if (enemy[i].y + enemy[i].h > 460)
            {
                enemy[i].y = 20;
            }
        }
        else
        {
            enemy[i].y -= (i + 1) % 3 * speed; //(i + 1) / speed + (i + 1) % speed;
            if (enemy[i].y <= 20)
            {
                enemy[i].y = 460 - enemy[i].h;
            }
        }
    }
}
