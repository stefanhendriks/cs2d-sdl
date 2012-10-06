/*

  Counter-Strike 2D

  A tribute to steam... erm.. sort off

  */

#define STATE_NONE	0

// Main states
#define STATE_MAIN_MENU	1
#define STATE_MAIN_OPTIONS	2
#define STATE_MAIN_NEWGAME 3
#define STATE_MAIN_EDITOR 4
#define STATE_MAIN_EXIT 99

#define STATE_INGAME_OPTIONS 80

// Editor
#define STATE_EDITOR 50
#define STATE_EDITOR_MAPPROP 51


// Sub states

// Editor
#define STATE_EDITOR_SUB_TEXTURES 1
#define STATE_EDITOR_SUB_ENTITIES 2
#define STATE_EDITOR_SUB_PLAYERS 3
#define STATE_EDITOR_SUB_LIGHT	4

// Menu struct
struct tMenuItem
{
	int iSubState; // set substate to this one when going on this menu
	int iMainState; // set mainstate to this one...
	char text[45];	// Text to show on the screen	
};

class cSteam
{
public:
	void init();  // initializes
	void draw();
	void think(); // think & act
	void WindowDraw(BITMAP *dest, int x, int y, int width, int height, char *title);
	bool ButtonDraw(BITMAP *dest, int x, int y, int width, int height, char *title);
	void DrawMenuItems(int x, int y, int iMenu, bool bActive);
	

	int iMainState;	// Ie, main menu, in-game, etc
	int iSubState;	// Ie, options, setup, etc

	bool active(); // active?

	tMenuItem MENU_MainMenu[6];	// 6 main menu items
	tMenuItem MENU_OptionsMenu[6];	// 6 options menu items
	tMenuItem MENU_IngameMenu[6];	// 6 menu items
	tMenuItem EDITOR_MainMenu[10];	// 6 editor menu items

private:

	// private? euh
};
