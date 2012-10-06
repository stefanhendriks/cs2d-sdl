/**
   Tile
**/

#ifndef TILE_H
#define TILE_H

class Tile {
      
      public:
             Tile();
             Tile(int gx, int gy);
             
             void setGridX(int gx);
             void setGridY(int gy);
             
             int getGridX() { return gridx; }
             int getGridY() { return gridy; }
             
             bool drawMe() { return draw; }
             void setDraw(bool d) { draw = d; }
             
             bool isVisible() { return visible; }
             void setVisible(bool d) { visible = d; if (visible) alpha = 255;}
             
             bool isPassable() { return passable; }
             void setPassable(bool p) { passable = p; }
             
             int alpha;
      private:
              int gridx;
              int gridy;
              bool passable;
              bool draw;
              bool visible;
              
};

#endif
