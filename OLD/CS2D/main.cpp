/*

  Counter-Strike 2D

  */

#include <time.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <allegro.h>

#define ALFONT_DLL
#include <alfont.h>

// ---
#include "common.h"
#include "player.h"
#include "game.h"
#include "map.h"
#include "steam.h"
#include "hud.h"


// graphic data file headers
#include "graphics.h"


// Timers
volatile int RUNNING_TIMER_tenth = 0;
volatile int RUNNING_TIMER_fps = 0;

// Timers
int frame_count, fps;  // fps and such 

// Variables
BITMAP *bmp_screen;
BITMAP *bmp_collide;

// Datafile
DATAFILE *graphics;
DATAFILE *shadows;
DATAFILE *hud;

// classes
cGame game;
cSteam steam;
cMap map;

// FONT stuff
ALFONT_FONT *game_font;
ALFONT_FONT *cs_font;

/***
  Timer    : This timer is called very 0.1 second
  Purpose  : Activates think functions
  ***/
void timer_tenth()
{    
  RUNNING_TIMER_tenth++;
}

END_OF_FUNCTION(timer_tenth);


/***
  Timer    : This timer is called very second
  Purpose  : timer callback for measuring the frames per second
  ***/
void fps_proc(void)
{  
  RUNNING_TIMER_fps++;
  game.iTheTime++;
}
END_OF_FUNCTION(fps_proc);

/* Processes timer functions */
void TIME_process()
{
	// fix any super high timers
	if (RUNNING_TIMER_fps > 10)
	{
		logbook("Exeptional high RUNNING_TIMER_FPS, fixed at max 10");
		RUNNING_TIMER_fps = 10;
	}

	if (RUNNING_TIMER_tenth > 40)
	{
		logbook("Exeptional high RUNNING_TIMER_TENTH, fixed at max 40");
		RUNNING_TIMER_tenth = 40;
	}

	  // keep up with time cycles
  while (RUNNING_TIMER_fps > 0)
  {
 
	  map.think();

   fps = frame_count;
   frame_count = 0;     
   RUNNING_TIMER_fps--; // done!
  }

 
  // keep up with time cycles
  while (RUNNING_TIMER_tenth > 0)
  {
	  // All particles die slowly!
	  map.think_particles();
	  map.think_clouds();

	  // Players think
	  for (int i=0; i < MAX_PLAYERS; i++)
		 game.players[i].think();
		  

	  // Steam powered fading
	  game.iSteamFade++;
	  if (game.iSteamFade > 511)
		  game.iSteamFade=0;

	  if (game.iScrollSpeed > 0)
		  game.iScrollSpeed--;

	  // Scroll speeds up
	  if (mouse_x >= 639)
		  game.iScrollSpeed+=2;
	  
	  if (mouse_x <= 1)		
		  game.iScrollSpeed+=2;
	  
	  // Y
	  if (mouse_y >= 479)
		  game.iScrollSpeed+=2;

	  if (mouse_y <= 1)
		  game.iScrollSpeed+=2;

	  if (game.iScrollSpeed > 16)
		  game.iScrollSpeed = 16;

    RUNNING_TIMER_tenth--;
  }
}

