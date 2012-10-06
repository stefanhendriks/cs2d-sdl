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

#include "ppcol.h"


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
extern cGame game;

// datafile
extern DATAFILE *graphics;
extern DATAFILE *shadows;

// FONT stuff
extern ALFONT_FONT *game_font;
extern ALFONT_FONT *cs_font;


void cPlayer::init()
{
	iHealth=0;		// health
	iArmor=100;			// armor
	iTeam=-1;			// team
	iClass=-1;			// model	
	iWeapon=WEAPON_AK47;	// model to draw of weapon
	
	// speed
	fMoveSpeed=0.0;
	fSideSpeed=0.0;
	bMovingVertical=false;
	bMovingHorizontal=false;



	// Hud drawing / Weapon selection + weapon memory
	int i,j;
	i=j=0;

	for (i=0; i < 5; i++)
		for (j=0; j < 3; j++)
			WeaponSlot[i][j].iWeaponType = 0;	// filled weapon slot(s)

	for (i=0; i < MAX_WEAPONS; i++)
	{
		WeaponAmmo[i].iClip = 0;	// each weapon has ammo
		WeaponAmmo[i].iLeft = 0;	// ...
	}

	iClientFlags=0;	
	x=(game.screen_x/2);
	y=(game.screen_y/2);
	dNextX=x;
	dNextY=y;


	iAngle=0;
}

void cPlayer::spawn()
{

}

