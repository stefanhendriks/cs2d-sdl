/**
   Camera implementation
**/
#include "global.h"

Camera::Camera() {
 x = 0;
 y = 0;
 width = SCREEN_WIDTH;
 height = SCREEN_HEIGHT;
}

int Camera::getMapDrawX(int mapX) {
    return (getX(mapX * 32));
}

int Camera::getMapDrawY(int mapY) {
   return (getY(mapY * 32));  
}

int Camera::getX(long absX) {
    return (absX-x);        
}

int Camera::getY(long absY) {
    return (absY - y);
}

void Camera::setX(long newX) {
    x = newX;
}

void Camera::setY(long newY) {
    y = newY;   
}

long Camera::getCamX() {
    return x;
}

long Camera::getCamY() {
    return y;
}

// check if absolute coordinates (ie of a player) is in the FOV of this
// camera
bool Camera::isInFOV(long absX, long absY) {
    int rX = getX(absX);
    int rY = getY(absY);
    if ((rX >=0 && rX <= SCREEN_WIDTH) && 
        (rY >=0 && rY <= SCREEN_HEIGHT)) return true;
        
    return false;
}

/**
    Check if a rectangle is still in fov (meaning, all 4 coordinates)
**/
bool Camera::isInFOV(SDL_Rect *rect) {
    return isInFOV(rect->x, rect->y) && isInFOV(rect->x + rect->w, rect->y) &&
           isInFOV(rect->x, rect->y + rect->h) && isInFOV(rect->x + rect->w, rect->y + rect->h);    
}

void Camera::think() {
    // try to see the mouse cursor AND the player in one screen.
    
    // the max X and Y that can be reached is WIDTH / 2 and HEIGHT / 2
    int maxX = SCREEN_WIDTH / 2;
    int maxY = SCREEN_HEIGHT / 2;
    
    long oldX = x;
    long oldY = y;
    
    bool notHappy = true;
    
    int mul = 25;
    bool xIsOk, yIsOk; 
    while (notHappy) {
        xIsOk = true;
        yIsOk = true;
        
        x += ((mouse_x - (SCREEN_WIDTH / 2)) / mul);
        
        /*player.getX()+16, player.getY()+16 */
        if (!isInFOV(player.getRect())) {
            xIsOk = false;  
            x = oldX;
        } 
         

        y += ((mouse_y - (SCREEN_HEIGHT / 2)) / mul);

        if (!isInFOV(player.getRect())) {
            yIsOk = false;  
            y = oldY;
        } 

        if (xIsOk == false && yIsOk == false) {
           mul+=5;

           if (mul > 200)  break; 
        } else {
            
            break;
        }
        
    
    }
    
    //if (x < 0) x = 0;
    //if (y < 0) y = 0;
}
