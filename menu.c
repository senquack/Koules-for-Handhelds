/***********************************************************
*                      K O U L E S                         *
*----------------------------------------------------------*
*  C1995 JAHUSOFT                                          *
*        Jan Hubicka                                       *
*        Dukelskych Bojovniku 1944                         *
*        390 03 Tabor                                      *
*        Czech Republic                                    *
*        Phone: 0041-361-32613                             *
*        eMail: hubicka@limax.paru.cas.cz                  *
*----------------------------------------------------------*
*   Copyright(c)1995,1996 by Jan Hubicka.See README for    *
*                    licence details.                      *
*----------------------------------------------------------*
*  menu.c                                                  *
*   GUI menuing system					   *
***********************************************************/
/* Changes for joystick "accelerate by deflection"         *
 *  (c) 1997 by Ludvik Tesar (Ludv\'{\i}k Tesa\v{r})       *
 ************************LT*********************************/

#include "koules.h"
#include "net.h"
#include "client.h"
#ifdef JOYSTICK
#include <sys/ioctl.h>
#endif

//DKS
#include "sdl/input.h"

//DKS
#include "sdl/interface.h"
#include "sound.h"

typedef struct
  {
    char           *text;
    void            (*func) ();
  }
Menu;

typedef struct
  {
    int             x, y, *number, max, min;
    int             line;
    void            (*func) ();
  }
Numbers;

int             ssound = 1;
static Numbers  minim[2], maxim[2];
static int      nmenu;
static int      nnumbers;
static Menu     menu[20];
static int      selected = 0;
static float    mx1, my1, mx1p, my1p;
static float    mx2, my2, mx2p, my2p;
static int      mtime;
static char     playertext[] = " 1  PLAYER";
static char     leveltext[] = "LEVEL  000 ";
#define YPOSITION(i) (MAPHEIGHT/2+20-5*nmenu+10*i)
#define XPOSITION(i) (MAPWIDTH/2-4*strlen(menu[i].text))
#define XPOSITION1(i) (MAPWIDTH/2+4*strlen(menu[i].text))
static int      player, keynum, lastplayer;
#ifdef JOYSTICK
static int      joystick, joypos;
static struct JS_DATA_TYPE js_data;
#endif

#ifdef GP2X
//DKS - ADDED FOR SHOWING VOLUME
static char volstr[20] = "";
#endif

//DKS - added this to tell if we are in the damned actual game or in the main menu
//			, to facilitate whether we should draw scores along bottom
int in_actual_game = 0;

static void     main_menu ();
static void     change_menu ();
#ifdef JOYSTICK
static void
calibrate_init ()
{
  int             status;
  long            tmpl;

  if (joystickdevice[joystick]<0)
    {
      printf("Joystick %c not found during initialization.", joystick+'A');
      joystickplayer[joystick] = -2;
      gamemode = MENU;
      change_menu ();
      return;
    }

  status = ioctl (joystickdevice[joystick], JS_GET_TIMELIMIT, &tmpl);
  if (status == -1)
    {
      perror ("Joystick");
      joystickplayer[joystick] = -2;
      gamemode = MENU;
      change_menu ();
      return;
    }

  tmpl = 10; /* 10miliseconds */
  status = ioctl (joystickdevice[joystick], JS_SET_TIMELIMIT, &tmpl);
  if (status == -1)
    {
      perror ("Joystick");
      joystickplayer[joystick] = -2;
      gamemode = MENU;
      change_menu ();
      return;
    }
}
#endif


static void
player_change ()
{
  player = (selected - 2) / 2;
  lastplayer = (selected - 2) / 2;
#ifdef MOUSE
  if (player == mouseplayer)
    return;
#endif
#ifdef JOYSTICK
  if (player == joystickplayer[0])
    {
      joystick = 0;
      joypos = 0;
      gamemode = JOY;
      calibrate_init ();
      return;
    }
  if (player == joystickplayer[1])
    {
      joystick = 1;
      joypos = 0;
      gamemode = JOY;
      calibrate_init ();
      return;
    }
#endif
  keynum = 0;
  gamemode = KEYS;
}


