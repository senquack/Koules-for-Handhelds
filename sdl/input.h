//DKS
#ifndef SDL_INPUT_H
#define SDL_INPUT_H

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
*                     licence details.                     *
*----------------------------------------------------------*
*  interface.h   interface for gp2x                        *
***********************************************************/


//DKS
#ifdef GP2X
#define GP2X_BUTTON_UP              (0)
#define GP2X_BUTTON_DOWN            (4)
#define GP2X_BUTTON_LEFT            (2)
#define GP2X_BUTTON_RIGHT           (6)
#define GP2X_BUTTON_UPLEFT          (1)
#define GP2X_BUTTON_UPRIGHT         (7)
#define GP2X_BUTTON_DOWNLEFT        (3)
#define GP2X_BUTTON_DOWNRIGHT       (5)
#define GP2X_BUTTON_CLICK           (18)
#define GP2X_BUTTON_A               (12)
#define GP2X_BUTTON_B               (13)
#define GP2X_BUTTON_X               (15)
#define GP2X_BUTTON_Y               (14)
#define GP2X_BUTTON_L               (11)
#define GP2X_BUTTON_R               (10)
#define GP2X_BUTTON_START           (8)
#define GP2X_BUTTON_SELECT          (9)
#define GP2X_BUTTON_VOLUP           (16)
#define GP2X_BUTTON_VOLDOWN         (17)
#endif

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int SCREEN_BPP;

#define CUP 		0
#define CDOWN 		1
#define CLEFT		2
#define CRIGHT		3
#define CHELP		4
#define CPAUSE 	5 
#define CENTER		6
#define CESCAPE	7
#define CVOLUP		8	// not used on systems that don't require volume control
#define CVOLDOWN	9	// not used on systems that don't require volume control
#define CSSHOT		10	// screenshot command
#define CALTUP		11	// added another set of up/down/left/right controls for controlling
							//	Player 1 with 2 different modes (i.e. DPAD and A/B/X/Y buttons for ease of use)
							// -OR- Allowing two player mode on one device (i.e. DPAD is Player 1 and ABXY are Player 2)
#define CALTDOWN			12
#define CALTLEFT			13
#define CALTRIGHT			14
#define CANALOGUP			15
#define CANALOGDOWN 		16
#define CANALOGLEFT		17
#define CANALOGRIGHT		18
#define CGSENSORUP		19
#define CGSENSORDOWN		20
#define CGSENSORLEFT		21
#define CGSENSORRIGHT	22
#define CNUMCONTROLS		23

extern int control_state[CNUMCONTROLS];

extern int control_pressed(int control);
extern int any_control_pressed(void);
extern void control_update(void);

//DKS
#ifdef GP2X
#define IsPressedEnter() SDL_JoystickGetButton(joy, GP2X_BUTTON_B)
#define IsPressedUp() SDL_JoystickGetButton(joy, GP2X_BUTTON_UP)
#define IsPressedDown() SDL_JoystickGetButton(joy, GP2X_BUTTON_DOWN)
#define IsPressedLeft() SDL_JoystickGetButton(joy, GP2X_BUTTON_LEFT)
#define IsPressedRight() SDL_JoystickGetButton(joy, GP2X_BUTTON_RIGHT)
#define IsPressedEsc() SDL_JoystickGetButton(joy, GP2X_BUTTON_SELECT)
#define IsPressedH() SDL_JoystickGetButton(joy, GP2X_BUTTON_R)
#define IsPressedP() SDL_JoystickGetButton(joy, GP2X_BUTTON_L)
//#define IsPressed(button) SDL_JoystickGetButton(joy, (button))
//#define UpdateInput() SDL_JoystickUpdate()
#elif GCW
#define IsPressedEnter() control_pressed(CENTER)
#define IsPressedUp() control_pressed(CUP)
#define IsPressedDown() control_pressed(CDOWN)
#define IsPressedLeft() control_pressed(CLEFT)
#define IsPressedRight() control_pressed(CRIGHT)
#define IsPressedEsc() control_pressed(CESCAPE)
#define IsPressedH() control_pressed(CHELP)
#define IsPressedP() control_pressed(CPAUSE)
#define IsPressedScreenshot() control_pressed(CSSHOT)
#define IsPressedAltUp() control_pressed(CALTUP)
#define IsPressedAltDown() control_pressed(CALTDOWN)
#define IsPressedAltLeft() control_pressed(CALTLEFT)
#define IsPressedAltRight() control_pressed(CALTRIGHT)
#define IsPressedAnalogUp() control_pressed(CANALOGUP)
#define IsPressedAnalogDown() control_pressed(CANALOGDOWN)
#define IsPressedAnalogLeft() control_pressed(CANALOGLEFT)
#define IsPressedAnalogRight() control_pressed(CANALOGRIGHT)
#define IsPressedGsensorUp() control_pressed(CGSENSORUP)
#define IsPressedGsensorDown() control_pressed(CGSENSORDOWN)
#define IsPressedGsensorLeft() control_pressed(CGSENSORLEFT)
#define IsPressedGsensorRight() control_pressed(CGSENSORRIGHT)
#define UpdateInput() control_update()
#endif

// DKS - Is a button pushed?
#ifdef GP2X
extern inline int Pressed ()
{
    return (  SDL_JoystickGetButton(joy, GP2X_BUTTON_A) ||
              SDL_JoystickGetButton(joy, GP2X_BUTTON_B) ||
              SDL_JoystickGetButton(joy, GP2X_BUTTON_X) ||
              SDL_JoystickGetButton(joy, GP2X_BUTTON_Y) ||
              SDL_JoystickGetButton(joy, GP2X_BUTTON_CLICK) ||
              SDL_JoystickGetButton(joy, GP2X_BUTTON_SELECT) ||
              SDL_JoystickGetButton(joy, GP2X_BUTTON_START) ||
              SDL_JoystickGetButton(joy, GP2X_BUTTON_L) ||
              SDL_JoystickGetButton(joy, GP2X_BUTTON_R) );
}
#else
#define Pressed() any_control_pressed()
#endif

#endif // SDL_INPUT_H

