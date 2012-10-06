/*

  Player class

  */

#define MAX_RUNSPEED_LENGTH 200


#define MAX_RUNSPEED	1.75
#define MAX_SIDESPEED	2.5
#define MIN_RUNSPEED	0.2
#define MIN_SIDESPEED   0.4
#define SIDESTEP		0.07
#define MOVESTEP		0.05

// Each player has a weapon slot (1to5, and each slot has max of 3 weapons)
struct tWeaponSlot
{
	int iWeaponType;	// refer to any weapon type (for right drawing, action, etc)	
};

struct tWeaponAmmo
{
	int iClip;			// How many in clip?
	int iLeft;			// And what is left?
};

class cPlayer
{

public:
	int iHealth;		// health
	int iArmor;			// armor
	int iTeam;			// team
	int iClass;			// model	
	int iWeapon;		// weapon ID to draw

	int iClientFlags;			// flags
	int iAngle;

	float fMoveSpeed;	// move speed
	float fSideSpeed;	// side speed

	// Pixel perfect baby!
	double x;
	double y;

	// the positions that the player wants to go to, this will be checked first in the next frame!
	double dNextX;
	double dNextY;

	bool bMovingHorizontal;
	bool bMovingVertical;
	
	// Hud drawing / Weapon selection + weapon memory
	tWeaponSlot WeaponSlot[5][3];	// filled weapon slot(s)
	tWeaponAmmo WeaponAmmo[MAX_WEAPONS];	// each weapon has ammo

	// Functions
	void init();

	// think
	void think();

	// input
	void input();

	// verify
	void verify();
	
	// Spawn the player (make it alive)
	void spawn();

	// draw player
	void draw();

private:
};