static void
control_change ()
{
  int             player;
  player = (selected - 1) / 2;
#ifdef NETSUPPORT
  if (client && !control[player])
    return;
#endif
#ifdef MOUSE
   if(mouseplayer<0) mouseplayer=-1;
   if(mouseplayer>=nrockets) mouseplayer=-1;
#endif
#ifdef JOYSTICK
   if(joystickplayer[0]<0) joystickplayer[0]=-1;
   if(joystickplayer[0]>=nrockets) joystickplayer[0]=-1;
   if(joystickplayer[1]<0) joystickplayer[1]=-1;
   if(joystickplayer[1]>=nrockets) joystickplayer[1]=-1;
#ifdef NETSUPPORT
  if(client && !control[joystickplayer[0]]) joystickplayer[0]=-1;
  if(client && !control[joystickplayer[1]]) joystickplayer[1]=-1;
  if (client && !control[player])
    return;
#endif
#endif
#ifdef NETSUPPORT
  if(client && !control[mouseplayer]) mouseplayer=-1;
#endif

   if (rotation[player]==1)
      {
         rotation[player] = 0;
#ifdef MOUSE
	 if (mouseplayer == -1 && !nomouse) mouseplayer = player;
#ifdef JOYSTICK
         else goto maybejoystick;
	   /* mouse couldn't be chosen, but let's offer joystick if available*/
#endif
      }
   else if (player == mouseplayer)
      {
         mouseplayer = -1;
#endif
#ifdef JOYSTICK
maybejoystick:;
         if((joystickplayer[0] == -1)&&(joystickdevice[0]>=0))
            {
               joystickplayer[0]=player;
	       joystickmul[0]=JOYMUL1;
	    }
         else if((joystickplayer[1] == -1)&&(joystickdevice[1]>=0))
            {
   	       joystickplayer[1]=player;
	       joystickmul[1]=JOYMUL1;
	    }
      }
   else if(player==joystickplayer[0])
      {
         if(joystickmul[0]==JOYMUL1)joystickmul[0]=JOYMUL2;
	 else
	    {
	       joystickplayer[0]=-1;
	       if((joystickplayer[1] == -1)&&(joystickdevice[1]>=0))
	          {
		     joystickplayer[1]=player;
                     joystickmul[1]=JOYMUL1;
	          }
	    }
      }
   else if(player==joystickplayer[1])
      {
	 if(joystickmul[1]==JOYMUL1)joystickmul[1]=JOYMUL2;
	 else joystickplayer[1]=-1;
#endif
      }
   else if (rotation[player]==0)
      {
         rotation[player] = 1;
      }
   change_menu ();
}

void
start ()
{
  int             i;
#ifdef JOYSTICK
  if (joystickplayer[0] >= 0 && !calibrated[0])
    {
      joystick = 0;
      joypos = 0;
      gamemode = JOY;
      calibrate_init ();
      return;
    }
  if (joystickplayer[1] >= 1 && !calibrated[1])
    {
      joystick = 1;
      joypos = 0;
      gamemode = JOY;
      calibrate_init ();
      return;
    }
#endif
  for (i = 0; i < nrockets; i++)
    {
      object[i].score = 0;
#ifdef JOYSTICK
      if(joystickplayer[0]==i)
	  {
	     object[i].joymulx = joystickmul[0]/center[0][0];
	     object[i].joymuly = joystickmul[0]/center[0][1];
	     object[i].joythresh = joystickthresh[0];
	  }
      else if(joystickplayer[1]==i)
	  {
	     object[i].joymulx = joystickmul[1]/center[1][0];
	     object[i].joymuly = joystickmul[1]/center[1][1];
	     object[i].joythresh = joystickthresh[1];
	  }
#endif
    }
  sound = ssound;
  gamemode = GAME;
  in_actual_game = 1;
  gameplan_init ();
  init_objects ();
}


//DKS - limited to two players on handhelds
//static void
//playerchange ()
//{
///*  nrockets++;
//   if (nrockets > 5)
//   nrockets = 1; */
//  playertext[1] = nrockets + '0';
//}
static void
playerchange ()
{
//	nrockets++;
	if (nrockets > 2)
	  nrockets = 2; 
	else if (nrockets < 1)
		nrockets = 1;
  playertext[1] = nrockets + '0';
  //DKS - next line fixed a bug when one player is playing in deathmatch mode 
  if (nrockets == 1)
	  gameplan = COOPERATIVE;
}

static void
levelchange ()
{
  sprintf (leveltext, "LEVEL  %03i ", lastlevel + 1);
}


static void
quit ()
{
#ifdef NETSUPPORT
  if (client)
    CQuit ("Selected quit in main menu");
#endif

  //DKS - added saving here:
  save_rc();

  uninitialize ();
  exit (0);
}

static void
fit_selector ()
{
  mtime = MENUTIME;
  mx1p = (XPOSITION (selected) - 2 - mx1) / mtime;
  mx2p = (XPOSITION1 (selected) + 1 - mx2) / mtime;
  my1p = (YPOSITION (selected) - 2 - my1) / mtime;
  my2p = (YPOSITION (selected) + 8 + 0 - my2) / mtime;
}

static void
to_main_menu ()
{
  save_rc ();
  main_menu ();
}

static void
change_menu ()
{
  static char     s[2][5][40], *s1;
  int             i;
#ifdef NETSUPPORT
  if (client)
    {
      menu[0].text = "START GAME";
      menu[0].func = start_game;
    }
  else
#endif
    {
      menu[0].text = "BACK TO MAIN MENU";
      menu[0].func = to_main_menu;
    }
  nnumbers = 0;
  for (i = 0; i < nrockets; i++)
    {
#ifdef NETSUPPORT
      if (control[i] == 0 && client)
	s1 = "REMOTE";
      else
#endif
#ifdef MOUSE
      if (i == mouseplayer)
	s1 = "MOUSE";
      else
#endif
#ifdef JOYSTICK
      if (i == joystickplayer[0])
	 {
	    if(joystickmul[0]==JOYMUL1)s1 = "JOYSTICK A button";
	    else s1 = "JOYSTICK A deflection";
	 }
      else if (i == joystickplayer[1])
	 {
	    if(joystickmul[1]==JOYMUL1)s1 = "JOYSTICK B button";
	    else s1 = "JOYSTICK B deflection";
	 }
      else
#endif
      if (rotation[i])
	s1 = "ROTATION KEYBOARD";
      else
	s1 = "KEYBOARD";
      sprintf (s[0][i], "PLAYER %i:%s", i + 1, s1);
#ifdef NETSUPPORT
      if (control[i] == 0 && client)
	sprintf (s[1][i], " ");
      else
#endif
#ifdef JOYSTICK
      if (i == joystickplayer[0])
	sprintf (s[1][i], "CALIBRATE");
      else if (i == joystickplayer[1])
	sprintf (s[1][i], "CALIBRATE");
      else
#endif
#ifdef MOUSE
      if (i == mouseplayer)
	sprintf (s[1][i], " ");
      else
#endif
	sprintf (s[1][i], "CHANGE KEYS");
      menu[i * 2 + 1].text = s[0][i];
      menu[i * 2 + 2].text = s[1][i];
      menu[i * 2 + 1].func = control_change;
      menu[i * 2 + 2].func = player_change;
    }
  nmenu = nrockets * 2 + 1;
  if (selected >= nmenu)
    selected = 0;
  fit_selector ();
}
static void
deathmatch ()
{
  gameplan = DEATHMATCH;
  main_menu ();
}

