#include "global.h"

Game::Game() {
   running = true;   
}

int Game::install() {
    // randomize timer
    srand(time(NULL));

    /* Initialize SDL */
    if (SDL_Init (SDL_INIT_VIDEO) < 0) {
        return 1;
    }
    
    atexit (SDL_Quit);

    screen = SDL_SetVideoMode (SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BITDEPTH, SDL_DOUBLEBUF | SDL_SWSURFACE | SDL_FULLSCREEN);
    if (screen == NULL) {
        return 2;
    }
    
    SDL_ShowCursor(0); 
    SDL_WM_SetCaption ("2D Game Engine", NULL);
 
    SDL_EnableKeyRepeat(10, 10);

    loadData();
      
    map.init();
    
    return 0;
}

void Game::loadData() {
     loadtile("data\\tiles\\vila.bmp");         // 0 = grid
     loadtile("data\\backgrounds\\grass2.bmp");  // 1 = background
     // players
     loadtile("data\\player_vip.bmp");
}

void Game::run() {
     while (running) {
           doLogic();
           draw(); 
     }
}

void Game::doLogic() {
    SDL_Event event;
    SDL_GetMouseState(&mouse_x, &mouse_y); 

    cam.think();
    
    /* Check for events */
    while (SDL_PollEvent (&event))
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:
               	switch( event.key.keysym.sym ){
        		// ESC-Key means quit-game. Allways.
        		// TODO: Remove this crap, as this should be defined per state.
        		case SDLK_ESCAPE:
        			running = false;
        			break;
        			
       			case SDLK_z:
        			map.zFocus++;
        			if (map.zFocus >= MAP_DEPTH) map.zFocus = 0;
        			break;
       			case SDLK_w:
                    player.setWalkSpeed(player.getMovespeed());
                     break;
       			case SDLK_s:
                    player.setWalkSpeed(-player.getMovespeed());
                     break;
       			case SDLK_a:
                    player.setStrafeSpeed(-player.getMovespeed());
                     break;
       			case SDLK_d:
                    player.setStrafeSpeed(player.getMovespeed());
                     break;
                }
            break;
        case SDL_KEYUP:
            	switch( event.key.keysym.sym ){
        		// ESC-Key means quit-game. Allways.
        		// TODO: Remove this crap, as this should be defined per state.
       			case SDLK_w:
                    player.setWalkSpeed(0.0f);
                     break;
       			case SDLK_s:
                    player.setWalkSpeed(0.0f);
                     break;
       			case SDLK_a:
                    player.setStrafeSpeed(0.0f);
                     break;
       			case SDLK_d:
                    player.setStrafeSpeed(0.0f);
                     break;
                }
            break;

        case SDL_QUIT:
            running = false;
            break;
        default:
            break;
        }
    }
}

void Game::drawBackground() {
    SDL_Surface *b = tiles[1];
    
    int w = b->w;
    int h = b->h;
    
    int maxW = (SCREEN_WIDTH / w) + 1; 
    int maxH = (SCREEN_HEIGHT / h) + 1; 
    
    for (int x = 0; x < maxW; x++) {
        for (int y = 0; y < maxH; y++) {
            drawSurface(tiles[1], 0, 0, x*w, y*h, SCREEN_WIDTH, SCREEN_HEIGHT, 0, false, 0);
        }    
    }
}

void Game::draw() {
    /* Create a black background */
    //SDL_FillRect (screen, NULL, SDL_MapRGB(screen->format, 0,0,0));
    drawBackground();
       
    player.think();
    
    map.resetVisibility();
    player.markTilesVisible();
    
    map.draw();
    player.draw();
    /* eventually flip for double buffering */  
    SDL_Flip (screen);
}
