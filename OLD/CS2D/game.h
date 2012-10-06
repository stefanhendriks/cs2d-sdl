
#define GAME_MENU 1
#define GAME_GAME 2
#define GAME_EDITOR 3

#define COLOR_WHITE	makecol(255,255,255)
#define COLOR_GREY	makecol(160,170,149)

#define MAX_MAP_ENTITIES 250
#define MAX_TYPE_ENTITIES 50
#define MAX_PARTICLES 4096

#define MAX_TEXTURES 2024
#define MAX_TEXTUREGROUPS 1000
#define MAX_CLOUDS 50


// Settings
// High / Low
#define SETTING_DETAILS_HIGH 0
#define SETTING_DETAILS_LOW 1

// On / Of
#define SETTING_DETAILS_ON 0
#define SETTING_DETAILS_OFF 1


// in main.cpp
void TIME_process();

struct tTextureData
{
	BITMAP *bmp;		// a texture, created out of the "anything.dat" file
	bool bWater;		// water effect on this one?
	bool bDust;			// dust effect on this one?
};


struct tTextureGroup
{
	int iWidth;			// ...
	int iHeight;		// width and height (so we know when to 'cut off'
	int iTextures[64];	// textures references
};

class cGame
{

public:
	
	int screen_x;     // Screen X resolution
    int screen_y;     // Screen Y resolution

	bool windowed;	  // windowed mode or not?
	bool playing;	  // playing the game?

	int state;		// menu/in-game

	double dWaterAngle;

	bool bMouseLeftPressed;
	bool bMouseRightPressed;

	// Editor specific
	// ----
	bool bShowGroupTextures;	// Show group of textures in editor or SINGLE ones
	bool bBlockTextures;
	int iGroupNumber;
	int iSingleNumber;
	int iShadowNumber;			// Shadow (& Light!) number
	int iBrightness;

	// Game options thingies 
	int iTransDetails;
	int iCreatedParticles;
	int iHudTransVal;

	// HUD & Player stuff
	int iPlayerID;
	int iSpecID;
	cPlayer players[32];

	// Time
	int iTheTime;		// gets resetted
	
	// Server vars
	int iTimeLimit;		// time limit

	// ----
	int iSteamFade;
	int iScrollSpeed;

	// ----
	tTextureData DATA_Textures[MAX_TEXTURES];	// 250 textures
	tTextureGroup GROUP_Textures[MAX_TEXTUREGROUPS];	// 100 groups

	// ----
	void init();
	void run();

	void install_entities();	// install entities
	void LOAD_TexturesFromDataFile(char *filename);

	void CREATE_TEXTURE_GROUP(int iGroupNumber, int iHeight, int iWidth);
	void CREATE_Texture(BITMAP *bmp, int iGroupNumber, bool bWater, bool bDust);	// creates a texture.

	void DRAW_Players();
	void VERIFY_Players();
	void INPUT_Players(); // update input data

	void DO_Effects();

	// Methods to detect single-clicks
	bool MOUSE_PRESSED_LEFT();
	bool MOUSE_PRESSED_RIGHT();

private:

};