static void
cooperative ()
{
  gameplan = COOPERATIVE;
  main_menu ();
}

static void
setsound ()
{
#ifdef SOUND
//DKS disabled
//  if (sndinit)
//    ssound = !ssound;
#endif
  main_menu ();
}

void
setanalog ()
{
	analog_enabled ^= 1;
	to_main_menu();
}

void
setgsensor ()
{
	gsensor_enabled ^= 1;
	to_main_menu();
}


static void
setmusic ()
{
	toggle_music();
	to_main_menu();
}

static void
veryhard ()
{
  difficulty = 0;
  main_menu ();
}

static void
hard ()
{
  difficulty = 1;
  main_menu ();
}

static void
medium ()
{
  difficulty = 2;
  main_menu ();
}

static void
easy ()
{
  difficulty = 3;
  main_menu ();
}

static void
veryeasy ()
{
  difficulty = 4;
  main_menu ();
}

static void
change_mode ()
{
	//DKS
	if (nrockets == 1) {
		gamemode = COOPERATIVE;
		return;
	}
  nnumbers = 0;
  //DKS
//  menu[0].text = "DEATH MATCH(DOOM)";
  menu[0].text = "DEATH MATCH";
  menu[0].func = deathmatch;
  menu[1].text = "COOPERATIVE";
  menu[1].func = cooperative;
  nmenu = 2;
  //DKS
//  selected = 1;
  selected = (gameplan==COOPERATIVE) ? 1 : 0;
  fit_selector ();
}

static void
change_obtiznost ()
{
  menu[0].text = "NIGHTMARE";
  menu[0].func = veryhard;
  menu[1].text = "HARD";
  menu[1].func = hard;
  menu[2].text = "MEDIUM";
  menu[2].func = medium;
  menu[3].text = "EASY";
  menu[3].func = easy;
  menu[4].text = "VERY EASY";
  menu[4].func = veryeasy;
  nnumbers = 0;
  nmenu = 5;
  //DKS - why does it it not select the current difficulty!?
//  selected = 2;
  selected = difficulty;
  fit_selector ();
}

