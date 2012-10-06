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

// -----
// Variables
extern BITMAP *bmp_screen;
extern BITMAP *bmp_collide;

extern int frame_count, fps;  // fps and such 

// classes
extern cSteam steam;
extern cMap map;

// datafile
extern DATAFILE *graphics;

// FONT stuff
extern ALFONT_FONT *game_font;
extern ALFONT_FONT *cs_font;

void cGame::init()
{
	screen_x = 640;
	screen_y = 480;

	dWaterAngle=0;
	
	iPlayerID=0;
	iSpecID=0;

	iTheTime=0;
	iTimeLimit = 310; // 300 seconds = 5 minutes

	iSteamFade = rnd(255);
	iScrollSpeed=0;
	iBrightness=255;
	iShadowNumber=0;

	windowed = true;
	playing  = true;

	// ----
	iTransDetails=SETTING_DETAILS_HIGH;
	iCreatedParticles=100;
	iHudTransVal = 128;
	// ----
	
	bShowGroupTextures=true;
	bBlockTextures=false;

	iGroupNumber=0;
	iSingleNumber=0;

	state = GAME_MENU;

	bMouseLeftPressed=false;
	bMouseRightPressed=false;

	int i;
	// texture data
	for (i=0; i < MAX_TEXTURES; i++)
	{
		DATA_Textures[i].bmp = NULL;
		DATA_Textures[i].bWater = false;
		DATA_Textures[i].bDust = false;
	}

	// texture groups
	for (i=0; i < MAX_TEXTUREGROUPS; i++)
	{
		GROUP_Textures[i].iHeight = -1;
		GROUP_Textures[i].iWidth = -1;

		for (int j=0; j < 64; j++)
			GROUP_Textures[i].iTextures[j] = -1;

	}

	for (i=0; i < MAX_PLAYERS; i++)
		players[i].init();

	players[0].iHealth = 100;
	players[0].iClientFlags = 1;
	players[0].iClass = PLAYER_LEET;
	players[0].iWeapon = WEAPON_M4A1;

	players[2].iHealth = 100;
	players[2].iClientFlags = 2;
	players[2].iClass = PLAYER_SAS;
	players[2].iWeapon = WEAPON_M4A1;
	players[2].x = 26;
	players[2].y = 26;
	players[2].iAngle = rnd(255);
	
	players[3].iHealth = 100;
	players[3].iClientFlags = 2;
	players[3].iClass = PLAYER_GIGN;
	players[3].iWeapon = WEAPON_M4A1;
	players[3].x = 66;
	players[3].y = 66;
	players[3].iAngle = rnd(255);
	
	players[4].iHealth = 100;
	players[4].iClientFlags = 2;
	players[4].iClass = PLAYER_GSG9;
	players[4].iWeapon = WEAPON_M4A1;
	players[4].x = 66;
	players[4].y = 26;
	players[4].iAngle = rnd(255);
	

	players[5].iHealth = 100;
	players[5].iClientFlags = 2;
	players[5].iClass = PLAYER_URBAN;
	players[5].iWeapon = WEAPON_M4A1;
	players[5].x = 26;
	players[5].y = 66;
	players[5].iAngle = rnd(255);
	

	players[1].iHealth = 100;
	players[1].iClientFlags = 2;
	players[1].iClass = PLAYER_ARCTIC;
	players[1].iWeapon = WEAPON_AK47;
	players[1].x = 300;
	players[1].y = 300;
	players[1].iAngle = rnd(255);


	map.create_particle(100,100, WEAPON_AK47_DROPPED, 100, 255, 0, 0);
		
	map.create_particle(300,300, WEAPON_M4A1_DROPPED, 100, 255, 0, 0);

	map.create_particle(500,400, SPRITE_C4, 100, 255, 0, 0);
}

// retrieve input
void cGame::INPUT_Players()
{
	for (int i=0; i < MAX_PLAYERS; i++)
	{
		if (players[i].iHealth > -1 && players[i].iClass > -1)
			players[i].input();
	}
}

// draw all players on the field
void cGame::DRAW_Players()
{
	for (int i=0; i < MAX_PLAYERS; i++)
	{
		if (players[i].iHealth > -1 && players[i].iClass > -1)
			players[i].draw();
	}
}

// verify all players 
void cGame::VERIFY_Players()
{
	for (int i=0; i < MAX_PLAYERS; i++)
	{
		if (players[i].iHealth > -1 && players[i].iClass > -1)
			players[i].verify();
	}
}

void cGame::install_entities()
{
	for (int i=0; i < MAX_TYPE_ENTITIES; i++)
	{
		map.entities[i].bBombSpot = false;
		map.entities[i].bHostage = false;
		map.entities[i].bRescueZone = false;
		map.entities[i].iGraphic = -1;
		map.entities[i].iHealth = 0;
		map.entities[i].iPlayer = -1;
	}

	// Install entity types here:

}