// verify movement
void cPlayer::verify()
{
	// when we should check...
	if (dNextX != x || dNextY != y)
	{
		BITMAP *player;
		player=create_bitmap(30,30);

		clear_to_color(player, makecol(255,255,255));
	
		// Player drawing
		int iDx, iDy;
		iDx=iDy=0;
		
		bool bXgo=false;
		bool bYgo=false;
		bool bBgo=false;

		// Calculate drawing X and Y


		// boundries
		if (dNextX < 0) dNextX=0;
		if (dNextY < 0) dNextY=0;

		iDx=dNextX-map.lScrollX;
		iDy=dNextY-map.lScrollY;
		
		// collision
		if (!check_pp_collision_normal(player, bmp_collide, iDx-14, iDy-14, 0, 0))		
			bBgo=true;
		else
		{
			// First check X
			iDx=dNextX-map.lScrollX;
			//iDy=dNextY-map.lScrollY;
			iDy=y-map.lScrollY;


			// collision
			if (!check_pp_collision_normal(player, bmp_collide, iDx-14, iDy-14, 0, 0))		
				bXgo=true;
			
			// now do Y
			iDx=x-map.lScrollX;
			iDy=dNextY-map.lScrollY;		

			// collision
			if (!check_pp_collision_normal(player, bmp_collide, iDx-14, iDy-14, 0, 0))		
				bYgo=true;
			
		}

		bool bNewSet=false;

		if (bBgo)
		{
			x = dNextX;
			y = dNextY;
			bNewSet=true;
		}
		// only change X
		else if (bXgo)
		{
			x=dNextX;
			dNextY=y;
			bNewSet=true;
		}
		// only change Y
		else if (bYgo)
		{
			dNextX=x;
			y=dNextY;
			bNewSet=true;
		}
		// we may not move at all!
		else
		{			
			dNextX=x;
			dNextY=y;
		}

		if (bNewSet)
		{
			// check tile we are standing on
			int mapx, mapy;
			mapx = x/32;
			mapy = y/32;

			// water effect
			if (map.map[mapx][mapy].iProperty & BIT_WATER)
			{
				int iW=((BITMAP *)graphics[SPRITE_RAINDROP01].dat)->w;
				int iH=((BITMAP *)graphics[SPRITE_RAINDROP01].dat)->h;

				if (rnd(100) < 50)
					map.create_particle(x-(iW/2) + (-(iW/4)+rnd((iW/2))), y-(iH/2)+ (-(iH/4)+rnd((iH/2))), SPRITE_RAINDROP01, 100, 20+rnd(90), 5, 0);
					
			}

			// dust effect
			if (map.map[mapx][mapy].iProperty & BIT_DUST)
			{
				int iW=((BITMAP *)graphics[SPRITE_PUFF01].dat)->w;
				int iH=((BITMAP *)graphics[SPRITE_PUFF01].dat)->h;

				if (rnd(100) < 30)
					map.create_particle(x-(iW/2) + (-(iW/4)+rnd((iW/2))), y-(iH/2)+ (-(iH/4)+rnd((iH/2))), SPRITE_PUFF01, 100, 45, 5, 0);
			}
		}

		destroy_bitmap(player);
	}

	if (iClientFlags == 1)
	{
		// scrolling is centered around player and such...
		map.lScrollX = x - (game.screen_x/2);
		map.lScrollY = y - (game.screen_y/2);

		if (map.lScrollX < 0) map.lScrollX = 0;
		if (map.lScrollY < 0) map.lScrollY = 0;
	}

}
// retrieve input
void cPlayer::input()
{
	bMovingVertical=false;
	bMovingHorizontal=false;
	
	int iDx, iDy;
	iDx=iDy=0;

	// Calculate drawing X and Y
	iDx=x-map.lScrollX;
	iDy=y-map.lScrollY;

	// react on keys:
	if (iClientFlags==1)
	{	
		double dDistance=length(iDx+16, iDy+16, mouse_x, mouse_y); // get the desired stuff from mouse

		// when distance is bigger then 20; we begin deciding to move (how fast)		
		if (dDistance > 20)
		{
			double fMaxSpeed = MAX_RUNSPEED * (dDistance / MAX_RUNSPEED_LENGTH) + MIN_RUNSPEED;

			// move
			if (key[KEY_W] && fMoveSpeed < fMaxSpeed)	
			{
				if (fMoveSpeed < MIN_RUNSPEED)
					fMoveSpeed = MIN_RUNSPEED;
				else
					fMoveSpeed +=MOVESTEP;

				// fix
				if (fMoveSpeed > fMaxSpeed)
					fMoveSpeed = fMaxSpeed;

				bMovingVertical=true;
			}
			else if (key[KEY_S] && fMoveSpeed > -fMaxSpeed)
			{
				if (fMoveSpeed > -MIN_RUNSPEED)
					fMoveSpeed = -MIN_RUNSPEED;
				else
					fMoveSpeed -=MOVESTEP;

				// fix
				if (fMoveSpeed < -fMaxSpeed)
					fMoveSpeed = -fMaxSpeed;

				bMovingVertical=true;
			}
		}

		
		// strafe
		if (key[KEY_A] && fSideSpeed < MAX_SIDESPEED)
		{
			if (fSideSpeed < MIN_SIDESPEED)
				fSideSpeed = MIN_SIDESPEED;
			else
				fSideSpeed += SIDESTEP;
			
			bMovingHorizontal=true;
		}
		else if (key[KEY_D] && fSideSpeed > -MAX_SIDESPEED)
		{
			if (fSideSpeed > -MIN_SIDESPEED)
				fSideSpeed = -MIN_SIDESPEED;
			else
				fSideSpeed -= SIDESTEP;
			
			bMovingHorizontal=true;
		}		
	}	
	// or update (clientflags==2)
	else if (iClientFlags==2)
	{
		// client
		// retrieve movespeed updates and such
	}	
	else
	{
		// bot
		// yeap
	}

}

