#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "helper.h"

int main(int argc, char *argv[])
{
    int pos_x = 100;
    int pos_y = 100;
    const int res_x = 640;
    const int res_y = 480;

    int scale = 1;

    setup *app = sdl_setup("Movement", pos_x, pos_y, res_x, res_y);

    if (TTF_Init() != 0)
    {
        logSDLError("TTF_Init");
        cleanup("rw", app->renderer, app->window);
        free(app);
        SDL_Quit();
        return -1;
    }

    SDL_Rect container;
    container.x = 20;
    container.y = 20;
    container.w = res_x - 2 * container.x;
    container.h = res_y - 2 * container.y;

    SDL_Rect box;
    box.w =	scale * container.x;
    box.h = scale * container.y;
    box.x = container.x;//res_x / 2 - box.w / 2;
    box.y = res_y / 2 - box.h / 2;

    SDL_Rect goal;
    goal.w = box.w;
    goal.h = box.h;
    goal.x = res_x - container.x;
    goal.y = res_y / 2 - goal.h / 2;

    SDL_Rect winner;
    winner.h = container.h - 20;
    winner.w = container.w - 20;
    winner.x = container.x + 10;
    winner.y = container.y + 10;

    int size = 26;
    SDL_Rect *army	= addEnemy(size);
//    SDL_Rect *army2	= addEnemy(size);

    bool quit = false;
    bool won = false;
    SDL_Event e;

    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
                break;
            }
            if (e.type == SDL_KEYDOWN)
            {
                switch(e.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                case SDLK_UP:
                    box.y -= box.h;
                    break;
                case SDLK_DOWN:
                    box.y += box.h;
                    break;
                case SDLK_LEFT:
                    box.x -= box.w;
                    break;
                case SDLK_RIGHT:
                    box.x += box.w;
                    break;
                default:
                    break;
                }
            }
        }
        if (!won)
        {
            randomMove(army, size, 1);
//        randomMove(army2, size, 3);
            if (contains(&container, &box) || enemyCollision(&box, army, size))// || enemyCollision(&box, army2, size))
            {
                printf("Collision\n");
                if (collision(&goal, &box))
                {
                    won = true;
                    printf("WINNER!\n");
                }
                box.x = container.x;//res_x / 2 - box.w / 2;
                box.y = res_y / 2 - box.h / 2;
            }
            renderHard(app->renderer, &container, &box, &goal, army, NULL, size);
            SDL_Delay(2);
        }
        if (won)
        {
//            SDL_Delay(100);
            SDL_SetRenderDrawColor(app->renderer, 0xff, 0xff, 0xff, 0xff);
            SDL_RenderFillRect(app->renderer, &winner);

            const string f_name = "fonts/Gidole-Regular.ttf";
            SDL_Color f_color = {25, 25, 25, 25};
            SDL_Texture *slogan = renderText(app->renderer, "You have WON the game! BANZAI!!!", f_name, 30, f_color);
            if (slogan == NULL)
            {
                cleanup("rw", app->renderer, app->window);
                free(army);
                free(app);
                TTF_Quit();
                SDL_Quit();
                logSDLError("renderText");
                return -2;
            }
            int slgn_h, slgn_w;
            SDL_QueryTexture(slogan, NULL, NULL, &slgn_w, &slgn_h);

            int x = res_x / 2 - slgn_w / 2;
            int y = res_y / 2 - slgn_h / 2;

            renderTexture(app->renderer, slogan, x, y, NULL);

            SDL_RenderPresent(app->renderer);
        }
    }

    cleanup("rw", app->renderer, app->window);
    free(army);
    free(app);
    SDL_Quit();

    return 0;
}
