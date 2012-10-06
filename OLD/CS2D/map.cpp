#include <time.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

// classes
extern cSteam steam;
extern cGame game;

// datafile
extern DATAFILE *graphics;
extern DATAFILE *shadows;


// Initialize
void cMap::init()
{
	lScrollX = lScrollY = 0;

	iClouds = 2;
	iLightColor = makecol(255,255,20);
	iGeneralLightLevel = 255;
	iRainDrops=50;

	int x,y;
	for (x=0; x < 1024; x++)
		for (y=0; y < 1024; y++)
		{
			map[x][y].bBlockable = false;
			map[x][y].bCanSeeThrough = false;
			map[x][y].iTexture = -1;
			map[x][y].iEntityType = -1;
			map[x][y].iLightLevel = iGeneralLightLevel;
			map[x][y].iProperty=0;

			shadow_map[x][y] = -1;
		}

	int i;

	for (i=0; i < MAX_MAP_ENTITIES; i++)
		iEntities[i] = -1; // none are active

	for (i=0; i < MAX_PARTICLES; i++)
	{
		particle[i].iAlpha = 128;
		particle[i].iGraphic = -1;
		particle[i].iHealth = -1;
		particle[i].x = 0;
		particle[i].y = 0;
		particle[i].iTimer = 0;
		particle[i].iLayer=0;
	}

	for (i=0; i < MAX_CLOUDS; i++)
	{
		cloud[i].x = -1;
		cloud[i].y = -1;
		cloud[i].iTimer=0;
		cloud[i].iBitmap=-1;
		cloud[i].iAlpha=-1;
		cloud[i].iSpeed=-1;
	}

}

void cMap::load_map()
{
	// load map
}

// Create a tile
void cMap::create_tile(int x, int y, int iTexture, int iLightLevel, bool bBlock)
{
	// put on X, Y
	map[x][y].iTexture = iTexture;
	map[x][y].iLightLevel = iLightLevel;
	map[x][y].bCanSeeThrough = false;
	map[x][y].iEntityType = -1;
	map[x][y].iLightLevel = iLightLevel;
	map[x][y].bBlockable = bBlock;	
	
	if (game.DATA_Textures[iTexture].bWater)
		map[x][y].iProperty |= BIT_WATER; // set bit water
	else 
	{
		if (map[x][y].iProperty & BIT_WATER)
			map[x][y].iProperty &= (~BIT_WATER); // remove
	}
	
	if (game.DATA_Textures[iTexture].bDust)
		map[x][y].iProperty |= BIT_DUST; // set bit dust
	else
	{
		if (map[x][y].iProperty & BIT_DUST)
			map[x][y].iProperty &= (~BIT_DUST); // remove
	}

}

// creates a complete 'texture', that was devided into tiles. Wohoo
void cMap::create_tile_group(int x, int y, int iGroupNumber, int iLightLevel, bool bBlock)
{
	if (iGroupNumber < 0)
		return;
	
	int dx=0;
	int dy=0;
	bool bFirst=false;

	for (int i=0; i < 64; i++)
	{
		if (game.GROUP_Textures[iGroupNumber].iTextures[i] > -1)
		{
			// now create tile at the correct spot at the map:
			create_tile(x+dx, y+dy, game.GROUP_Textures[iGroupNumber].iTextures[i], iLightLevel, bBlock);			

			dx++;
			if (dx >= game.GROUP_Textures[iGroupNumber].iWidth)
			{
				dx=0;
				dy++;				
			}			
		}
	}	
}

// Create a particle
void cMap::create_particle(long x, long y, int iGraphic, int iHealth, int iAlpha, int iSpeed, int iLayer)
{
	int iNew=-1;
	for (int i=0; i < game.iCreatedParticles; i++)
		if (particle[i].iHealth < 0)
		{
			iNew=i;
			break;
		}

	if (iNew < 0)
		return;

	particle[iNew].iHealth = iHealth;
	particle[iNew].iAlpha = iAlpha;
	particle[iNew].x = x;
	particle[iNew].y = y;
	particle[iNew].iGraphic = iGraphic;
	particle[iNew].iTimer = 0;
	particle[iNew].iSpeed = iSpeed;
	particle[iNew].iLayer = iLayer;
}