#ifdef GCW
static void
nmain_menu ()
{
  nnumbers = 2;
  //DKS simplified the menu:
  nmenu = 9;
  menu[0].text = "START GAME";
  menu[0].func = start;
  menu[1].text = playertext;
  menu[1].func = playerchange;
  minim[0].x = XPOSITION (1);
  minim[0].y = YPOSITION (1);
  minim[0].line = 1;
  minim[0].number = &nrockets;
  //DKS
//  minim[0].max = 5;
  minim[0].max = 2;
  minim[0].min = 1;
  minim[0].func = playerchange;
  maxim[0].x = XPOSITION (1) + 8 * 2;
  maxim[0].y = YPOSITION (1);
  maxim[0].line = 1;
  maxim[0].number = &nrockets;
//  maxim[0].max = 5;
  maxim[0].max = 2;
  maxim[0].min = 1;
  maxim[0].func = playerchange;
  playerchange ();
  levelchange ();
  menu[2].text = leveltext;
//  menu[2].func = playerchange;
  menu[2].func = levelchange;
  minim[1].x = XPOSITION (2) + 7 * 7;
  minim[1].y = YPOSITION (2);
  minim[1].line = 2;
  minim[1].number = &lastlevel;
  minim[1].max = maxlevel;
  minim[1].min = 0;
  minim[1].func = levelchange;
  maxim[1].x = XPOSITION (2) + 11 * 7;
  maxim[1].y = YPOSITION (2);
  maxim[1].line = 2;
  maxim[1].number = &lastlevel;
  maxim[1].max = maxlevel;
  maxim[1].min = 0;
  maxim[1].func = levelchange;
  nnumbers = 2;
  menu[3].text = "DIFFICULTY";
  menu[3].func = change_obtiznost;
//  menu[4].text = "GAME MODE";
  menu[4].text = "MULTI-PLAYER GAME MODE";
  menu[4].func = change_mode;
  //DKS - awful hack to allow displaying of grayed-out text for this option
#define MENU_LINE_FOR_MULTIPLAYER_GAME_MODE 4
  menu[5].text = music_enabled ? "MUSIC: ENABLED" : "MUSIC: DISABLED";
  menu[5].func = setmusic;
	menu[6].text = analog_enabled ? "ANALOG STICK: ENABLED" : "ANALOG STICK: DISABLED";
	menu[6].func = setanalog;
	menu[7].text = gsensor_enabled ? "G-SENSOR: ENABLED" : "G-SENSOR: DISABLED";
	menu[7].func = setgsensor;
  menu[8].text = "QUIT";
  menu[8].func = quit;
  
  selected = 0;
  fit_selector ();
}
#else // non-GCW platform, i.e. this below is made for Dingoo A320 for now:
static void
nmain_menu ()
{
  nnumbers = 2;
  //DKS simplified the menu:
  nmenu = 7;
  menu[0].text = "START GAME";
  menu[0].func = start;
  menu[1].text = playertext;
  menu[1].func = playerchange;
  minim[0].x = XPOSITION (1);
  minim[0].y = YPOSITION (1);
  minim[0].line = 1;
  minim[0].number = &nrockets;
  //DKS
//  minim[0].max = 5;
  minim[0].max = 2;
  minim[0].min = 1;
  minim[0].func = playerchange;
  maxim[0].x = XPOSITION (1) + 8 * 2;
  maxim[0].y = YPOSITION (1);
  maxim[0].line = 1;
  maxim[0].number = &nrockets;
//  maxim[0].max = 5;
  maxim[0].max = 2;
  maxim[0].min = 1;
  maxim[0].func = playerchange;
  playerchange ();
  levelchange ();
  menu[2].text = leveltext;
//  menu[2].func = playerchange;
  menu[2].func = levelchange;
  minim[1].x = XPOSITION (2) + 7 * 7;
  minim[1].y = YPOSITION (2);
  minim[1].line = 2;
  minim[1].number = &lastlevel;
  minim[1].max = maxlevel;
  minim[1].min = 0;
  minim[1].func = levelchange;
  maxim[1].x = XPOSITION (2) + 11 * 7;
  maxim[1].y = YPOSITION (2);
  maxim[1].line = 2;
  maxim[1].number = &lastlevel;
  maxim[1].max = maxlevel;
  maxim[1].min = 0;
  maxim[1].func = levelchange;
  nnumbers = 2;
  menu[3].text = "DIFFICULTY";
  menu[3].func = change_obtiznost;
//  menu[4].text = "GAME MODE";
  menu[4].text = "MULTI-PLAYER GAME MODE";
  menu[4].func = change_mode;
  menu[5].text = music_enabled ? "MUSIC: ENABLED" : "MUSIC: DISABLED";
  menu[5].func = setmusic;
  menu[6].text = "QUIT";
  menu[6].func = quit;
  
  selected = 0;
  fit_selector ();
}
#endif

/***************************************************************************/
#ifdef NETSUPPORT
extern void     register_players ();
extern void     start_game ();
int             nplayers = 1, maxnplayers = 5;
static void
nplayerchange ()
{
  playertext[1] = nplayers + '0';
}
void
setnplayers (int n)
{
  nrockets = n;
  maxnplayers = MAXROCKETS - nrockets;
  if (minim[0].number == &nplayers)
    {
      minim[0].max = maxnplayers;
      if (nplayers > maxnplayers)
	nplayers = maxnplayers;
    }
  nplayerchange ();
}
void
cmenu2 (unsigned char *message, int size)
{
  printf ("entering change menu");
  change_menu ();
  gamemode = MENU;
}

static void
cmain_menu ()
{
  nnumbers = 1;
  nmenu = 4;
  menu[0].text = "REGISTER PLAYERS";
  menu[0].func = register_players;
  menu[1].text = playertext;
  menu[1].func = nplayerchange;
  minim[0].x = XPOSITION (1);
  minim[0].y = YPOSITION (1);
  minim[0].line = 1;
  minim[0].number = &nplayers;
  minim[0].max = maxnplayers;
  minim[0].min = 1;
  minim[0].func = nplayerchange;
  maxim[0].x = XPOSITION (1) + 8 * 2;
  maxim[0].y = YPOSITION (1);
  maxim[0].line = 1;
  maxim[0].number = &nplayers;
  maxim[0].max = maxnplayers;
  maxim[0].min = 1;
  maxim[0].func = nplayerchange;
  nplayerchange ();
#ifdef SOUND
//DKS
  //menu[2].text = ssound ? "SOUND ON" : (sndinit ? "SOUND OFF" : "SOUND OFF-NOT INITIALIZED");
    menu[2].text = ssound ? "SOUND AVAILABLE" : (sndinit ? "SOUND OFF" : "SOUND OFF-NOT INITIALIZED");
#else
  menu[2].text = "SOUND OFF(NOT AVAIABLE)";
#endif
  menu[2].func = setsound;
  menu[3].text = "QUIT";
  menu[3].func = quit;
  selected = 0;
  fit_selector ();
}
#endif
/***************************************************************************/

static void
main_menu ()
{
	//DKS - added to determine if we are actually playing a game or in the menu
	in_actual_game = 0;

#ifdef NETSUPPORT
  if (client)
    cmain_menu ();
  else
#endif
    nmain_menu ();
}

