#include <time.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <allegro.h>

#define ALFONT_DLL
#include <alfont.h>

// ----


#include "common.h"
#include "player.h"
#include "game.h"
#include "map.h"
#include "steam.h"
#include "hud.h"

extern BITMAP *bmp_screen;

extern cGame game;

// Keep a logbook
void logbook(char *txt) 
{
  FILE *fp;
  fp = fopen("log.txt", "at");
    
  if (fp) 
  {
    fprintf(fp, "%s\n", txt); // print the text into the file
  }

  fclose(fp);
}

int rnd(int max)
{
  if (max < 1) return 0;
  return (rand() % max);
}


void Blur(BITMAP *bmp, int radius) {
    BITMAP *tmp;
    int x, y, i, j;
    int src, dest;
    int r,g,b;
    
    int div = (2*radius+1)*(2*radius+1);
    
    tmp = create_bitmap(bmp->w + 2*radius, bmp->h + 2*radius);
    clear_to_color(tmp, makecol(0,0,0));
    blit(bmp, tmp, 0, 0, radius, radius, bmp->w, bmp->h);
    
    for (y=radius; y<tmp->h-radius; y++) {
        for (x=radius; x<tmp->w-radius; x++) {
            r = g = b = 0;
            for (j=-radius; j<=radius; j++) {
                for (i=-radius; i<=radius; i++) {
                    src = getpixel(tmp, x+i, y+j);
                    r += getr(src);
                    g += getg(src);
                    b += getb(src);
                }
            }
            
            dest = makecol(r/div, g/div, b/div);
            putpixel(bmp, x-radius, y-radius, dest);
        }
    }
    
    destroy_bitmap(tmp);
}

// Return 'allegro degrees'
fixed degrees(int x1, int y1, int x2, int y2)
{
  //float angle    = -1;         // the angle we will return later on


  //fixed delta_x = itofix((x2-x1));
  //fixed delta_y = itofix((y2-y1));
  
  fixed delta_x = (x2-x1);
  fixed delta_y = (y2-y1);
  
  // convert to allegro degrees now
  fixed allegro_degrees;

  allegro_degrees = fixatan2 (delta_y, delta_x);
  int test = fixtoi(allegro_degrees);
  test+=64;
  allegro_degrees = itofix(test);

	return allegro_degrees;
}

// returns in radians
float radians(int x1, int y1, int x2, int y2)
{
  float delta_x = (x2-x1);
  float delta_y = (y2-y1);  
  
  // calculate radians
  float r = (atan2(delta_y, delta_x));
  
  // return in radians
  return r;
}

// returns length between 2 points
double length(double x1, double y1, double x2, double y2)
{
  double A = abs(x2-x1) * abs(x2-x1);
  double B = abs(y2-y1) * abs(y2-y1);  
  return sqrt(A+B); // A2 + B2 = C2 :)
}

// Make shimmer
void Shimmer(int r, int x, int y)
{
  // r = radius
  // X, Y = position

  /* 
    Logic

    Each X and Y position will be taken and will be switched with a randomly choosen neighbouring
    pixel.

    Shimmer effect is done on BMP_SCREEN only.
  */

  int x1,y1;
  int nx,ny;
  int gp=0;     // Get Pixel Result
  int tc=0;

  // go around 360 degrees
  for (int dr=0; dr < r; dr++)
  {
    for (int d=0; d < 360; d++)
    {
       x1 = (x + (cos(d)*(dr)));
       y1 = (y + (sin(d)*(dr)));
  
       if (x1 < 0) x1=0;
       if (y1 < 0) y1=0;
       if (x1 >= game.screen_x) x1 = game.screen_x-1;
       if (y1 >= game.screen_y) y1 = game.screen_y-1;

       gp = _getpixel16(bmp_screen, x1,y1); // use this inline function to speed up things.
       // Now choose random spot to 'switch' with.
       nx=(x1-1) + rnd(2);
       ny=(y1-1) + rnd(2);

       if (nx < 0) nx=0;
       if (ny < 0) ny=0;
       if (nx >= game.screen_x) nx=game.screen_x-1;
       if (ny >= game.screen_y) ny=game.screen_y-1;

       tc = _getpixel16(bmp_screen, nx,ny);

       if (gp > -1 && tc > -1)
       {
        // Now switch colors
        putpixel(bmp_screen, nx, ny, gp);
        putpixel(bmp_screen, x1, y1, tc);
       }

    }
  } 

}

