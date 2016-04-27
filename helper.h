/*	personal helper file comtaining
*	typedefs and convinient stuff
*/

typedef enum {false, true} bool;
typedef char* string;

/*
*	a structure with following member types:
*	* SDL_Window *
*	* SDL_Renderer *
*/
typedef struct _sdl_init
{
    SDL_Window *window;
    SDL_Renderer *renderer;
} setup;

//typedef SDL_Rect* enemy;

// fn related to Linked list
SDL_Rect* addEnemy(int n);
void randomMove(SDL_Rect *enemy, int n, int speed);
bool enemyCollision(SDL_Rect *player, SDL_Rect *enemy, int n);


// fn related to main game
void	logSDLError(char *msg);
void	cleanup(char *type, ...);
setup*	sdl_setup(char *title, int posX, int posY, int resX, int resY);
bool	contains(SDL_Rect *bound, SDL_Rect *obj);
void	render(SDL_Renderer *ren, SDL_Rect *container, SDL_Rect *box, SDL_Rect *g, SDL_Rect *enemy, SDL_Rect *enemy2, int n);
bool	collision(SDL_Rect *box1, SDL_Rect *box2);
