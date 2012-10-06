#include "global.h"

Player::Player() {
      x = 386;
      y = 16;
      walkspeed = 0.0f;
      strafespeed = 0.0f;
      setWalkspeed = 0.0f;
      setStrafespeed = 0.0f;
      moveAbsolute = true;
}

void Player::setWalkSpeed(float speed) {
    setWalkspeed = speed;   
}

void Player::setStrafeSpeed(float speed) {
    setStrafespeed = speed;  
}


void Player::think() {
    think_move();
    think_strafe();
    
    
    if ((x-16) < cam.getCamX()) cam.setX((x-16));
    if ((y-16) < cam.getCamY()) cam.setY((y-16));
    
    if ((x+16) > cam.getCamX() + SCREEN_WIDTH) cam.setX((x+16)- SCREEN_WIDTH);
    if ((y+16) > cam.getCamY() + SCREEN_HEIGHT) cam.setY((y+16)- SCREEN_HEIGHT);
}

void Player::think_strafe() {
    // think about strafe speed
    if (strafespeed < setStrafespeed) strafespeed += 1.0;
    if (strafespeed > setStrafespeed) strafespeed -= 0.5;
   
    long nx, ny;
    // relative movement    
    if (moveAbsolute == false) {
         
        float angle = getMouseRadians();
        int degrees = getDegreesFromRadians(angle);
        
        degrees += 90;   
        double radians = degrees * (PI / 180);
        nx = (getX() + (cos(radians)*(strafespeed)));
        ny = (getY() + (sin(radians)*(strafespeed)));
    
       
    } else {
         nx = (getX() + (PLAYER_MAX_MOVESPEED * (strafespeed)));
         ny = getY();
    }
    
    
     bool yAllowed = moveAllowed(x, ny);
     bool xAllowed = moveAllowed(nx, y);
     if (yAllowed) y = ny;
     if (xAllowed) x = nx;
}

/**
    Think about movement
**/
void Player::think_move() {
    // think about walk speed
    if (walkspeed < setWalkspeed) walkspeed += 1.0;
    if (walkspeed > setWalkspeed) walkspeed -= 0.5;

    long nx, ny;    
    
    // relative movement
    if (moveAbsolute == false) {
     
        
        
        float angle = getMouseRadians();
       
        nx = (getX() + (cos(angle)*(walkspeed)));
        ny = (getY() + (sin(angle)*(walkspeed)));
    
        
    } else {
     nx = getX();
     ny = (getY() - (PLAYER_MAX_MOVESPEED * (walkspeed)));
    }

    bool yAllowed = moveAllowed(x, ny);
    bool xAllowed = moveAllowed(nx, y);
    if (yAllowed) y = ny;
    if (xAllowed) x = nx;
}

// 
bool Player::moveAllowed(long nx, long ny) {
    if ((nx-16) < 0) return false;
    if ((ny-16) < 0) return false;
    
    if ((nx + 16) > MAP_WIDTH * 32) return false;
    if ((ny + 16) > MAP_HEIGHT * 32) return false;
    
    int mapX = nx / 32;
    int mapY = ny / 32;
    
    int nDrawX = cam.getX(nx);
    int nDrawY = cam.getY(ny);
    
    // check for surrounding tiles if they are not passable.
    // if not passable; check if it collides with the coordinates.
    int startX = mapX - 1;
    int startY = mapY - 1;
    int endX   = mapX + 2;
    int endY   = mapY + 2;

    // player abs coordinates    
    SDL_Surface *playersurface = getPlayerSurface();        
    int absX = cam.getX(nx - 16);
    int absY = cam.getY(ny - 16);
    
    boxRGBA(screen, absX, absY, absX+playersurface->w, absY+ playersurface->h, 255, 0, 255, 255);

    SDL_Rect src;
    src.x = absX;
    src.y = absY;
    src.w = 31;
    src.h = 31;

    bool result = true;
    for (int cy = startY; cy < endY; cy++) {
        for (int cx = startX; cx < endX; cx++) {
            if (cx == mapX && cy == mapY) continue; // do not check self
            
            Tile *t = map.getMapTile(cx, cy);
            
            if (t->isPassable()) continue; // may pass
            
            SDL_Rect dest;
            dest.x = cam.getMapDrawX(cx);
            dest.y = cam.getMapDrawY(cy);
            dest.w = 31;
            dest.h = 31;
            int isFree = SDL_CollideBoundingBox(src, dest);
             if (isFree != 0) {
                result = false;
                boxRGBA(screen, cam.getMapDrawX(cx), cam.getMapDrawY(cy), cam.getMapDrawX(cx)+32, cam.getMapDrawY(cy)+32, 255, 0, 255, 255);
                break;
            }
        }    
    }
    
    SDL_FreeSurface(playersurface);
    return result;
}

