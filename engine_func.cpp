/**
   Global functionality for loading stuff, etc.
**/
#include "global.h"

void drawtile(int index, int srcx, int srcy, int x, int y, Uint32 colorKey, bool transparant) {
     drawtile(index, srcx, srcy, x, y, colorKey, transparant, -1);
}

/**
   Draw a tile (a piece of 32x32) from a tilegrid, onto the screen at X, Y
   
   Source X and Source Y will be already placed in grid, so you do not have
   to worry about pixels. (just give the 32x32 tile coordinates)
**/
void drawtile(int index, int srcx, int srcy, int x, int y, Uint32 colorKey, bool transparant, int alpha) {
    drawSurface(tiles[index], srcx*32,  srcy*32, x, y, colorKey, transparant, alpha);
}

void drawtile(int index, int srcx, int srcy, int x, int y, Uint32 colorKey, bool transparant, int alpha, SDL_Surface *destSurface) {
    drawSurface(tiles[index], srcx*32,  srcy*32, x, y, 32, 32, colorKey, transparant, alpha, destSurface);
}

void drawSurface(SDL_Surface *image, int srcx, int srcy, int x, int y, Uint32 colorKey, bool transparant, int alpha) {
    drawSurface(image, srcx,  srcy, x, y, 32, 32, colorKey, transparant, alpha, screen);
}

SDL_Surface *getTileSurface(SDL_Surface *image, int srcx, int srcy) {
    SDL_Surface *surf = createSurface(32, 32);
    drawtile(0, srcx, srcy, 0, 0, 0, false, 0, surf);
    return surf;
}

SDL_Surface *createSurface(int width, int height) {
   /* Create a 32-bit surface with the bytes of each pixel in R,G,B,A order,
       as expected by OpenGL for textures */
    SDL_Surface *surface;
    Uint32 rmask, gmask, bmask, amask;

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, SCREEN_BITDEPTH,
                                   rmask, gmask, bmask, amask);
                                   
    return surface;
}

void drawtileZoomed(int index, int srcx, int srcy, int x, int y, Uint32 colorKey, bool transparant, int alpha, double zoomFactor) {
     SDL_Surface *image = zoomSurface(tiles[index], zoomFactor, zoomFactor, 1);
     drawSurface(image, srcx, srcy, x, y, colorKey, transparant, alpha);
     SDL_FreeSurface(image);
}

void drawSurface(SDL_Surface *image, int srcx, int srcy, int x, int y, int w, int h, Uint32 colorKey, bool transparant, int alpha) {
    drawSurface(image, srcx, srcy, x, y, w, h, colorKey, transparant, alpha, screen);
}

void drawSurface(SDL_Surface *image, int srcx, int srcy, int x, int y, int w, int h, Uint32 colorKey, bool transparant, int alpha, SDL_Surface *destSurface) {
    SDL_Rect destrect;
    destrect.x = x;
    destrect.y = y;
    
    SDL_Rect srcrect;
    srcrect.x = srcx;
    srcrect.y = srcy;
    srcrect.w = w;
    srcrect.h = h;
    
    /*
     * Palettized screen modes will have a default palette (a standard
     * 8*8*4 colour cube), but if the image is palettized as well we can
     * use that palette for a nicer colour matching
     */
    if (image->format->palette && screen->format->palette) {
        SDL_SetColors(screen, image->format->palette->colors, 0,
                      image->format->palette->ncolors);
    }
    

    
    if (transparant) {
        SDL_SetColorKey(image, SDL_SRCCOLORKEY, colorKey);
    }

    if (alpha > 0 && alpha < 256 && !transparant) {
        SDL_SetAlpha(image, SDL_SRCALPHA, alpha);
    }

    /* Blit onto the screen surface */
    if(SDL_BlitSurface(image, &srcrect, destSurface, &destrect) < 0)
        fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());

    if (alpha > 0 && alpha < 256 && !transparant) {
        SDL_SetAlpha(image, SDL_SRCALPHA, SDL_ALPHA_OPAQUE);
    } else {
        SDL_SetColorKey(image, 0, 0);
    }
}



// load a tile into the TILES stuff thingy
void loadtile(const char *file) {
     static int TILE_INDEX = 0;
     if (TILE_INDEX < MAX_TILES) {
     SDL_Surface *temp = IMG_Load(file);
         tiles[TILE_INDEX] = SDL_DisplayFormat(temp);
         TILE_INDEX++;
     } else {
          printf("Could not load file %s, tile index out of range", file);   
     }
}

int getRandomInt(int min, int max) {
 return ((rand() % (max-min)) + min);
}

int getRandomInt(int max) {
	getRandomInt(0, max);
}
