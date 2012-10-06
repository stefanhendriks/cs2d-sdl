#include "global.h"

float getDegreesFromRadians(float angle) {
  return angle * (180 / PI);      
}

float getIncreasedRad(float angle, int increaseDegrees) {
  const float DEG2RAD = 3.14159/180;
  float inDegrees = getDegreesFromRadians(angle);
  float newAngle = inDegrees + increaseDegrees;
  return newAngle *= DEG2RAD;
}


/**

Returns radians from middle of screen to mouse **/
/*
float getMouseRadians() {
  float delta_x = (mouse_x-(SCREEN_WIDTH/2));
  float delta_y = (mouse_y-(SCREEN_HEIGHT/2));
  return atan2(delta_y, delta_x);      
}*/