void Player::markTilesVisible() {
     float angle = getMouseRadians();

     // make own tile visible:
     map.makeVisible(getMapX(), getMapY());        
        
     // then make fov visible

    for (int deg = -50; deg <= 50; deg+=2) {
      
      float newAngle = getIncreasedRad(angle, deg);
      
      // now 'draw' ray and mark the tiles visible
      for (double distance=0; distance < 900; distance+=4)  
      {
          long nx = (getX() + (cos(newAngle)*(distance)));
          long ny = (getY() + (sin(newAngle)*(distance)));
          
          int mapX = nx / 32;
          int mapY = ny / 32;
          
          map.makeVisible(mapX, mapY);
          if (!map.isPassable(mapX, mapY)) break;
      }
      
    }
     
}

// return X - scrollX
long Player::getX() {
    return x;
}

long Player::getY() {
    return y;
}

int Player::getMapX() {
    return (getX() / 32);
}

int Player::getMapY() {
    return (getY() / 32);   
}

void Player::setX(long newX) {
     x = newX;     
}

void Player::setY(long newY) {
     y = newY;     
}

float Player::getDistanceToMouse() {
  float delta_x = (mouse_x-cam.getX(x));
  float delta_y = (mouse_y-cam.getY(y));
  float A = fabs(delta_x) * fabs(delta_x);
  float B = fabs(delta_y) * fabs(delta_y);  
  return sqrt(A+B); // A2 + B2 = C2 :)
}

float Player::getMouseRadians() {
  float delta_x = (mouse_x-cam.getX(x));
  float delta_y = (mouse_y-cam.getY(y));
  return atan2(delta_y, delta_x);      
}

/**
 move speed according to mouse
 
 the close the mouse is, the slower we move.
**/
float Player::getMovespeed() {
    //float dif = (getDistanceToMouse() / MAX_DISTANCE);
    //return dif * 10.0f;
    return 2.0f;
}

SDL_Rect *Player::getRect() {
 SDL_Rect *rect = new SDL_Rect();
 rect->w = 32;
 rect->h = 32;
 rect->x = x-16;
 rect->y = y-16;
 return rect;    
}


SDL_Surface *Player::getPlayerSurface() {
     float angle = getMouseRadians();

     float delta_x = (mouse_x-cam.getX(x));
     float delta_y = (mouse_y-cam.getY(y));
     float wrong_angle = atan2(delta_x, delta_y);      

     // MEMORY USAGE GOES THROUGH THE ROOF BECAUSE OF THIS!
     SDL_Surface *result = rotozoomSurface(tiles[2], getDegreesFromRadians(wrong_angle)-180, 1.0, 1);
     result = SDL_DisplayFormat(result);
     return result;
}

void Player::draw() {
     lineRGBA(screen, cam.getX(x), cam.getY(y), mouse_x, mouse_y, 255, 0, 0, 255);
     
     aalineRGBA(screen, mouse_x-5, mouse_y-5, mouse_x+ 5, mouse_y + 5, 255, 0, 0, 255);
     aalineRGBA(screen, mouse_x-5, mouse_y+5, mouse_x+ 5, mouse_y - 5, 255, 0, 0, 255);
     
     
     // MEMORY USAGE GOES THROUGH THE ROOF BECAUSE OF THIS!
     SDL_Surface *result = getPlayerSurface();
     int absX = cam.getX((x - (result->w/2)));
     int absY = cam.getY((y - (result->h/2)));
     drawSurface(result, 0, 0, absX, absY, 64, 64, SDL_MapRGB(result->format, 0, 0, 0), true, -1);
     SDL_FreeSurface(result);
     
     aalineRGBA(screen, x-5,y-5,x+ 5, y + 5, 255, 0, 0, 255);
     aalineRGBA(screen, x-5, y+5, x+ 5,y - 5, 255, 0, 0, 255);

    // drawSurface(tiles[2], 0, 0, mouse_x-16, mouse_y-16, SDL_MapRGB(tiles[2]->format, 254, 0, 254), true, -1);

/*
    for (int deg = -50; deg <= 50; deg+=2) {
      float newAngle = getIncreasedRad(angle, deg);
      int nx = (mouse_x + (cos(newAngle)*(900)));
      int ny = (mouse_y + (sin(newAngle)*(900)));
    
    //  lineRGBA(screen, x, y, nx, ny, 255, 255, 0, 255);
    }*/
}
