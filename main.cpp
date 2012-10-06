#include "global.h"

/* The screen surface */
SDL_Surface *screen = NULL;
/* tiles database */
SDL_Surface *tiles[MAX_TILES];
/* mouse */
int mouse_x, mouse_y;
/* Map */
Map map;
/* Camera */
Camera cam;
/* Player */
Player player;
/* grid meta */
//GridMeta gridmeta;

int main (int argc, char *argv[])
{
    Game game;
    
    if (game.install() != 0) {
       printf("Error initializing game : %s", SDL_GetError());
       return -1;
    }
    
    game.run();

    return 0;
}
