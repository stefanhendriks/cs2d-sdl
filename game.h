/**
   Game class
**/
#ifndef GAME_H
#define GAME_H

class Game {
  
  public:
         Game();
         
         int install();
         void run();
         
  private:
          void loadData();
          void drawBackground();
          void draw();
          void doLogic();
          bool running;
};

#endif