void
init_menu ()
{
  mx1 = 10;
  mx2 = MAPWIDTH - 10;
  my2 = 10;
  my2 = MAPHEIGHT - 10;
  ssound = sound;
  playertext[1] = nrockets + '0';
	
  //DKS - fixing bug starting 1-player game in deathmatch mode
  if (nrockets == 1) gamemode = COOPERATIVE;
  main_menu ();
}

//DKS - modified & replaced
//void
//draw_menu (CONST int draw)
//{
//  int             i;
//  sound = 0;
//  if (draw)
//    {
//      levelchange ();
//      DrawRectangle ((int) mx1, (int) my1, (int) mx2, (int) my2, ball (2));
//      DrawRectangle ((int) mx1 + 1, (int) my1 + 1, (int) mx2 + 1, (int) my2 + 1, ball (20));
//      DrawBlackMaskedText (MAPWIDTH / 2 - 8 * 4 + 1, 1, "THE GAME");
//      DrawBlackMaskedText (MAPWIDTH / 2 - 10 * 4 + 1, 11, "K O U L E S");
//      DrawBlackMaskedText (MAPWIDTH / 2 - 2 * 4 + 1, 21, "BY");
//      DrawBlackMaskedText (MAPWIDTH / 2 - 8 * 4 + 1, 31, "JAHUSOFT");
//      DrawBlackMaskedText (MAPWIDTH / 2 - 30 * 4 + 1, 46, "PRESS P FOR PAUSE / H FOR HELP");
//#ifdef __OS2__
//      DrawBlackMaskedText (MAPWIDTH / 2 - 28 * 4, 40, "OS/2 Warp VERSION BY T.A.K.K");
//#endif
//      DrawWhiteMaskedText (MAPWIDTH / 2 - 8 * 4, 0, "THE GAME");
//      DrawWhiteMaskedText (MAPWIDTH / 2 - 10 * 4, 10, "K O U L E S");
//      DrawWhiteMaskedText (MAPWIDTH / 2 - 2 * 4, 20, "BY");
//      DrawWhiteMaskedText (MAPWIDTH / 2 - 8 * 4, 30, "JAHUSOFT");
//      DrawWhiteMaskedText (MAPWIDTH / 2 - 30 * 4, 45, "PRESS P FOR PAUSE / H FOR HELP");
//#ifdef __OS2__
//      DrawWhiteMaskedText (MAPWIDTH / 2 - 28 * 4, 40, "OS/2 Warp VERSION BY T.A.K.K");
//#endif
//      for (i = 0; i < nnumbers; i++)
//	{
//	  Line (minim[i].x, minim[i].y + 3, minim[i].x + 6, minim[i].y + 3, ball (2));
//	  Line (minim[i].x + 3, minim[i].y + 0, minim[i].x, minim[i].y + 3, ball (2));
//	  Line (minim[i].x + 4, minim[i].y + 0, minim[i].x + 1, minim[i].y + 3, ball (2));
//	  Line (minim[i].x + 3, minim[i].y + 6, minim[i].x, minim[i].y + 3, ball (2));
//	  Line (minim[i].x + 4, minim[i].y + 6, minim[i].x + 1, minim[i].y + 3, ball (2));
//	  Line (maxim[i].x + 2, maxim[i].y + 3, maxim[i].x + 8, maxim[i].y + 3, ball (2));
//	  Line (maxim[i].x + 5, maxim[i].y + 0, maxim[i].x + 8, maxim[i].y + 3, ball (2));
//	  Line (maxim[i].x + 4, maxim[i].y + 0, maxim[i].x + 7, maxim[i].y + 3, ball (2));
//	  Line (maxim[i].x + 5, maxim[i].y + 6, maxim[i].x + 8, maxim[i].y + 3, ball (2));
//	  Line (maxim[i].x + 4, maxim[i].y + 6, maxim[i].x + 7, maxim[i].y + 3, ball (2));
//	}
//      for (i = 0; i < nmenu; i++)
//	{
//	  DrawBlackMaskedText ((int) XPOSITION (i) + 1, (int) YPOSITION (i) + 1, menu[i].text);
//	  DrawWhiteMaskedText ((int) XPOSITION (i), (int) YPOSITION (i), menu[i].text);
//	}
//    }
//  if (mtime)
//    {
//      mtime--;
//      my1 += my1p;
//      mx1 += mx1p;
//      my2 += my2p;
//      mx2 += mx2p;
//    }
//}

