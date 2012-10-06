/**
   Map class
**/
#include "global.h"

Map::Map() {
    zFocus = 0;

}

// get specific tile
Tile *Map::getMapTile(int x, int y, int z) {
     return &tiles[x][y][z];
}

// get tile of zFocus
Tile *Map::getMapTile(int x, int y) {
     return &tiles[x][y][zFocus];     
}

void Map::resetVisibility() {
     for (int z = 0; z < MAP_DEPTH; z++) {
         for (int x = 0; x < MAP_WIDTH; x++) {
             for (int y = 0; y < MAP_HEIGHT; y++) {
                 Tile *t = getMapTile(x, y, z);
                 t->setVisible(false);
             }  
         }
     }   
}

/**
    return if tile on X, Y is passable (on zFocus)
**/
bool Map::isPassable(int x, int y) {
      if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
          Tile *t = map.getMapTile(x, y);        
          return t->isPassable();
      }
      return false; // never passable outside borders of map
}

// Make tile on X, Y visible
void Map::makeVisible(int x, int y) {
      if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
          Tile *t = map.getMapTile(x, y);
          t->setVisible(true);
          return;
      }
}

/**
    Return if map tile is in fov.
**/
bool Map::inFOV(int x, int y, int z) {
     int drawX = cam.getMapDrawX(x);
     int drawY = cam.getMapDrawY(y);
     
     int drawXRight = drawX + 32;
     int drawYBottom = drawY + 32;
     
     if (drawXRight < 0) return false;
     if (drawX > SCREEN_WIDTH) return false;
     
     if (drawYBottom < 0) return false;
     if (drawY > SCREEN_HEIGHT) return false;
     
     return true;
}
 
void Map::draw() {
     for (int x = 0; x < MAP_WIDTH; x++) {
         for (int y = 0; y < MAP_HEIGHT; y++) {
             Tile *t = getMapTile(x,y);
             int drawX = cam.getMapDrawX(x);
             int drawY = cam.getMapDrawY(y);
             
             if (!t->drawMe()) {              
              continue;      
             }
             
             if (!t->isVisible()) {
                 if (t->alpha > 128) t->alpha -= FOG_FADE_SPEED;
                // if (t->alpha < 128) continue;                     
             }
             
            if (!inFOV(x, y, zFocus)) continue;

            // if (z != zFocus) continue;
             
             //if (z < zFocus) continue;
             
             if (t->isVisible())  t->alpha += FOG_FADE_SPEED;
           
               
//               if (t->alpha < 255) {
//                    boxRGBA(screen, drawX, drawY, 32, 32, 0, 0, 0, 255);
//               }
               boxRGBA(screen, drawX, drawY, drawX+31, drawY+31, 0, 0, 0, 255); 
               drawtile(0, t->getGridX(), t->getGridY(), drawX, drawY, 0, false, t->alpha);                      
         }    
     }
}

// create some cool map or something
void Map::init() {
     for (int z = 0; z < 2; z++) {
        for (int x = 0; x < 128; x++) {
            for (int y = 0; y < 128; y++) {
                Tile *t = getMapTile(x, y, z);
                if (x > 10 && x < 20) {
                    t->setDraw(true);
                    t->setPassable(true);
                    t->setGridX(getRandomInt(2, 3));
                    t->setGridY(getRandomInt(2, 4));
                }
            }
        }
        for (int x = 0; x < 128; x++) {
            for (int y = 0; y < 128; y++) {
                Tile *t = getMapTile(x, y, z);
                if (y > 10 && y < 20) {
                    t->setDraw(true);
                    t->setPassable(true);
                    t->setGridX(getRandomInt(2, 3));
                    t->setGridY(getRandomInt(2, 4));
                }
            }
        }

    }
    
    setTileGrid(0, 10, 0, 0, 8);
    setTilePassable(0,10, 0, false);

    setTileGrid(1, 10, 0, 0, 8);
    setTilePassable(1,10, 0, false);

    setTileGrid(2, 10, 0, 0, 8);
    setTilePassable(2,10, 0, false);

    setTileGrid(0, 30, 0, 0, 8);
    setTilePassable(0,30, 0, false);

    setTileGrid(1, 30, 0, 0, 8);
    setTilePassable(1,30, 0, false);

    setTileGrid(2, 30, 0, 0, 8);
    setTilePassable(2,30, 0, false);
    

    // crate
    setTileGrid(10, 7, 0, 0, 0);
    setTilePassable(10,7, 0, false);

    setTileGrid(11, 7, 0, 0, 0);
    setTilePassable(11,7, 0, false);

    setTileGrid(10, 8, 0, 0, 0);
    setTilePassable(10,8, 0, false);

    setTileGrid(11, 8, 0, 0, 0);
    setTilePassable(11,8, 0, false);
    
    // crate
    setTileGrid(20, 13, 0, 0, 0);
    setTilePassable(20, 13, 0, false);

    setTileGrid(21, 13, 0, 0, 0);
    setTilePassable(20, 13, 0, false);

    setTileGrid(20, 14, 0, 0, 0);
    setTilePassable(20,14, 0, false);

    setTileGrid(21, 14, 0, 0, 0);
    setTilePassable(20,14, 0, false);
    
    
}

void Map::setTileGrid(int x, int y, int z, int gridX, int gridY) {
     Tile  *t = getMapTile(x, y, z);
     t->setGridX(gridX);
     t->setGridY(gridY);
     t->setDraw(true);
}

void Map::setTilePassable(int x, int y, int z, bool passable) {
    Tile  *t = getMapTile(x, y, z);
     t->setPassable(passable);
}