void cGame::CREATE_TEXTURE_GROUP(int iGroupNumber, int iHeight, int iWidth)
{
	if (iGroupNumber < 0 || iGroupNumber > MAX_TEXTUREGROUPS)
	{
		logbook("CREATE_TEXTURE_GROUP - ERROR: Invalid group number specified");
	}	

	GROUP_Textures[iGroupNumber].iHeight = iHeight;
	GROUP_Textures[iGroupNumber].iWidth = iWidth;

	// clear the group textures list
	for (int j=0; j < 64; j++)
		GROUP_Textures[iGroupNumber].iTextures[j] = -1;
}

void cGame::CREATE_Texture(BITMAP *bmp, int iGroupNumber, bool bWater, bool bDust)
{
	// create a bitmap now, and blit it to this
	int iNew=-1;
	for (int i=0; i < MAX_TEXTURES; i++)
	{
		if (DATA_Textures[i].bmp == NULL)
		{
			iNew=i;
			break;
		}
	}

	if (iNew < 0)
	{
		logbook("ERROR: Could not create texture, no free texture slots left.");
		return;
	}

	DATA_Textures[iNew].bmp = create_bitmap(32,32);
	// blit this
	blit(bmp, DATA_Textures[iNew].bmp, 0,0,0,0,32,32);
	DATA_Textures[iNew].bWater = bWater;
	DATA_Textures[iNew].bDust = bDust;

	// Add this texture, and when iGroupNumber > -1, set it to that group!
	if (iGroupNumber > -1)
	{
		if (GROUP_Textures[iGroupNumber].iHeight < 1)
			logbook("WARNING: Assigning texture to texture group with invalid iHeight\n");

		if (GROUP_Textures[iGroupNumber].iWidth < 1)
			logbook("WARNING: Assigning texture to texture group with invalid iWidth\n");

		for (int id=0; id < 64; id++)
		{
			if (GROUP_Textures[iGroupNumber].iTextures[id] < 0)
			{
				GROUP_Textures[iGroupNumber].iTextures[id] = iNew; // set textures array
				break;
			}
		}

		if (id > 63)
			logbook("ERROR: Could not assign texture to group; group is full!\n");
	}
}

void cGame::LOAD_TexturesFromDataFile(char *filename)
{
	//
	// this function loads a data file
	// examines it. Creates 32x32 textures out of the 'big' textures (from normal HL or whatever)
	// and then create GROUP_Textures out of them so we can still use them as 'a whole' or
	// in pieces if we want to.
	//

	logbook("----------------");
	logbook("LOADING TEXTURES");
	logbook("----------------");
	logbook("Initializing textures database.");
	logbook("Going to create temp-variables.");
	
	DATAFILE *textures;
	BITMAP *piece;
	int iGroupNumber=0;

	
	piece = create_bitmap(32,32);

	if (piece == NULL)
	{
		logbook("ERROR: Could not create bitmap! - (LOAD_TexturesFromDataFile() failed)");
		return;
	}	
	else
		logbook("Bitmap created.");

	textures = load_datafile(filename);

	if (textures == NULL)  
	{
		logbook("ERROR: Could not load datafile! - (LOAD_TexturesFromDataFile() failed)");
		return;
	}
	else
		logbook("Datafile loaded - beginning examination");
	
	int index=0;
	// for every BMP object in this datafile.
	while (index > -1)
	{	
		if (textures[index].type != DAT_BITMAP)
		{
			// this could be the end of the list of files, so do not say its a bad
			// object when its the end.
			if (textures[index].type == DAT_END)
				break;
			
			/*
			if (textures[index].type != DAT_END)
			{
				logbook("WARNING: Found an object in the datafile that is NOT a bitmap");
			}*/
			
			index++;
			continue;		
		}

		// water property found
		bool bWater=false;
		bool bDust=false;

		if (stricmp(get_datafile_property(textures+index, DAT_ID('R','E','A', 'C')), "WATER") == 0)
			bWater=true;

		if (stricmp(get_datafile_property(textures+index, DAT_ID('R','E','A', 'C')), "DUST") == 0)
			bDust=true;

		 
		int iWidth = ((BITMAP *)textures[index].dat)->w;
		int iHeight = ((BITMAP *)textures[index].dat)->h;
		
		char message[80];
		sprintf(message, "BEGIN PROCESSING: Texture %d , size %dx%d.", index, iWidth, iHeight);
		logbook(message);

		if (bWater)
			logbook("PROPERTY: REAC-WATER");


		iWidth = (iWidth/32);
		iHeight = (iHeight/32);

		CREATE_TEXTURE_GROUP(iGroupNumber, iHeight, iWidth);
		clear(piece);
		
		for (int iX=0; iX < iWidth; iX++)
			for (int iY=0; iY < iHeight; iY++)
			{
				// blit the correct piece
				blit((BITMAP *)textures[index].dat, piece, iY*32, iX*32, 0,0, 32,32);
				CREATE_Texture(piece, iGroupNumber, bWater, bDust);
			}			
		
		logbook("DONE PROCESSING: Texture");

		index++;
		iGroupNumber++;
	}	

	destroy_bitmap(piece);
}

