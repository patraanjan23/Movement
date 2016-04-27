#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "helper.h"

int main(int argc, char *argv[])
{
	int pos_x = 100;
	int pos_y = 100;
	const int res_x = 640;
	const int res_y = 480;

	int scale = 1;

	setup *app = sdl_setup("Movement", pos_x, pos_y, res_x, res_y);

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

	int size = 26;
	SDL_Rect *army	= addEnemy(size);
	SDL_Rect *army2	= addEnemy(size);

	bool quit = false;
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
		randomMove(army, size, 2);
		randomMove(army2, size, 3);
		if (contains(&container, &box) || enemyCollision(&box, army, size) || enemyCollision(&box, army2, size))
		{
			printf("Collison\n");
			if (collision(&goal, &box))
			{
				printf("WINNER!\n");
			}
			box.x = container.x;//res_x / 2 - box.w / 2;
			box.y = res_y / 2 - box.h / 2;
		}
		render(app->renderer, &container, &box, &goal, army, army2, size);
		SDL_Delay(2);
	}

	cleanup("rw", app->renderer, app->window);
	free(army);
	free(app);
	SDL_Quit();

	return 0;
}
