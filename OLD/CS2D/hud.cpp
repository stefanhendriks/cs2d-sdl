#include <time.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h> 

#include <allegro.h>

#define ALFONT_DLL
#include <alfont.h>

#include "common.h"
#include "player.h"
#include "game.h"
#include "map.h"
#include "steam.h"
#include "hud.h"


// graphic datafile headers
#include "graphics.h"
#include "hud_gfx.h"

// -----
// Variables
extern BITMAP *bmp_screen;

extern int frame_count, fps;  // fps and such 

// classes
extern cSteam steam;
extern cMap map;
extern cGame game;

// datafile
extern DATAFILE *graphics;
extern DATAFILE *hud;

// FONT stuff
extern ALFONT_FONT *game_font;
extern ALFONT_FONT *cs_font;

// ---------------------------------------------- 
// Head(s) Up Display for CS 2D
// ----------------------------------------------
// A group of functions only

void HUD_DrawID(int id, int x, int y, int iAlpha)
{
	if (iAlpha < 0)
	{
		if (game.iHudTransVal < 255)
		{
			set_trans_blender(255,255,255, game.iHudTransVal);
			iAlpha = game.iHudTransVal;
		}
	}	


	if (iAlpha < 255)
		draw_trans_sprite(bmp_screen, (BITMAP *)hud[id].dat, x, y);		
	else
		draw_sprite(bmp_screen, (BITMAP *)hud[id].dat, x, y);
}

void HUD_DrawNumber(char msg[10], int x, int y)
{
	// Draws a specific string using our own nice charset
	int iLength = strlen(msg);
	int i=0;
	BITMAP *temp;
	temp=create_bitmap(24,24);	

	int iHudTransVal = game.iHudTransVal;

	int iMinutes=0;
	int iSeconds=0;
	int iTime = game.iTimeLimit - game.iTheTime;
	iMinutes = iTime / 60;
	iSeconds = iTime - (iMinutes*60);

	if (iSeconds < 1)
		iHudTransVal += 128;
	
	if (iHudTransVal < 255)
		set_trans_blender(255,255,255, game.iHudTransVal);

	for (i=0; i < iLength; i++)
	{
		clear(temp);
		char cChar[1];
		sprintf(cChar, "%c", msg[i]);
		int iNr=atoi(cChar);
		
		blit((BITMAP *)hud[HUD_NUMBERS].dat, temp, (iNr*24), 0, 0, 0, 24, 24);
		
		if (iHudTransVal < 255)
			draw_trans_sprite(bmp_screen, temp, x+(i*24), y);		
		else
			draw_sprite(bmp_screen, temp, x+(i*24), y);

	}

	destroy_bitmap(temp);
}

// On bombsite, etc
void HUD_DrawStatus(int iID)
{
	if (game.iHudTransVal < 255)
		set_trans_blender(255,255,255, game.iHudTransVal);

	// Draw any icon that we can have:

	// Draw the time left
	int iMinutes=0;
	int iSeconds=0;
	int iTime = game.iTimeLimit - game.iTheTime;
	iMinutes = iTime / 60;
	iSeconds = iTime - (iMinutes*60);
	char cMinutes[5];
	char cSeconds[5];
	sprintf(cMinutes, "%d", iMinutes);
	int iHudTransVal = game.iHudTransVal;
	
	if (iSeconds < 1)
		iHudTransVal += 128;

	if (iSeconds < 10)
		sprintf(cSeconds, "0%d", iSeconds);
	else
		sprintf(cSeconds, "%d", iSeconds);
	
	HUD_DrawID(HUD_CLOCK, (game.screen_x/2) - 48, (game.screen_y-32), iHudTransVal);
	// Minutes
	HUD_DrawNumber(cMinutes, (game.screen_x/2)-24, (game.screen_y-32));
	// Dubble Dot (:)
	HUD_DrawID(HUD_DBLDOT, (game.screen_x/2)+1, (game.screen_y-32), iHudTransVal);
	// Seconds
	HUD_DrawNumber(cSeconds, (game.screen_x/2)+8, (game.screen_y-32));	
}

// Draw weapon stuff
void HUD_DrawSlots(int iID)
{
	if (game.iHudTransVal < 255)
		set_trans_blender(255,255,255, game.iHudTransVal);


}

// Draw player things (ammo, health, etc)
void HUD_DrawPlayer(int iID)
{
	if (game.iHudTransVal < 255)
		set_trans_blender(255,255,255, game.iHudTransVal);

}

// Main drawing routine
void HUD_Draw()
{
	// Draws the game.iPlayer ID , this is the actual player that is 'playing the game' or
	// 'being watched' in spectator mode.
	int iTheID=game.iPlayerID;

	HUD_DrawStatus(iTheID);
	// When the health of the player is < 0, then automaticly go to spectator mode..
}