void
draw_menu (CONST int draw)
{
  in_actual_game = 0; // DKS - added to determine whether we're in the actual game or in the menu

  int             i;
  //DKS  Sound on by default
  //sound = 0;
  sound = 1;
  if (draw)
    {
      levelchange ();
      DrawRectangle (physicalscreen, (int) mx1, (int) my1, (int) mx2, (int) my2, ball (2));
      DrawRectangle (physicalscreen, (int) mx1 + 1, (int) my1 + 1, (int) mx2 + 1, (int) my2 + 1, ball (20));
      DrawBlackMaskedText (physicalscreen, MAPWIDTH / 2 - 8 * 4 + 1, 1, "THE GAME");
      DrawBlackMaskedText (physicalscreen, MAPWIDTH / 2 - 10 * 4 + 1, 11, "K O U L E S");
//      DrawBlackMaskedText (physicalscreen, MAPWIDTH / 2 - 11 * 4 + 1, 21, "BY JAHUSOFT");
      DrawBlackMaskedText (physicalscreen, MAPWIDTH / 2 - 14 * 4 + 1, 21, "By Jan Hubicka");
#ifdef GP2X
      DrawBlackMaskedText (physicalscreen, MAPWIDTH / 2 - 27 * 4 + 1, 41, "GP2X VERSION BY SENOR QUACK");
#elif DINGOO
      DrawBlackMaskedText (physicalscreen, MAPWIDTH / 2 - 27 * 4 + 1, 41, "DINGOO VERSION BY SENOR QUACK");
#elif GCW
      DrawBlackMaskedText (physicalscreen, MAPWIDTH / 2 - 35 * 4 + 1, 41, "SDL conversion & GCW port: senquack");
#endif

		//DKS - disabled help, it's pretty useless
//      DrawBlackMaskedText (physicalscreen, MAPWIDTH / 2 - 33 * 4 + 1, 46, "PRESS L FOR HELP / START TO PAUSE");
//      DrawBlackMaskedText (physicalscreen, MAPWIDTH / 2 - 20 * 4 + 1, 46, "PRESS START TO PAUSE");
      DrawBlackMaskedText (physicalscreen, MAPWIDTH / 2 - 27 * 4 + 1, 51, "Music: DST @ nosoapradio.us");
      DrawBlackMaskedText (physicalscreen, MAPWIDTH / 2 - 30 * 4 + 1, 61, "(Creative Commons 3.0 License)");

      DrawWhiteMaskedText (physicalscreen, MAPWIDTH / 2 - 8 * 4, 0, "THE GAME");
      DrawWhiteMaskedText (physicalscreen, MAPWIDTH / 2 - 10 * 4, 10, "K O U L E S");
//      DrawWhiteMaskedText (physicalscreen, MAPWIDTH / 2 - 11 * 4, 20, "BY JAHUSOFT");
      DrawWhiteMaskedText (physicalscreen, MAPWIDTH / 2 - 14 * 4, 20, "By Jan Hubicka");
#ifdef GP2X
      DrawWhiteMaskedText (physicalscreen, MAPWIDTH / 2 - 28 * 4, 40, "GP2X VERSION BY SENOR QUACK");
#elif DINGOO
      DrawWhiteMaskedText (physicalscreen, MAPWIDTH / 2 - 30 * 4, 40, "DINGOO VERSION BY SENOR QUACK");
#elif GCW
      DrawWhiteMaskedText (physicalscreen, MAPWIDTH / 2 - 35 * 4, 40, "SDL conversion & GCW port: senquack");
#endif
//      DrawWhiteMaskedText (physicalscreen, MAPWIDTH / 2 - 33 * 4, 45, "PRESS L FOR HELP / START TO PAUSE");
//      DrawWhiteMaskedText (physicalscreen, MAPWIDTH / 2 - 20 * 4, 46, "PRESS START TO PAUSE");
      DrawWhiteMaskedText (physicalscreen, MAPWIDTH / 2 - 27 * 4, 50, "Music: DST @ nosoapradio.us");
      DrawWhiteMaskedText (physicalscreen, MAPWIDTH / 2 - 30 * 4, 60, "(Creative Commons 3.0 License)");

		if (gsensor_enabled) {
			DrawBlackMaskedText (physicalscreen, MAPWIDTH / 2 - 31 * 4 + 1, 210 + 1, "PRESS L+R TO RE-CENTER G-SENSOR");
			DrawWhiteMaskedText (physicalscreen, MAPWIDTH / 2 - 31 * 4, 210, "PRESS L+R TO RE-CENTER G-SENSOR");
		}

      for (i = 0; i < nnumbers; i++)
	{
	  Line (physicalscreen, minim[i].x, minim[i].y + 3, minim[i].x + 6, minim[i].y + 3, ball (2));
	  Line (physicalscreen, minim[i].x + 3, minim[i].y + 0, minim[i].x, minim[i].y + 3, ball (2));
	  Line (physicalscreen, minim[i].x + 4, minim[i].y + 0, minim[i].x + 1, minim[i].y + 3, ball (2));
	  Line (physicalscreen, minim[i].x + 3, minim[i].y + 6, minim[i].x, minim[i].y + 3, ball (2));
	  Line (physicalscreen, minim[i].x + 4, minim[i].y + 6, minim[i].x + 1, minim[i].y + 3, ball (2));
	  Line (physicalscreen, maxim[i].x + 2, maxim[i].y + 3, maxim[i].x + 8, maxim[i].y + 3, ball (2));
	  Line (physicalscreen, maxim[i].x + 5, maxim[i].y + 0, maxim[i].x + 8, maxim[i].y + 3, ball (2));
	  Line (physicalscreen, maxim[i].x + 4, maxim[i].y + 0, maxim[i].x + 7, maxim[i].y + 3, ball (2));
	  Line (physicalscreen, maxim[i].x + 5, maxim[i].y + 6, maxim[i].x + 8, maxim[i].y + 3, ball (2));
	  Line (physicalscreen, maxim[i].x + 4, maxim[i].y + 6, maxim[i].x + 7, maxim[i].y + 3, ball (2));
	}
      for (i = 0; i < nmenu; i++)
	{
	  DrawBlackMaskedText (physicalscreen, (int) XPOSITION (i) + 1, (int) YPOSITION (i) + 1, menu[i].text);
	  // DKS - awful hack to allow some grayed-out menu options
#ifdef GCW
		if (i == MENU_LINE_FOR_MULTIPLAYER_GAME_MODE && nrockets == 1) {
		  DrawGrayMaskedText (physicalscreen, (int) XPOSITION (i), (int) YPOSITION (i), menu[i].text);
		} else {
		  DrawWhiteMaskedText (physicalscreen, (int) XPOSITION (i), (int) YPOSITION (i), menu[i].text);
		}
#else
	  DrawWhiteMaskedText (physicalscreen, (int) XPOSITION (i), (int) YPOSITION (i), menu[i].text);
#endif
	}

#ifdef GP2X
	//ADDED TO SHOW CURRENT VOLUME
	if (sound) {
    if ((int) cur_volume == 128) {
      sprintf(volstr, "VOLUME: %d (MAX)", (int) (cur_volume) );
    } else {
      sprintf(volstr, "VOLUME: %d", (int) (cur_volume) );
    }
    DrawBlackMaskedText(physicalscreen, 1 + 1, MAPHEIGHT - 10 + 1, volstr);
    DrawWhiteMaskedText(physicalscreen, 1, MAPHEIGHT - 10, volstr);
	}
#endif

    }
  if (mtime)
    {
      mtime--;
      my1 += my1p;
      mx1 += mx1p;
      my2 += my2p;
      mx2 += mx2p;
    }
}

