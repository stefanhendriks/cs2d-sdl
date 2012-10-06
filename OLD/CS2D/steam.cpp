/*

  Counter-Strike 2D

  A tribute to steam... erm.. sort off

  */

#include <time.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <allegro.h>

#define ALFONT_DLL
#include <alfont.h>

#include "graphics.h"
#include "common.h"
#include "player.h"
#include "game.h"
#include "map.h"
#include "steam.h"
#include "hud.h"



extern BITMAP *bmp_screen;

extern DATAFILE *graphics;
extern DATAFILE *shadows;

extern ALFONT_FONT *game_font;

extern cGame game;
extern cMap map;

void cSteam::init()
{
	iMainState = STATE_MAIN_MENU;
	iSubState = STATE_NONE;

	game.iBrightness = map.iGeneralLightLevel;

	// initialize menu stuff
	for (int i=0; i < 10; i++)
	{
		if (i < 6)
		{
		MENU_MainMenu[i].iMainState = -1;
		MENU_MainMenu[i].iSubState = -1;
		memset(MENU_MainMenu[i].text, 0, sizeof(MENU_MainMenu[i].text));

		MENU_OptionsMenu[i].iMainState = -1;
		MENU_OptionsMenu[i].iSubState = -1;
		memset(MENU_OptionsMenu[i].text, 0, sizeof(MENU_OptionsMenu[i].text));

		MENU_IngameMenu[i].iMainState = -1;
		MENU_IngameMenu[i].iSubState = -1;
		memset(MENU_IngameMenu[i].text, 0, sizeof(MENU_IngameMenu[i].text));
		}

		EDITOR_MainMenu[i].iMainState = -1;
		EDITOR_MainMenu[i].iSubState = -1;
		memset(EDITOR_MainMenu[i].text, 0, sizeof(EDITOR_MainMenu[i].text));
	}
	
	// ***** 
	// Create main menu items
	// *****
	MENU_MainMenu[0].iMainState = STATE_MAIN_NEWGAME;
	MENU_MainMenu[0].iSubState = STATE_NONE;
	strcpy(MENU_MainMenu[0].text, "New Game");

	MENU_MainMenu[1].iMainState = STATE_MAIN_OPTIONS;
	MENU_MainMenu[1].iSubState = STATE_NONE;
	strcpy(MENU_MainMenu[1].text, "Options");

	MENU_MainMenu[2].iMainState = STATE_MAIN_EDITOR;
	MENU_MainMenu[2].iSubState = STATE_NONE;
	strcpy(MENU_MainMenu[2].text, "Editor");

	MENU_MainMenu[5].iMainState = STATE_MAIN_EXIT;
	MENU_MainMenu[5].iSubState = STATE_NONE;
	strcpy(MENU_MainMenu[5].text, "Quit");

	// ********************************
	// Options
	MENU_OptionsMenu[0].iMainState = STATE_MAIN_OPTIONS;
	MENU_OptionsMenu[0].iSubState = STATE_NONE; 
	strcpy(MENU_OptionsMenu[0].text, "Controls");

	MENU_OptionsMenu[1].iMainState = STATE_MAIN_OPTIONS;
	MENU_OptionsMenu[1].iSubState = STATE_NONE; 
	strcpy(MENU_OptionsMenu[1].text, "Video");

	MENU_OptionsMenu[2].iMainState = STATE_MAIN_OPTIONS;
	MENU_OptionsMenu[2].iSubState = STATE_NONE; 
	strcpy(MENU_OptionsMenu[2].text, "Audio");

	// Devider
	MENU_OptionsMenu[4].iMainState = STATE_MAIN_MENU;
	MENU_OptionsMenu[4].iSubState = STATE_NONE; 
	strcpy(MENU_OptionsMenu[4].text, "-");
	// Devider

	MENU_OptionsMenu[5].iMainState = STATE_MAIN_MENU;
	MENU_OptionsMenu[5].iSubState = STATE_NONE; 
	strcpy(MENU_OptionsMenu[5].text, "Back");

	// *********************************
	// In-Game menu
	
	MENU_IngameMenu[0].iMainState = STATE_NONE;
	MENU_IngameMenu[0].iSubState = STATE_NONE;
	strcpy(MENU_IngameMenu[0].text, "Return to game");

	MENU_IngameMenu[1].iMainState = STATE_NONE;
	MENU_IngameMenu[1].iSubState = STATE_NONE;
	strcpy(MENU_IngameMenu[1].text, "Options");

	MENU_IngameMenu[5].iMainState = STATE_MAIN_MENU;
	MENU_IngameMenu[5].iSubState = STATE_NONE;
	strcpy(MENU_IngameMenu[5].text, "Quit to menu");

	// *********************************
	// Editor options
	EDITOR_MainMenu[0].iMainState = STATE_NONE;
	EDITOR_MainMenu[0].iSubState = STATE_NONE;
	strcpy(EDITOR_MainMenu[0].text, "New map");

	EDITOR_MainMenu[1].iMainState = STATE_NONE;
	EDITOR_MainMenu[1].iSubState = STATE_NONE;
	strcpy(EDITOR_MainMenu[1].text, "Load map");

	EDITOR_MainMenu[2].iMainState = STATE_NONE;
	EDITOR_MainMenu[2].iSubState = STATE_NONE;
	strcpy(EDITOR_MainMenu[2].text, "Save map");

	// Devider
	EDITOR_MainMenu[3].iMainState = STATE_EDITOR_MAPPROP;
	EDITOR_MainMenu[3].iSubState = STATE_NONE;
	strcpy(EDITOR_MainMenu[3].text, "-");
	// Devider

	EDITOR_MainMenu[4].iMainState = STATE_EDITOR_MAPPROP;
	EDITOR_MainMenu[4].iSubState = STATE_NONE;
	strcpy(EDITOR_MainMenu[4].text, "Map properties");
	
	EDITOR_MainMenu[5].iMainState = STATE_EDITOR;
	EDITOR_MainMenu[5].iSubState = STATE_EDITOR_SUB_TEXTURES;
	strcpy(EDITOR_MainMenu[5].text, "Textures");

	EDITOR_MainMenu[6].iMainState = STATE_EDITOR;
	EDITOR_MainMenu[6].iSubState = STATE_EDITOR_SUB_LIGHT;
	strcpy(EDITOR_MainMenu[6].text, "Lights / Shadows");
	
	EDITOR_MainMenu[7].iMainState = STATE_EDITOR_MAPPROP;
	EDITOR_MainMenu[7].iSubState = STATE_NONE;
	strcpy(EDITOR_MainMenu[7].text, "Entities");
	
	// Devider
	EDITOR_MainMenu[8].iMainState = STATE_EDITOR_MAPPROP;
	EDITOR_MainMenu[8].iSubState = STATE_NONE;
	strcpy(EDITOR_MainMenu[8].text, "-");
	// Devider

	EDITOR_MainMenu[9].iMainState = STATE_MAIN_MENU;
	EDITOR_MainMenu[9].iSubState = STATE_NONE;
	strcpy(EDITOR_MainMenu[9].text, "Back to menu");

	
	// *********************************
	// Map properties

	// *********************************
	// Textures
	// DOES NOT USE A MENU

	// *********************************
	// Entities




}

