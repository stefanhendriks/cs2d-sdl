#include "global.h"

GridMeta::GridMeta() {
    for (int x = 0; x < MAX_GRID_WIDTH; x++) {
        for (int y = 0; y < MAX_GRID_HEIGHT; y++) {
            setPassable(x, y, true);
            setSlowsdown(x, y, 0.5f);
        }   
    }
}

void GridMeta::setSlowsdown(int x, int y, float val) {
    slowsdown[x][y] = val;
}

void GridMeta::setPassable(int x, int y, bool val) {
  passable[x][y] = val;   
}
