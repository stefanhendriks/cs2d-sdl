/** 
    Engine funcs header
**/
#ifndef ENGINE_FUNC_H
#define ENGINE_FUNC_H
void loadtile(const char *file);
void drawtile(int index, int srcx, int srcy, int x, int y, Uint32 colorKey, bool transparant);
void drawtile(int index, int srcx, int srcy, int x, int y, Uint32 colorKey, bool transparant, int alpha);
void drawtile(int index, int srcx, int srcy, int x, int y, Uint32 colorKey, bool transparant, int alpha, SDL_Surface *destSurface);
void drawtileZoomed(int index, int srcx, int srcy, int x, int y, Uint32 colorKey, bool transparant, int alpha, double zoomFactor);
void drawSurface(SDL_Surface *image, int srcx, int srcy, int x, int y, Uint32 colorKey, bool transparant, int alpha);
void drawSurface(SDL_Surface *image, int srcx, int srcy, int x, int y, int w, int h, Uint32 colorKey, bool transparant, int alpha);
void drawSurface(SDL_Surface *image, int srcx, int srcy, int x, int y, int w, int h, Uint32 colorKey, bool transparant, int alpha, SDL_Surface *destSurface);
int getRandomInt(int min, int max);
int getRandomInt(int max);
SDL_Surface *getTileSurface(SDL_Surface *image, int srcx, int srcy);
SDL_Surface *createSurface(int width, int height);
//void 

#endif