// reacts on everything (input)
void cSteam::think()
{

}

// draws everything (output)
void cSteam::draw()
{
	if (game.state == GAME_MENU)
	{

		// Menu Menu window is always drawn...
		WindowDraw(bmp_screen, 16,360,120,100, "Main Menu");

		// Main Menu / main
		if (iMainState == STATE_MAIN_MENU)
		{
			// Draw menu items and react upon them
			DrawMenuItems(36, 390, 0, true);
		}

		if (iMainState == STATE_MAIN_OPTIONS)
		{
			// Draw main options buttons, but do not use them
			DrawMenuItems(36, 390, 0, false);

			// draw options window
			WindowDraw(bmp_screen, 150, 60, 400,400, "Options");

			DrawMenuItems(170, 90, 1, true);

			// devide bar (vertically)
			line(bmp_screen, 240, 90, 240, 420, makecol(40,46,34));
			line(bmp_screen, 241, 90, 241, 420, makecol(136,145,128));

		}

		// Start a new game
		if (iMainState == STATE_MAIN_NEWGAME)
		{
			iMainState = STATE_NONE;
			iSubState  = STATE_NONE;
			game.state = GAME_GAME;
		}

		// Start editor
		if (iMainState == STATE_MAIN_EDITOR)
		{
			iMainState = STATE_EDITOR;
			iSubState  = STATE_NONE;
			game.state = GAME_EDITOR;
		}

		// get out the game here
		if (iMainState == STATE_MAIN_EXIT)
			game.playing = false;
	}
	
	// In the game
	if (game.state == GAME_GAME)
	{
		if (iMainState == STATE_INGAME_OPTIONS)
		{
			WindowDraw(bmp_screen, 160,120,320,240, "In-game options");
			DrawMenuItems(180, 150, 2, true);
		}

		if (iMainState == STATE_MAIN_MENU)
			game.state = GAME_MENU;
	}

	// In the editor
	if (game.state == GAME_EDITOR)
	{
		if (iMainState == STATE_MAIN_MENU)
			game.state = GAME_MENU;

		// Editor with options
		WindowDraw(bmp_screen, 0, 300, 160, 180, "Editor");
		// Minimap window
		WindowDraw(bmp_screen, 0, 0, 160, 140, "Minimap");

		if (iMainState == STATE_EDITOR)
		{
			DrawMenuItems(20, 330, 3, true);			

			// depending on sub-state:
			if (iSubState == STATE_EDITOR_SUB_TEXTURES)
			{
				WindowDraw(bmp_screen, 0, 141, 160, 158, "Textures");
				bool bBlock;

				// draw a window of the current texture + 2 buttons (next/prev)
				bool bNext = ButtonDraw(bmp_screen, 126, 274, 19, 16, ">>");
				bool bPrev = ButtonDraw(bmp_screen, 16, 274, 19, 16, "<<");
				
				bool bUp = ButtonDraw(bmp_screen, 140, 187, 12, 20, "+");
				bool bDown = ButtonDraw(bmp_screen, 140, 227, 12, 40, "-");
				
				// Blocked or not?
				if (game.bBlockTextures)
					bBlock = ButtonDraw(bmp_screen, 140, 167, 12, 20, "U");
				else
					bBlock = ButtonDraw(bmp_screen, 140, 167, 12, 20, "B");

				if (bBlock)
				{
					if (game.bBlockTextures)
						game.bBlockTextures=false;
					else
						game.bBlockTextures=true;
				}

				// ---- 
				// BUTTON: Switch from GROUP to SINGLE and back
				bool bGroupPressed = false;
				if (game.bShowGroupTextures == false)
					bGroupPressed = ButtonDraw(bmp_screen, 46, 274, 68, 16, "< GROUP >");
				else
					bGroupPressed = ButtonDraw(bmp_screen, 46, 274, 68, 16, "< SINGLE >");

				if (bGroupPressed)
				{
					if (game.bShowGroupTextures)
						game.bShowGroupTextures = false;
					else
						game.bShowGroupTextures = true;
				}
				// ----

				// next
				if (bNext)
				{
					if (game.bShowGroupTextures)
						game.iGroupNumber++;
					else
						game.iSingleNumber++;
				}
				
				// go back
				if (bPrev)
				{
					if (game.bShowGroupTextures)
						game.iGroupNumber--;
					else
						game.iSingleNumber--;

					if (game.iGroupNumber < 0) game.iGroupNumber = 0;
					if (game.iSingleNumber < 0) game.iSingleNumber = 0;
				}

				// Upper and lower the brightness
				if (bUp)
				{
					game.iBrightness+=10;
					if (game.iBrightness > 255)
						game.iBrightness = 255;
				}

				if (bDown)
				{
					game.iBrightness -=10;
					if (game.iBrightness < 10)
						game.iBrightness = 10;
				}
				
				// draw something black in the window
				rectfill(bmp_screen, 10, 165, 130, 260, makecol(0,0,0));
				set_clip(bmp_screen, 10, 165, 130, 260);

				set_trans_blender(255, 255, 255, game.iBrightness);

				// clip the entire texture group / or single texture!
				if (game.bShowGroupTextures)
				{
					// draw the entire thingy
					int dx=0;
					int dy=0;				
					
					for (int i=0; i < 64; i++)
					{
						if (game.GROUP_Textures[game.iGroupNumber].iTextures[i] > -1)
						{
							// now create tile at the correct spot at the map:
							draw_trans_sprite(bmp_screen, game.DATA_Textures[game.GROUP_Textures[game.iGroupNumber].iTextures[i]].bmp, 11+(dx*32), 166+(dy*32));
					
							dx++;
							if (dx >= game.GROUP_Textures[game.iGroupNumber].iWidth)
							{
								dx=0;
								dy++;
							}			
						}
					}
				}
				else
				{
					// its a 32x32 square
					if (game.DATA_Textures[game.iSingleNumber].bmp != NULL)
						draw_trans_sprite(bmp_screen, game.DATA_Textures[game.iSingleNumber].bmp, 60, 180);
				}

				set_clip(bmp_screen, 0, 0, game.screen_x, game.screen_y);


				// When the mouse is on the editing field, and a mouse button is pressed
				// add the tiles (group or single)
				if (mouse_x > 160)
				{
					if (game.MOUSE_PRESSED_LEFT())
					{
						// calculate x and y
						int x=(mouse_x-160)/32;
						int y=mouse_y/32;
						x+=(map.lScrollX/32);
						y+=(map.lScrollY/32);

						if (game.bShowGroupTextures)						
							map.create_tile_group(x,y,game.iGroupNumber, game.iBrightness, game.bBlockTextures);
						else
							map.create_tile(x,y,game.iSingleNumber, game.iBrightness, game.bBlockTextures);
						
					}
					else
					{
						int ddx, ddy;
						ddx=mouse_x/32;
						ddy=mouse_y/32;
						
						ddx*=32;
						ddy*=32;

						// Increase (because editor = 160 (=5x32) + 5 pixels = 165)
						ddx+=5;

						// Make sure scrolling is taken into account to!
						ddx-= (map.lScrollX)-((int)(map.lScrollX/32)*32);
						ddy-= (map.lScrollY)-((int)(map.lScrollY/32)*32);



					/*	ddx+= (map.lScrollX-(map.lScrollX/32));
						ddy+= (map.lScrollY-(map.lScrollY/32));*/

						if (game.bShowGroupTextures)
						{
							int dx=0;
							int dy=0;				
							
							if (game.GROUP_Textures[game.iGroupNumber].iWidth > -1)
							{
								rectfill(bmp_screen, ddx+2, ddy+2, ddx+2+game.GROUP_Textures[game.iGroupNumber].iWidth*32, ddy+2+game.GROUP_Textures[game.iGroupNumber].iHeight*32, makecol(0,0,0));
							}

							for (int i=0; i < 64; i++)
							{
								if (game.GROUP_Textures[game.iGroupNumber].iTextures[i] > -1)
								{
									// now create tile at the correct spot at the map:
									draw_trans_sprite(bmp_screen, game.DATA_Textures[game.GROUP_Textures[game.iGroupNumber].iTextures[i]].bmp, ddx+(dx*32), ddy+(dy*32));
									
									dx++;
									if (dx >= game.GROUP_Textures[game.iGroupNumber].iWidth)
									{
										dx=0;
										dy++;
									}			
								}
							}							
						}
						else
						{
							if (game.DATA_Textures[game.iSingleNumber].bmp != NULL)
							{
								rectfill(bmp_screen, ddx+2, ddy+2, ddx+34, ddy+34, makecol(0,0,0));								
								draw_trans_sprite(bmp_screen, game.DATA_Textures[game.iSingleNumber].bmp, ddx, ddy);
							}
						}
					}
				}
			}
				// substate
				if (iSubState == STATE_EDITOR_SUB_LIGHT)
				{
					WindowDraw(bmp_screen, 0, 141, 160, 158, "Light/Shadows");
					// draw a window of the current texture + 2 buttons (next/prev)
					bool bNext = ButtonDraw(bmp_screen, 126, 274, 19, 16, ">>");
					bool bPrev = ButtonDraw(bmp_screen, 16, 274, 19, 16, "<<");
					
					bool bUp = ButtonDraw(bmp_screen, 140, 167, 12, 40, "+");
					bool bDown = ButtonDraw(bmp_screen, 140, 227, 12, 40, "-");
				
					// next
					if (bNext)
						if (shadows[game.iShadowNumber].type != DAT_END)
						game.iShadowNumber++;
					
					// go back
						if (bPrev || shadows[game.iShadowNumber].type != DAT_BITMAP)
							game.iShadowNumber--;
					
					if (game.iShadowNumber < 0) game.iShadowNumber = 0;
					

					// Upper and lower the brightness
					if (bUp)
					{
						game.iBrightness+=10;
						if (game.iBrightness > 255)
							game.iBrightness = 255;
					}

					if (bDown)
					{
						game.iBrightness -=10;
						if (game.iBrightness < 10)
							game.iBrightness = 10;
					}
					
					// draw something black in the window
					rectfill(bmp_screen, 10, 165, 130, 260, makecol(192,192,192));
					set_clip(bmp_screen, 10, 165, 130, 260);

					// fill in
					set_trans_blender(255, 255, 255, game.iBrightness);				
					draw_trans_sprite(bmp_screen, (BITMAP *)shadows[game.iShadowNumber].dat, 60, 180);

					// set clip back
					set_clip(bmp_screen, 0, 0 , game.screen_x, game.screen_y);

					if (mouse_x > 160)
					{
						if (game.MOUSE_PRESSED_LEFT())
						{
							// calculate x and y
							int x=(mouse_x-160)/32;
							int y=mouse_y/32;						
							x+=(map.lScrollX/32);
							y+=(map.lScrollY/32);

							map.shadow_map[x][y] = game.iShadowNumber;
						}
						else
						{
							int ddx, ddy;
							ddx=mouse_x/32;
							ddy=mouse_y/32;
							
							ddx*=32;
							ddy*=32;

							// Increase (because editor = 160 (=5x32) + 5 pixels = 165)
							ddx+=5;

							// Make sure scrolling is taken into account to!
							ddx-= (map.lScrollX)-((int)(map.lScrollX/32)*32);
							ddy-= (map.lScrollY)-((int)(map.lScrollY/32)*32);

							rectfill(bmp_screen, ddx+2, ddy+2, ddx+34, ddy+34, makecol(0,0,0));						
							draw_trans_sprite(bmp_screen, (BITMAP *)shadows[game.iShadowNumber].dat, ddx, ddy);
							
						}
					}				
				}
		}


	}
}