/***
  Function : install()
  Returns  : TRUE on succes, FALSE on failure
  Purpose  : Will install game variables, sets up the entire game.
***/
bool install()
{ 
  // Each time we run the game, we clear out the logbook
  FILE *fp;
  fp = fopen("log.txt", "wt");

  if (fp) 
   {
     fprintf(fp, "Counter-Strike 2D Logbook\n");
     fprintf(fp, "-------------------------\n\n"); // print head of logbook
     fclose(fp);
   }

  logbook("--------------");
  logbook("Initialization");
  logbook("--------------");

  // init game
  map.init();
  game.init();
  steam.init();  

  logbook("Creating entity types.");
  game.install_entities();
  
  // Logbook notification
  logbook("\n-------");
  logbook("Allegro");
  logbook("-------");


  // ALLEGRO - INIT    
  if (allegro_init() != 0)
    return false;

  logbook(allegro_id);
  yield_timeslice();
  logbook("yield_timeslice()");

  int r = install_timer();
  if (r > -1) logbook("install_timer()");
  else
  {
	logbook("FAILED");
	return false;
  }	

  alfont_init();
  logbook("alfont_init()");
  install_keyboard();
  logbook("install_keyboard()");
  install_mouse();
  logbook("install_mouse()");

  logbook("setting up timer functions / locking functions & memory");
  /* set up the interrupt routines... */   
  LOCK_VARIABLE(RUNNING_TIMER_tenth);  
  LOCK_VARIABLE(RUNNING_TIMER_fps);    
  LOCK_FUNCTION(timer_tenth);  
  LOCK_FUNCTION(fps_proc);  
    
  install_int(timer_tenth, 10);
  install_int(fps_proc, 1000);
  
  logbook("Timers installed");

  frame_count = fps = 0;


  // set window title
  char title[80];
  sprintf(title, "Counter-Strike 2D");
  
  // Set window title
  set_window_title(title);  
  char window_title[256];
  sprintf(window_title, "Window title set: [%s]", title);
  logbook(window_title);
  
  set_color_depth(16);      // run in 16 bit mode
  if (game.windowed)
  {
    int r = set_gfx_mode(GFX_AUTODETECT_WINDOWED, game.screen_x, game.screen_y, game.screen_x, game.screen_y);
    if (r > -1)
    {
      // Succes      
    }
    else
    {      
      // GFX_DIRECTX_ACCEL / GFX_AUTODETECT
      r = set_gfx_mode(GFX_DIRECTX_ACCEL, game.screen_x, game.screen_y, game.screen_x, game.screen_y);
      if (r > -1)
      {  
        game.windowed = false;
        logbook("Could not enter windowed-mode; settings.d3 adjusted");
      }
      else
      {        
		logbook("ERROR - !");
        return false;
      }
    }
  }
  else
  {
    int r = set_gfx_mode(GFX_AUTODETECT, game.screen_x, game.screen_y, game.screen_x, game.screen_y);

    // succes
    if (r > -1)
    {
      
    }
    else
    {
		logbook("ERROR - !!");
      return false;
    }

  }
    
  
  text_mode(0);

  logbook("Loading font data");
  // loading font

  game_font = alfont_load_font("gfx\\font\\tahoma.ttf");  

  if (game_font != NULL)
  {
    alfont_set_font_size(game_font, 20); // set size
  }
  else
    allegro_message("Error loading tahoma.ttf!");
  
  // CS Font
  cs_font = alfont_load_font("gfx\\font\\cs.ttf");  

  if (cs_font != NULL)
  {
    alfont_set_font_size(cs_font, 20); // set size
  }
  else
    allegro_message("Error loading cs.ttf");

  alfont_text_mode(-1);
  
  if (set_display_switch_mode(SWITCH_BACKGROUND) < 0)
  {
      set_display_switch_mode(SWITCH_PAUSE);
      logbook("Display 'switch and pause' mode set");
  }
  else
      logbook("Display 'switch to background' mode set");

  // sound
  logbook("Initializing sound");
  int s = install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
  
  /*** 
   Bitmap Creation
   ***/
  
  bmp_screen = create_bitmap(game.screen_x, game.screen_y);
  
  if (bmp_screen == NULL)
  {
    logbook("ERROR: Could not create bitmap: bmp_screen");
    return false;
  }
  else
    logbook("Bitmap created: bmp_screen");

  bmp_collide = create_bitmap(game.screen_x, game.screen_y);
  
  if (bmp_collide == NULL)
  {
    logbook("ERROR: Could not create bitmap: bmp_collide");
    return false;
  }
  else
    logbook("Bitmap created: bmp_collide");
 
  /*** End of Bitmap Creation ***/

  // load datafiles
  graphics = load_datafile("graphics.dat");
  if (graphics == NULL)  
  {
    logbook("ERROR: Could not load datafile: graphics.dat");
    return false;
  }
  else
    logbook("Datafile loaded: graphics.dat");

  // Shadows
  shadows = load_datafile("shadows.dat");
  if (shadows == NULL)  
  {
    logbook("ERROR: Could not load datafile: shadows.dat");
    return false;
  }
  else
    logbook("Datafile loaded: shadows.dat");

  // HUD
  hud = load_datafile("hud.dat");
  if (hud == NULL)  
  {
    logbook("ERROR: Could not load datafile: hud.dat");
    return false;
  }
  else
    logbook("Datafile loaded: hud.dat");

  //set_color_conversion(COLORCONV_NONE);  
  set_color_conversion(COLORCONV_MOST);
  logbook("Color conversion method set");
  
  // setup mouse speed
  set_mouse_speed(2,2);

  logbook("Mouse speed set");

  logbook("");
  logbook("----");
  logbook("GAME ");
  logbook("----");

  game.LOAD_TexturesFromDataFile("data//de_aztec.dat");
//  DATA_Init();

  // randomize timer
  srand( (unsigned)time( NULL ) );
  
  //srand(time(NULL));
  
  // normal sounds are loud, the music is lower (its background music, so it should not be disturbing)
  set_volume(255, 200);

  set_trans_blender(128, 128, 128, 128);

 logbook("");
 logbook("--------------");
 logbook("BATTLE CONTROL");
 logbook("--------------"); 
 logbook("\n3...2...1... GO!\n");
 return true;
}


int main()
{
	if (install())
		game.run();
	
	alfont_exit();
	logbook("Allegro FONT library shut down.");
	
	// Now we are all neatly closed, we exit Allegro and return to OS hell.
	allegro_exit();
	logbook("Allegro shut down.");
	logbook("\nThanks for playing!");
	return 0;
}
END_OF_MAIN();