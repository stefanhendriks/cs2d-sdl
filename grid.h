/*
    Grid meta-data
    
    The BMP consists out of 32x32 tiles. The grid class holds meta data per
    grid data.
    
    The max size of a grid / texture file is 2048x2048 pixels
*/

#ifndef GRID_H
#define GRID_H

class GridMeta {
  public:
        GridMeta();
        
        void setPassable(int x, int y, bool val);
        void setSlowsdown(int x, int y, float val);
        
  private:
    bool passable[MAX_GRID_WIDTH][MAX_GRID_HEIGHT];
    float slowsdown[MAX_GRID_WIDTH][MAX_GRID_HEIGHT];
    
};

#endif
