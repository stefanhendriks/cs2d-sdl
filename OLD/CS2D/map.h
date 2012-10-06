/*

  Counter-Strike 2D

  Map Header file

  Tiles can be 32x32, but textures can be bigger.

  */

#define ENTITY_BOMBSPOT	0
//#define ENTITY_

#define BIT_WATER	(1 << 1)
#define BIT_DUST	(1 << 2)


// A tile
struct tTile
{
	int iTexture;
	int iEntityType;		// Any possible entity is linked here
	int iLightLevel;		// Lightlevel
	bool bBlockable;
	bool bCanSeeThrough;
	int iProperty;
};

// Entities:
/*
	I do not code this to code it like HL; i code it because i find it fun
	So no base class, sub-classes and shit. Not needed anyways. Keep it simple

	An entity in the game is anything - EXCEPT A PLAYER / BOT -. So this can be
	an item; a box you have to blow up, or an area. 

	Entities are 'types' (pre-defined) in the game. A tile can refer to an entity
	ie for areas (ie, bombspot tiles can refer to this, so you can plant the bomb)

*/
struct tEntity
{
	int iHealth;		// health / hitpoints
	
	// Graphical info
	int iGraphic;		// Tile to show, this one is from entities.dat

	// Flags for entity
	bool bBombSpot;		// Is it a bombspot area?
	bool bRescueZone;	// is it a rescue zone?
	bool bHostage;		// is it a hostage?
	
	// 
	int iPlayer;		// Reference to any player ID (ie for hostage to follow)
};


struct tParticle
{
	float x;
	float y;
	int iAlpha;
	int iGraphic;
	int iHealth;
	int iTimer;
	int iSpeed;
	int iLayer;
};

struct tCloud
{
	float x;
	float y;
	int iTimer;
	int iAlpha;
	int iSpeed;
	int iBitmap;	// bitmap referal	
};

class cMap
{

public:

	void init();	
	void draw(bool bEditor);

	void create_tile(int x, int y, int iTexture, int iLightLevel, bool bBlock);
	void create_tile_group(int x, int y, int iGroupNumber, int iLightLevel, bool bBlock);

	void load_map();

	void think();
	void think_particles();
	void think_clouds();

	// Creation of objects
	void create_particle(long x, long y, int iGraphic, int iHealth, int iAlpha, int iSpeed, int iLayer);
	void create_cloud(int x, int y, int iBitmap, int iAlpha, int iSpeed);

	int iGeneralLightLevel;			// general light level
	int iLightColor;				// Color of the general light
	int iClouds;					// general amount of clouds...
	int iRainDrops;

	// 20 types of entities in the game
	tEntity entities[MAX_TYPE_ENTITIES];	// entity types (!!)

	// ----
	tTile map[1024][1024]; // Map of 1024x1024 tiles	
	int shadow_map[1024][1024];	// cast shadow here! (0 = no shadow, > 0 = shadow tile id)
	int iEntities[MAX_MAP_ENTITIES];		// 250 entities, they refer to entities[]! (on map)
	tParticle particle[MAX_PARTICLES];
	tCloud cloud[MAX_CLOUDS];
		
	double lScrollX, lScrollY; // pixel perfect scrolling dude
};