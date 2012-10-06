/**
   Tile implementation
**/
#include "global.h"

Tile::Tile() {
   draw = false;
   visible = false;
   passable = false;
   alpha = 255;
}

Tile::Tile(int gx, int gy) {
      gridx=gx;
      gridy=gy;       
}

void Tile::setGridX(int gx) {
     gridx = gx;     
}

void Tile::setGridY(int gy) {
     gridy = gy;
}