void cMap::think_particles()
{
	for (int i=0; i < MAX_PARTICLES; i++)
	{
		if (particle[i].iHealth > -1)
		{
			// smoke grenade / smoke cloud
			if (particle[i].iGraphic >= SPRITE_SMOKE01 &&
				particle[i].iGraphic <= SPRITE_SMOKE30)
			{
				particle[i].iTimer++;
				if (particle[i].iTimer > particle[i].iSpeed)
				{
					particle[i].iGraphic++;
					particle[i].iTimer = 0;

					particle[i].x += (-1) + rnd(3);
					particle[i].y += (-1) + rnd(3);

					particle[i].iSpeed++;
				}

				if (particle[i].iGraphic > SPRITE_SMOKE30)
					particle[i].iHealth = -1;
			}
						
			// Raindrop
			if (particle[i].iGraphic >= SPRITE_RAINDROP01 &&
				particle[i].iGraphic <= SPRITE_RAINDROP09)
			{
				particle[i].iTimer++;
				if (particle[i].iTimer > particle[i].iSpeed)
				{
					particle[i].iGraphic++;
					particle[i].iTimer = 0;
										
					if (particle[i].iAlpha > 10)
						particle[i].iAlpha -= 5;
				}

				if (particle[i].iGraphic > SPRITE_RAINDROP09)
					particle[i].iHealth = -1;
			}

			// Explosion 1
			if (particle[i].iGraphic >= SPRITE_EXPLO001 &&
				particle[i].iGraphic <= SPRITE_EXPLO025)
			{
				particle[i].iTimer++;
				if (particle[i].iTimer > particle[i].iSpeed)
				{
					particle[i].iGraphic++;
					particle[i].iTimer = 0;
				}

				if (particle[i].iGraphic == SPRITE_EXPLO015)
					create_particle(particle[i].x, particle[i].y, SPRITE_SMOKE01, 100, 20, 1, 0);

				if (particle[i].iGraphic > SPRITE_EXPLO025)
				{
					particle[i].iHealth = -1;					
				}
			}

			// Smoke puff (machine fire on walls, etc)
			if (particle[i].iGraphic >= SPRITE_PUFF01 &&
				particle[i].iGraphic <= SPRITE_PUFF10)
			{
				particle[i].iTimer++;
				if (particle[i].iTimer > particle[i].iSpeed)
				{
					particle[i].iGraphic++;
					particle[i].iTimer = 0;

					if (particle[i].iAlpha > 10)
						particle[i].iAlpha-= 2;

					// Puffs travel..
					particle[i].x+=2;
				}
				
				if (particle[i].iGraphic > SPRITE_PUFF10)
				{
					particle[i].iHealth = -1;					
				}
			}
		}
	}
}

// think
void cMap::think_clouds()
{
	for (int i=0; i < MAX_CLOUDS; i++)
	{
		if (cloud[i].iBitmap > -1)
		{
			cloud[i].iTimer++;

			// move cloud
			if (cloud[i].iTimer > cloud[i].iSpeed)
			{
				cloud[i].iTimer = 0;
				
				int r = rnd(100);
				if (r < 20)
					cloud[i].x++;
				else if (r > 80)
					cloud[i].x--;

				cloud[i].y++;
			}
		}
	}

	// & rain
	int iRain=0;
	for (i=0; i < MAX_PARTICLES; i++)
	{
		if (particle[i].iGraphic >= SPRITE_RAINDROP01 &&
			particle[i].iGraphic <= SPRITE_RAINDROP09)
			iRain++;
	}

	if (iRain < iRainDrops)
	{
		// create rain drops dude
		int amount = iRainDrops - iRain;
		int made=0;
		while (amount > made || made < ((game.iCreatedParticles/MAX_PARTICLES)*10))
		{
			create_particle(rnd(game.screen_x) + lScrollX, rnd(game.screen_y) + lScrollY, SPRITE_RAINDROP01, 100, 50+rnd(127), rnd(10), 1);
			made++;
		}		
	}

}