static int      inctime, changed, waittime;
static void
increase (int i)
{
  changed = 1;
  if (waittime)
    {
      waittime--;
      return;
    }
  waittime = inctime;
  if (inctime > 1)
    inctime--;
  else
    (*maxim[i].number) += 2;
  (*maxim[i].number)++;
  if (*maxim[i].number > maxim[i].max)
    *maxim[i].number = maxim[i].max;
#ifdef SOUND
  if (ssound && inctime != 1)
    play_sound (S_CREATOR2);
#endif
  maxim[i].func ();
  //DKS disabled saving here
  //save_rc ();
}
static void
decrease (int i)
{
  changed = 1;
  if (waittime)
    {
      waittime--;
      return;
    }
  waittime = inctime;
  if (inctime > 1)
    inctime--;
  else
    (*maxim[i].number) -= 2;
  (*minim[i].number)--;
  if (*minim[i].number < minim[i].min)
    *minim[i].number = minim[i].min;
#ifdef SOUND
  if (ssound && inctime != 1)
    play_sound (S_CREATOR2);
#endif
  minim[i].func ();
  //DKS disabled saving here
  //save_rc ();
}

void
menu_keys ()
{
  static int      enter;
  int             ent = 0;
  static int	  esc;
#ifdef MOUSE
  static int      button;
  int             but = 0;
#endif
  if (maxim[1].max != maxlevel && !client)
    main_menu ();
  changed = 0;
  if (IsPressedLeft ())
    {
      int             i;
      for (i = 0; i < nnumbers; i++)
	if (maxim[i].line == selected)
	  decrease (i);
    }
  if (IsPressedRight ())
    {
      int             i;
      for (i = 0; i < nnumbers; i++)
	if (maxim[i].line == selected)
	  increase (i);
    }
#ifdef MOUSE
  but = nomouse ? 0 : MouseButtons ();
  if (but)
    {
      int             i;
      for (i = 0; i < nnumbers; i++)
	{
	  if (MouseX () > maxim[i].x &&
	      MouseX () < maxim[i].x + 8 &&
	      MouseY () > maxim[i].y &&
	      MouseY () < maxim[i].y + 8)
	    increase (i);
	  if (MouseX () > minim[i].x &&
	      MouseX () < minim[i].x + 8 &&
	      MouseY () > minim[i].y &&
	      MouseY () < minim[i].y + 8)
	    decrease (i);
	}
    }
  if (!but && button)
    {
      int             i;
      i = (MouseY () - MAPHEIGHT / 2 - 20 + 5 * nmenu) / 10;
      if (i >= 0 && i < nmenu)
	{
	  selected = i;
#ifdef SOUND
	  if (ssound)
	    play_sound (S_CREATOR2);
#endif
	  menu[selected].func ();
	  button = 0;
	  return;
	}
    }
  button = but;
#endif
  if (IsPressedEsc ())
    {
      if(!esc) {
			//DKS - disable quitting menu with SELECT
//      if (client || nnumbers == 2)
//	quit ();
      to_main_menu ();
      }
      esc=1;
    } else esc=0;
  if (IsPressedUp () && !mtime && selected > 0)
    selected--, fit_selector ();
  if (IsPressedDown () && !mtime && selected < nmenu - 1)
    selected++, fit_selector ();
  if (IsPressedEnter () && !mtime)
    ent = 1;
  if (!ent && enter)
    {
#ifdef SOUND
      if (ssound)
	play_sound (S_CREATOR2);
#endif
      menu[selected].func ();
    }
  enter = ent;
  if (!changed)
    inctime = 7, waittime = 0;

//DKS:
//#ifdef SOUND
#if defined(SOUND) && defined(GP2X)
  //DKS : note... no need for this vol control on Dingoo/GCW:
  //DKS ADDED FOR VOLUME CONTROL
  if(IsPressed(GP2X_BUTTON_VOLUP)) {
    if (cur_volume < 128) {
      cur_volume++;
      if (cur_volume > 128)
        cur_volume = 128;
      adjust_volume();
    }
  }
  //DKS ADDED FOR VOLUME CONTROL
  if(IsPressed(GP2X_BUTTON_VOLDOWN)) {
    if (cur_volume > 0) {
      // Finer-grained controls for headphone use below volume 10
      if (cur_volume > 10) {
        cur_volume--;
        if (cur_volume < 0)
          cur_volume = 0;
      } else {
        cur_volume -= .25;
        if (cur_volume < 0)
          cur_volume = 0;
      }
    }
//      if (cur_volume == 0)
//        sound = 0;
      adjust_volume();
    }
#endif
}