void cSteam::DrawMenuItems(int x, int y, int iMenu, bool bActive)
{
	tMenuItem MENU_Items[10]; // max of 10 options

	unsigned int iWidth=0;

	// initialize menu stuff
	for (int i=0; i < 10; i++)
	{
		MENU_Items[i].iMainState = -1;
		MENU_Items[i].iSubState = -1;
		memset(MENU_Items[i].text, 0, sizeof(MENU_Items[i].text));
	}

	// Main Menu
	if (iMenu == 0)
	{
		// copy the menu items
		for (int i=0; i < 6; i++)
		{
			MENU_Items[i].iMainState = MENU_MainMenu[i].iMainState;
			MENU_Items[i].iSubState = MENU_MainMenu[i].iSubState;			
			memcpy(MENU_Items[i].text, MENU_MainMenu[i].text, sizeof(MENU_MainMenu[i].text));
			if (strlen(MENU_Items[i].text) > iWidth) iWidth = strlen(MENU_Items[i].text);
		}
	}	
	// any other menu
	else if (iMenu == 1)
	{
		// copy the menu items
		for (int i=0; i < 6; i++)
		{
			MENU_Items[i].iMainState = MENU_OptionsMenu[i].iMainState;
			MENU_Items[i].iSubState = MENU_OptionsMenu[i].iSubState;
			memcpy(MENU_Items[i].text, MENU_OptionsMenu[i].text, sizeof(MENU_OptionsMenu[i].text));
			if (strlen(MENU_Items[i].text) > iWidth) iWidth = strlen(MENU_Items[i].text);
		}
	}
	else if (iMenu == 2)
	{
		// copy the menu items
		for (int i=0; i < 6; i++)
		{
			MENU_Items[i].iMainState = MENU_IngameMenu[i].iMainState;
			MENU_Items[i].iSubState = MENU_IngameMenu[i].iSubState;
			memcpy(MENU_Items[i].text, MENU_IngameMenu[i].text, sizeof(MENU_IngameMenu[i].text));
			if (strlen(MENU_Items[i].text) > iWidth) iWidth = strlen(MENU_Items[i].text);
		}

	}
	else if (iMenu == 3)
	{
		// copy the menu items
		for (int i=0; i < 10; i++)
		{
			MENU_Items[i].iMainState = EDITOR_MainMenu[i].iMainState;
			MENU_Items[i].iSubState = EDITOR_MainMenu[i].iSubState;
			memcpy(MENU_Items[i].text, EDITOR_MainMenu[i].text, sizeof(EDITOR_MainMenu[i].text));
			if (strlen(MENU_Items[i].text) > iWidth) iWidth = strlen(MENU_Items[i].text);
		}

	}


	alfont_set_font_size(game_font, 14); // set size
	
	int iTextY=y;	

	// Now draw / print / react etc
	for (int id=0; id < 10; id++)
	{
		// valid
		if (strlen(MENU_Items[id].text) > 0)
		{
			// draw this thing on the screen
			int iColor=COLOR_GREY;

			if (strcmp(MENU_Items[id].text, "-") == 0)
			{
				// devide bar
				line(bmp_screen, x, iTextY+7, x+(iWidth*8), iTextY+7, makecol(40,46,34));
				line(bmp_screen, x, iTextY+8, x+(iWidth*8), iTextY+8, makecol(136,145,128));
				iTextY += 14;
			}
			else
			{
				if (bActive)
				{
					// determine if mouse is over this one
					int iWidth = strlen(MENU_Items[id].text) * 8;
					
					if ((mouse_x > x && mouse_x < (x+iWidth)) && (mouse_y > iTextY && mouse_y < (iTextY+12)))
						iColor = COLOR_WHITE;
				}

			alfont_textprintf(bmp_screen, game_font, x, iTextY, iColor, MENU_Items[id].text);

			iTextY += 14;

			if (iColor == COLOR_WHITE)
				if (game.MOUSE_PRESSED_LEFT())
				{
					iMainState = MENU_Items[id].iMainState;
					iSubState = MENU_Items[id].iSubState;
				}
			} // its not a devider
		}
	}

}