void cPlayer::think()
{

	// Depending on move speed & sidespeed we move the player
	if (iHealth > -1)
	{
		
		double iNewX, iNewY;

		iNewX=x;
		iNewY=y;

		int iDx, iDy;
		iDx=iDy=0;
		
		// Calculate drawing X and Y
		iDx=x-map.lScrollX;
		iDy=y-map.lScrollY;


		// When we can scroll, scroll
		//int iCX=(game.screen_x/2+16);
		//int iCY=(game.screen_y/2+16);

		if (fMoveSpeed != 0)
		{			
			  double delta_x = (mouse_x-(iDx));
			  double delta_y = (mouse_y-(iDy));  
			  
			  // calculate radians
			  double r = (atan2(delta_y, delta_x));			  

			  double vx = (cos (r)) * fMoveSpeed;
			  double vy = (sin (r)) * fMoveSpeed;

			  iNewX += vx;
			  iNewY += vy;

			//  char msg[256];
			//  sprintf(msg, "Angle (r) = %f, vx=%lf, vy=%lf, iNewX=%d, iNewY=%d", (float)r, (float)vx, (float)vy, iNewX, iNewY);
			//  logbook(msg);

			  if (bMovingVertical==false)
			  {
				if (fMoveSpeed > 0.0f)
					fMoveSpeed -= 0.05;
								
				if (fMoveSpeed < 0.0f)
					fMoveSpeed += 0.05;

				if (fabs(fMoveSpeed) < 0.1)
					fMoveSpeed = 0.0;

			  }			  
		}

		// now do side speed!
		if (fSideSpeed != 0)
		{	
				// calculate the angle ; increase it; calculate new x and y out from it.. phew
			double delta_x = (iDx-mouse_x);
			double delta_y = (iDy-mouse_y);  

			// calculate radians
			double r = (atan2(delta_y, delta_x));			  

			// degrees
			double degrees = r * (180 / PI);

			// length for circle
			double l = length(iDx, iDy, mouse_x, mouse_y);

			// depending on left/right (add 90 degrees to 'strafe')
			if (fSideSpeed > 0.0)
				degrees += 90;
			else
				degrees -= 90;
			
			// figure out radians again (strafed ones)
			double radians = degrees * (PI / 180);

			// With this we can calculate the vx and vy
			double vx = (cos (radians)) * (l);
			double vy = (sin (radians)) * (l);
			
			// include X and Y
			vx+=iNewX;
			vy+=iNewY;

			// We know now the x and y where to go to. Now use radians again to move to it (like
			// we do when we move straight to it.
			
			delta_x = (vx-iNewX);
			delta_y = (vy-iNewY);  

			// calculate radians
			r = (atan2(delta_y, delta_x));		
			float fSpeed = fabs(fSideSpeed);

			double nextx = (cos (r)) * fSpeed;
			double nexty = (sin (r)) * fSpeed;

			iNewX = iNewX + nextx;
			iNewY = iNewY + nexty;
		
			if (bMovingHorizontal==false)
			{
				if (fSideSpeed < 0.0f)
					fSideSpeed += 0.05;
				
				if (fSideSpeed > 0.0f)
					fSideSpeed -= 0.05;

				if (fabs(fSideSpeed) < 0.1)
					fSideSpeed = 0.0;
			}
		}	
				
		// set these, they will be checked in the next frame so we can verify and do collision
		// detection.
		dNextX = iNewX;
		dNextY = iNewY;
		// check on any collision, and when thats ok, set the X and Y
	}
}

void cPlayer::draw()
{

	if (iClass < 0)
		return;
		
	// draw player shadow
	int iDx, iDy;
	iDx=iDy=0;

	// Calculate drawing X and Y
	iDx=x-map.lScrollX;
	iDy=y-map.lScrollY;

	
	// calculate angle and draw it
	set_trans_blender(255,255,255,128);

	draw_trans_sprite(bmp_screen,(BITMAP *)shadows[0].dat, iDx-16, iDy-16);
	
	fixed angle;
	// Calculate angle now and draw player		
	if (iClientFlags == 1)
	{	
		angle=degrees(iDx+16,iDy+16 , mouse_x, mouse_y);	
		int iAngle2=fixtoi(angle);
		
		line(bmp_screen, mouse_x-16, mouse_y, mouse_x+16, mouse_y, makecol(255,255,255));
		line(bmp_screen, mouse_x, mouse_y-16, mouse_x, mouse_y+16, makecol(255,255,255));

		line(bmp_screen, iDx, iDy, mouse_x, mouse_y, makecol(128,128,128));

		iAngle=iAngle2;
	}	
	


	
	angle=itofix(iAngle);

	// Now draw the weapon
	rotate_sprite(bmp_screen, (BITMAP *)graphics[iWeapon].dat, iDx-24, iDy-24, angle);

	// now draw the player	
	rotate_sprite(bmp_screen, (BITMAP *)graphics[iClass].dat, iDx-16, iDy-16, angle);
	
}