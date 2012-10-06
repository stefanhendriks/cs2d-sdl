/**
   Camera
   
   A 2D camera is a 'viewport' on a 2D map.
   
   The camera gives the eventuall correct X and Y coordinates for everything on
   the screen. The Camera has an X and Y position.
   

*/
#ifndef CAMERA_H
#define CAMERA_H

class Camera {
  
  public:
         Camera();
         
         // translate map coordinates into coordinates to draw them on the screen.
         int getMapDrawX(int mapX);
         int getMapDrawY(int mapY);
         
         // transform absolute (pixeled) coordinates into draw coordinates.
         int getX(long absX);
         int getY(long absY);
         
         // set X and Y for camera
         void setX(long x);
         void setY(long y);
         
         // get cameray x and y
         long getCamX();
         long getCamY();

         void think();         
         bool isInFOV(long absX, long absY);
         bool isInFOV(SDL_Rect *rect);
         
  private:
         long x, y;
         int width, height;
};


#endif