// returns true when clicked on it
bool cSteam::ButtonDraw(BITMAP *dest, int x, int y, int width, int height, char *title)
{
	bool result=false;

	if (width < 12) width = 12;
	if (height < 12) height = 12;

	rectfill(dest, x, y, x+width, y+height, makecol(76,88,68));

	// now draw lines
	line(dest, x, y, x+(width), y, makecol(136,145,128));
	line(dest, x, y, x, y+(height), makecol(136,145,128));
	line(dest, x+(width), y, x+(width), y+(height), makecol(40,46,34));
	line(dest, x, y+(height), x+(width), y+(height), makecol(40,46,34));

	// now draw text
	alfont_set_font_size(game_font, 14); // set size
	
	int color = COLOR_GREY;	

	if ((mouse_x > x && mouse_x < (x+width)) && (mouse_y > y && mouse_y < (y+height)))
	{
		color = COLOR_WHITE;		
	
		if (game.MOUSE_PRESSED_LEFT())
			result = true;
	}

	alfont_textprintf(dest, game_font, x+2, y+2,color, title);

	return result;
}


// draw a nice steam colored window
void cSteam::WindowDraw(BITMAP *dest, int x, int y, int width, int height, char *title)
{
	// Draw a neat looking window.

	// Minimum sizes are: 32x32
	if (width < 32) width=32;
	if (height < 32) height=32;

	// first draw 'the 4' (corners)	

	// UP LEFT
	draw_sprite(dest, (BITMAP *)graphics[STEAM_UPLEFT].dat, x, y);
	// UP RIGHT
	draw_sprite(dest, (BITMAP *)graphics[STEAM_UPRIGHT].dat, x+(width-16), y);

	// DOWN LEFT
	draw_sprite(dest, (BITMAP *)graphics[STEAM_DOWNLEFT].dat, x, y+(height-16));

	// DOWN RIGHT
	draw_sprite(dest, (BITMAP *)graphics[STEAM_DOWNRIGHT].dat, x+(width-16), y+(height-16));

	// draw the 'bars'
	line(dest, x+16, y, x+(width-16), y, makecol(136,145,128));
	line(dest, x, y+16, x, y+(height-16), makecol(136,145,128));

	line(dest, x+(width), y+16, x+(width), y+(height-16), makecol(40,46,34));
	line(dest, x+16, y+(height), x+(width-16), y+(height), makecol(40,46,34));

	// Fill it

	// middle
	rectfill(dest, x+16, y+16, x+(width-16), y+(height-16), makecol(76,88,68));

	// up
	rectfill(dest, x+16, y+1, x+(width-16), y+16, makecol(76,88,68));

	// down
	rectfill(dest, x+16, y+(height-16), x+(width-16), y+(height-1), makecol(76,88,68));

	// left
	rectfill(dest, x+1, y+16, x+16, y+(height-16), makecol(76,88,68));

	// right
	rectfill(dest, x+(width-16), y+16, x+(width-1), y+(height-16), makecol(76,88,68));


	// title:
	// size of text = 12
	alfont_set_font_size(game_font, 14); // set size
	alfont_textprintf(dest, game_font, x+17, y+2,makecol(255,255,255), title);

	// devide bar
	line(dest, x+17, y+17, x+(width-17), y+17, makecol(40,46,34));
	line(dest, x+17, y+18, x+(width-17), y+18, makecol(136,145,128));

	// ...
}

// active?
bool cSteam::active()
{
	if (iMainState != STATE_NONE)
		return true;

	return false;
}