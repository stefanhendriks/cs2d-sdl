/**

Player

**/

#ifndef PLAYER_H
#define PLAYER_H

class Player {
  
  public:
         Player();

         void draw();
         void markTilesVisible();
         float getMouseRadians();
         float getDistanceToMouse();
         float getMovespeed();
         
         void setX(long newX); 
         void setY(long newY);
         
         long getX();
         long getY();
         
         int getMapX();          // get map coordinates out of the X and Y
         int getMapY();
         
         SDL_Rect *getRect();
 
         // walk
         void setWalkSpeed(float speed);
         void setStrafeSpeed(float speed);
 
         void think();
  private:
        
        void think_move();
        void think_strafe();
        bool moveAllowed(long nx, long ny);
        SDL_Surface *getPlayerSurface();
        // X and Y are the positions on the map, so the max X (or Y) will be
        // MAP_TILES_WIDTH * TILE_SIZE 
          long x, y;
          
          // the actual speed we walk
          float walkspeed;      // walking forward, backwards
          float strafespeed;    // strafing left or right
          
          // the speed we should walk
          float setWalkspeed;
          float setStrafespeed;
          
          bool moveAbsolute;
};


#endif