// run the game
// keep it below
void cGame::run()
{

	map.load_map();
	int r,g,b;
	r=g=b=128;

	bool change=false;
	bool once=false;

	while (playing)
	{
		TIME_process();


		if (key[KEY_R]) r++;		
		if (key[KEY_G]) g++;
		if (key[KEY_B]) b++;

		if (r > 255) r = 0;
		if (g > 255) g = 0;
		if (b > 255) b = 0;

		
		
		// STEAM thinks all the time, no matter in what game state we are
		steam.think();
		
		// clear to 'light level color'!
		//clear_to_color(bmp_screen, map.iLightColor);
		clear(bmp_screen);
		clear_to_color(bmp_collide, makecol(255,0,255));


		// When in menu, steam is working all the time
		if (state == GAME_MENU)
		{
			// Draw background
			blit((BITMAP *)graphics[MENU_SHELL].dat, bmp_screen, 0,0,0,0, 640,480);

			// Draw - steam unpowered hahahaha
			int iFade=iSteamFade;
			if (iSteamFade > 255)
				iFade=256-(iSteamFade-255);

			set_trans_blender(255, 255, 255, iFade);
			draw_trans_sprite(bmp_screen, (BITMAP *)graphics[STEAM_NOTPOWERED].dat, 299,424);
			//masked_blit((BITMAP *)graphics[STEAM_NOTPOWERED].dat, bmp_screen, 0,0, 299,424,341,56);

			// Draw title of this game
			alfont_set_font_size(cs_font, 50); // set size
			alfont_textprintf(bmp_screen, cs_font, 2, 2, makecol(0,0,0), "Counter-Strike 2D");
			alfont_textprintf(bmp_screen, cs_font, 0, 0, COLOR_WHITE, "Counter-Strike 2D");
			alfont_set_font_size(cs_font, 15); // set size
			alfont_textprintf(bmp_screen, cs_font, 420, 50, COLOR_WHITE, "By Stefan Hendriks");
			alfont_set_font_size(cs_font, 10); // set size
			alfont_textprintf(bmp_screen, cs_font, 590, 470, COLOR_WHITE, "V 0.0.3");

			// key input
			if (key[KEY_ESC])
				playing = false; // get out					


		}
		// when not in menu, steam is only working... well when we press a certain key or something
		else if (state == GAME_GAME)
		{	
			map.draw(false);

			
			// now draw the water effect (first copy the bitmaps)
			if (steam.active())
			{
				
			}
			else
			{
				// key input
				if (key[KEY_ESC])
				{
					steam.iMainState = STATE_INGAME_OPTIONS;
					steam.iSubState = STATE_NONE;
				}
			}

			DO_Effects();

			INPUT_Players();
			VERIFY_Players();
			HUD_Draw();

		}
		else if (state == GAME_EDITOR)
		{
			map.draw(true);

			DO_Effects();
			// steam is working here standard...
		}

		
		steam.draw(); // steam always draws at last.. as it overrules all


		// When steam is active OR we are in the menu, show mouse
		if (steam.active() || state == GAME_MENU || state == GAME_EDITOR)
		{			
			draw_sprite(bmp_screen, (BITMAP *)graphics[MOUSE].dat,mouse_x, mouse_y);
		}

				
		frame_count++;

		alfont_textprintf(bmp_screen, game_font, 0,screen_y-12, makecol(255,255,255), "FPS: %d", fps);

		// Eventually bmp_water is sampled, that is bmp_screen + water effect
		blit(bmp_screen, screen, 0, 0, 0, 0, screen_x, screen_y);		
	}
}

void cGame::DO_Effects()
{
	/*
	BITMAP *temp;
	temp=create_bitmap(32,32);
	clear_to_color(temp, makecol(128,128,128));

	for (int i=0; i < MAX_TEXTURES; i++)
	{
		if (DATA_Textures[i].bWater)
		{
			draw_sprite(temp, DATA_Textures[i].bmp, 0, 0);			
			
			double a=180;

			for (int x1=0; x1 < 32; x1++)
				for (int y1=0; y1 < 32; y1++)
				{		
					

				}
			draw_sprite(DATA_Textures[i].bmp, temp , 0, 0);
		}			
	}

	dWaterAngle++;
	if (dWaterAngle>180)
		dWaterAngle=-180;
destroy_bitmap(temp);*/
}


// Mouse pressed left dude
bool cGame::MOUSE_PRESSED_LEFT()
{
	if (mouse_b & 1)
	{
		if (!bMouseLeftPressed)
		{
			bMouseLeftPressed=true;
			return true;
		}
		
		return false;
	}
	else
		bMouseLeftPressed=false; // when not pressing, unselect it here

	return false;
}