// create a cloud
void cMap::create_cloud(int x, int y, int iBitmap, int iAlpha, int iSpeed)
{
	int iNewId=-1;
	for (int i=0; i < MAX_CLOUDS; i++)
	{
		if (cloud[i].iBitmap < 0)
		{
			iNewId=i;
			break;
		}
	}

	if (iNewId < 0)
		return;

	cloud[i].iBitmap = iBitmap;
	cloud[i].x = x;
	cloud[i].y = y;
	cloud[i].iAlpha = iAlpha;
	cloud[i].iSpeed = iSpeed;
}

// The map itself thinks... every second
void cMap::think()
{
	int Clouds=0;
	for (int i=0; i < MAX_CLOUDS; i++)
		if (cloud[i].iBitmap > -1)
			Clouds++;

	if (Clouds < iClouds)
	{
		if (rnd(100) < 50)
			create_cloud((-256 + rnd(game.screen_x)), -255, CLOUD_SHADOW01+rnd(3), 10+rnd(50), 2+rnd(12));
	}	
}

void cMap::draw(bool bEditor)
{
	// Draw this, depending on scrollX and scrollY
	// they are pixel perfect!
	
	if (lScrollX < 0) lScrollX = 0;
	if (lScrollY < 0) lScrollY = 0;

	// ----

	int iStartX = lScrollX / 32;
	int iStartY = lScrollY / 32;
	
	// Remaining pixels...
	int iSX = lScrollX - (iStartX * 32);
	int iSY = lScrollY - (iStartY * 32);
		
	// using devision here, because we can later alter this for resolution
	int iEndX = (640/32) + 3;
	int iEndY = (480/32) + 3; 

	iEndX = iStartX + iEndX;
	iEndY = iStartY + iEndY;

	int draw_x, draw_y;
	draw_x=draw_y=0;

	// Draw lights first (, ie, its drawn transparant, so if we want a light, we draw
	// something white first, then draw this sprite transparant on it.
	// This is for 'background lightning'. 

	// look in array
	//set_trans_blender(255, 255, 255, 128);
	for (int dx=iStartX; dx < iEndX; dx++)
	{
		for (int dy=iStartY; dy < iEndY; dy++)
		{
			// array has texture, draw it
			if (map[dx][dy].iTexture > -1)
			{
				if (map[dx][dy].iLightLevel < 255)
					set_trans_blender(255, 255, 255, map[dx][dy].iLightLevel);				
					
				int x,y;

				x=draw_x-iSX;
				y=draw_y-iSY;

				// take bar into account in editor
				if (bEditor) x+= 165;

				// draw texture from texture thingies!				
				if (map[dx][dy].iLightLevel < 255 && game.iTransDetails == SETTING_DETAILS_HIGH)				
					draw_trans_sprite(bmp_screen, game.DATA_Textures[map[dx][dy].iTexture].bmp, x, y);
				else
					draw_sprite(bmp_screen, game.DATA_Textures[map[dx][dy].iTexture].bmp, x, y);

				if (map[dx][dy].bBlockable)
				{
					rectfill(bmp_collide, x, y, x+32, y+32, makecol(255,255,255));
					
					if (bEditor)
						draw_sprite(bmp_screen, (BITMAP *)graphics[EDITOR_BLOCKED].dat, x, y);
				}
				else
				{					

				}
			
				
			}
			draw_y += 32;
		}
		draw_y = 0;
		draw_x += 32;
	}


	if (bEditor)
	{
		// scrolling
		if (mouse_x >= 639 && lScrollX < 999999)
			lScrollX += game.iScrollSpeed;
		
		if (mouse_x <= 1 && lScrollX > 0)		
			lScrollX -=game.iScrollSpeed;
		
		// Y
		if (mouse_y >= 479 && lScrollY < 999999)
			lScrollY+=game.iScrollSpeed;

		if (mouse_y <= 1 && lScrollY > 0)
			lScrollY-=game.iScrollSpeed;

	
	}

	// For 'top' lighting , we draw lights OVER it..

	// TODO: Draw any rippling effects here on any rippling map available on sight
	for (int i=0; i < MAX_PARTICLES; i++)
	{
		if (particle[i].iHealth > -1 && particle[i].iGraphic > -1 
			&& particle[i].iLayer == 0)
		{
			// check alpha
			set_trans_blender(255, 255, 255, particle[i].iAlpha);

			int dx, dy;
			dx = particle[i].x;
			dy = particle[i].y;

			dx -= lScrollX;
			dy -= lScrollY;

			if (bEditor)
				dx+=165;

			if (game.iTransDetails == SETTING_DETAILS_HIGH)
				draw_trans_sprite(bmp_screen, (BITMAP *)graphics[particle[i].iGraphic].dat, dx, dy);
			else
				draw_sprite(bmp_screen, (BITMAP *)graphics[particle[i].iGraphic].dat, dx, dy);

			
		}
	}	


	// Draw players here!
	if (bEditor == false)
	game.DRAW_Players();

	// Cast shadow here
	draw_x=draw_y=0;
	set_trans_blender(255, 255, 255, 128);
	for (dx=iStartX; dx < iEndX; dx++)
	{
		for (int dy=iStartY; dy < iEndY; dy++)
		{
			// array has texture, draw it
			if (shadow_map[dx][dy] > -1)
			{		
				int x,y;

				x=draw_x-iSX;
				y=draw_y-iSY;

				// take bar into account in editor
				if (bEditor) x+= 165;

				// draw texture from texture thingies!
				if (game.iTransDetails == SETTING_DETAILS_HIGH)
					draw_trans_sprite(bmp_screen, (BITMAP *)shadows[shadow_map[dx][dy]].dat, x, y);
				else
					draw_sprite(bmp_screen, (BITMAP *)shadows[shadow_map[dx][dy]].dat, x, y);
			}
			draw_y += 32;
		}
		draw_y = 0;
		draw_x += 32;
	}



	// draw particles on top of everything
	//set_trans_blender(255, 255, 255, 128);
	for (i=0; i < MAX_PARTICLES; i++)
	{
		if (particle[i].iHealth > -1 && particle[i].iGraphic > -1 
			&& particle[i].iLayer == 1)
		{
			// check alpha
			set_trans_blender(255, 255, 255, particle[i].iAlpha);

			int dx, dy;
			dx = particle[i].x;
			dy = particle[i].y;

			dx -= lScrollX;
			dy -= lScrollY;

			if (bEditor)
				dx+=165;

			if (game.iTransDetails == SETTING_DETAILS_HIGH)
				draw_trans_sprite(bmp_screen, (BITMAP *)graphics[particle[i].iGraphic].dat, dx, dy);
			else
				draw_sprite(bmp_screen, (BITMAP *)graphics[particle[i].iGraphic].dat, dx, dy);
		}
	}	

	
	// On top of everything, clouds cast shadows	
	for (i = 0; i < MAX_CLOUDS; i++)
	{
		if (cloud[i].iBitmap > -1)
		{
			if (cloud[i].x > -256 && cloud[i].y > -256 &&
				cloud[i].x < game.screen_x+256 && cloud[i].y < game.screen_y+256)
			{
				set_trans_blender(255, 255, 255, cloud[i].iAlpha);
				// draw this fucking cloud
				int dx, dy;
				dx = cloud[i].x - (lScrollX/100);
				dy = cloud[i].y - (lScrollY/100);	
				
				if (bEditor)
					dx+=165;

				draw_trans_sprite(bmp_screen, (BITMAP *)graphics[cloud[i].iBitmap].dat, dx, dy);
			}
			else
				cloud[i].iBitmap = -1;
		}
	}
}