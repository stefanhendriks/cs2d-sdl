/**
   Map
**/
#ifndef MAP_H
#define MAP_H

class Map {
 
 public:
        Map();
        
        void draw();
        void init();
        
        void resetVisibility();
        
        Tile *getMapTile(int x, int y, int z); // get specific tile
        Tile *getMapTile(int x, int y);    // get tile of zFocus

        void makeVisible(int x , int y); 
        bool isPassable(int x, int y);
        
        int zFocus;
        
        bool inFOV(int x, int y, int z);
        
        void setTilePassable(int x, int y, int z, bool passable);
        void setTileGrid(int x, int y, int z, int gridX, int gridY);
 private:
        Tile tiles[MAP_WIDTH][MAP_HEIGHT][MAP_DEPTH]; 
};

#endif