void
keys_keys ()
{

//DKS - Not sure what all this does, I think it's for redefining keys
//      through the menu.  Oh well, just disable it for now.

//  static int      lscan_code;
//  int             scan_code;
//  scan_code = GetKey ();
//  if (!scan_code && lscan_code)
//    {
//      keys[player][keynum] = lscan_code;
//      keynum++;
//#ifdef SOUND
//      if (ssound)
//	play_sound (S_CREATOR2);
//#endif
//      if (keynum > (rotation[player] ? 2 : 3))
//	{
//	  player++;
//	  keynum = 0;
//	  if (player > lastplayer)
//	    {
//	      change_menu ();
//	      gamemode = MENU;
//	    }
//	}
//    }
//  lscan_code = scan_code;

  change_menu();
  gamemode = MENU;

}


void
draw_keys (int draw)
{
	//DKS - no longer needed
//  char            s[20];
//  static char    *key[4] =
//  {"UP", "DOWN", "LEFT", "RIGHT"};
//  static char    *rkey[3] =
//  {"ACCELERATION", "ROTATE LEFT", "ROTATE RIGHT"};
//  if (!draw)
//    return;
//  nmenu = 2;
//  sprintf (s, "PLAYER:%i", player + 1);
//
//  //DKS
//  //DrawWhiteMaskedText ((int) (MAPWIDTH / 2 - 4 * strlen (s)), (int) YPOSITION (0), s);
//  DrawWhiteMaskedText (physicalscreen, (int) (MAPWIDTH / 2 - 4 * strlen (s)), (int) YPOSITION (0), s);
//
//  if (rotation[player])
//    sprintf (s, "KEY:%s", rkey[keynum]);
//  else
//    sprintf (s, "KEY:%s", key[keynum]);
//
//  //DKS
//  //DrawWhiteMaskedText ((int) (MAPWIDTH / 2 - 4 * strlen (s)), (int) YPOSITION (1), s);
//  DrawWhiteMaskedText (physicalscreen, (int) (MAPWIDTH / 2 - 4 * strlen (s)), (int) YPOSITION (1), s);
}

#ifdef JOYSTICK
void
joy_keys ()
{
  int             status, tmp;
  static int      lastbutton, button;
  if (read (joystickdevice[joystick], &js_data, JS_RETURN) <= 0)
    {
      perror ("Joystick");
      joystickplayer[joystick] = -2;
      gamemode = MENU;
      change_menu ();
      return;
    }
  button = lastbutton;
  lastbutton = js_data.buttons + IsPressedEnter ();
  if (joypos == 0)
    {
      if ((button == 0x00 || lastbutton != 0x00))
	return;
      joypos++;

      for (tmp = 0; js_data.x > 0xff; tmp++, js_data.x = js_data.x >> 1);
      js_data.x = tmp;
      for (tmp = 0; js_data.y > 0xff; tmp++, js_data.y = js_data.y >> 1);
      js_data.y = tmp;


      status = ioctl (joystickdevice[joystick], JS_SET_CAL, &js_data);
      if (status == -1)
	{
	  perror ("Joystick");
	}
      return;
    }
  if ((button == 0x00 || lastbutton != 0x00))
    return;

  calibrated[joystick] = 1;
  center[joystick][0] = js_data.x;
  center[joystick][1] = js_data.y;
  gamemode = MENU;
  change_menu ();
}


void
draw_joy (CONST int draw)
{
	//DKS - no longer needed
//  char            s[20];
//  static char    *mess[2] =
//  {"Move joystick to lower right corner", "Center joystick"};
//  if (!draw)
//    return;
//  nmenu = 4;
//  sprintf (s, "JOYSTICK:%i", joystick + 1);
//  DrawWhiteMaskedText ((int) (MAPWIDTH / 2 - 4 * strlen (s)), (int) YPOSITION (0), s);
//  DrawWhiteMaskedText ((int) (MAPWIDTH / 2 - 4 * strlen (mess[joypos])), (int) YPOSITION (1), mess[joypos]);
//  DrawWhiteMaskedText ((int) (MAPWIDTH / 2 - 4 * 33), (int) YPOSITION (2), "and press either button or enter");
//  sprintf (s, "x:%i y:%i", js_data.x, js_data.y);
//  DrawWhiteMaskedText ((int) (MAPWIDTH / 2 - 4 * strlen (s)), (int) YPOSITION (3), s);
//
}

#endif
