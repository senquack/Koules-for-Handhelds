//DKS
#ifndef SDL_INTERFACE_H
#define SDL_INTERFACE_H
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
*  interface.h   interface for SDL                         *
***********************************************************/
/* DKS
#include <vga.h>
#include <vgagl.h>
#include <vgakeyboard.h>
*/

// DKS
#include "SDL/SDL.h"
#include "SFont.h"
//DKS - gcw
#include "input.h"

//DKS We'll now use SDL_prims for our line-drawing code:
#include "SDL_prims.h"


#ifdef MOUSE
#include <vgamouse.h>
#endif

//DKS
//#include <input.h>
#include "input.h"

#define EYE_RADIUS (DIV==1?5:6)
#define MOUSE_RADIUS 4

//DKS #define SVGALIBSUPPORT


//DKS typedef char   *RawBitmapType;

//DKS typedef char   *BitmapType;


//DKS typedef GraphicsContext *VScreenType;


#ifndef PLATFORM_VARIABLES_HERE
extern int      GAMEWIDTH;
extern int      GAMEHEIGHT;
extern int      MAPWIDTH;
extern int      MAPHEIGHT;
extern int      DIV;

extern int	flipping;
//DKS extern int	page;

//DKS - need to use SFont now
//extern void    *fontblack;
//extern void    *fontwhite;
extern SFont_Font *fontblack;
extern SDL_Surface *fontblack_surface;
extern SFont_Font *fontwhite;
extern SDL_Surface *fontwhite_surface;
extern SFont_Font *fontgray;
extern SDL_Surface *fontgray_surface;

/*DKS - modified
extern VScreenType physicalscreen;
extern VScreenType backscreen;
extern VScreenType background;
extern VScreenType starbackground;
*/
extern SDL_Surface *physicalscreen;
extern SDL_Surface *background;
extern SDL_Surface *starbackground;
extern SDL_Surface *bottom_bar;

//DKS - had to add these declarations to fix compiler problems
//Added by DKS - custom palette representations (8-bit game converted to palette-less format)
extern Uint32   colors_RGBA[256];  //Stores full RGBA representation of palette for both 32bpp display and gfx primitives use
#ifndef SDL32BPP
extern Uint16   colors_16bit[256]; //Stores 16-bit RGB representation of original
#endif
//                                   //palette for standard SDL pixel-level access

#ifdef GCW
extern int analog_enabled;	// This can be toggled on and off through main menu
extern int gsensor_enabled; // Ditto..
extern int rumble_enabled;	// Ditto (rumble support waiting on Zear's libhaptic being completed)
extern int gsensor_recently_recentered;	// This signals code in koules.c to display message confirming recentering of g-sensor
#endif


#endif


//DKS
//#define Truecolor 0
//#define FastAccess 1

//DKS #define BitmapToBuffer(bitmap) (bitmap)

//DKS #define VScreenToBuffer(screen) (screen)->vbuf


//DKS #define SetPalette(palette) gl_setpalette (palette)

/* DKS
#define SSetPixel(x,y,color) gl_setpixel(x,y,color)
DKS */


// DKS - Two replacement macros for use w/ 16bpp SDL surfaces
//DKS - modified for 32bpp compat. i.e. gcw:
#ifdef SDL32BPP
#define SGetPixel(passed_surface, x, y) (((Uint32 *)(passed_surface->pixels))[ ( (y) * passed_surface->w ) + (x) ])
#define SSetPixel(passed_surface, x, y, color) (((Uint32 *)(passed_surface->pixels))[ ( (y) * passed_surface->w ) + (x) ] = (Uint32) (colors_RGBA[(color)]))
#else
#define SGetPixel(passed_surface, x, y) (((Uint16 *)(passed_surface->pixels))[ ( (y) * passed_surface->w ) + (x) ])
#define SSetPixel(passed_surface, x, y, color) (((Uint16 *)(passed_surface->pixels))[ ( (y) * passed_surface->w ) + (x) ] = (Uint16) (colors_16bit[(color)]))
#endif

////DKS - new for GCW: support taking screenshot with START button
//extern void take_screenshot (void);

/* DKS
#if 0
#if DIV==1
#define BSetPixel(bitmap,x,y,color) *(BitmapToBuffer(bitmap)+(x)+(((y)&0xffffff00)>>1)+\
                                (((y)&0xffffff00)<<1))=color
#define SMySetPixel(screen,x,y,color) *(VScreenToBuffer(screen)+(x)+(((y)&0xffffff00)>>1)+\
                                (((y)&0xffffff00)<<1))=color
DKS - was disabled anyway */

/*#define SSetPixel(x,y,color) *((currentcontext.vbuf)+(x)+(y)*MAPWIDTH) )=color */

/* DKS
#else
#define BSetPixel(bitmap,x,y,color) *(BitmapToBuffer(bitmap)+(x)+((y)&0xffffff00)+(((y)\
                                &0xffffff00)>>2))=color
DKS - was disabled anyway */

/*#define SSetPixel(x,y,color) *((currentcontext.vbuf)+(x)+((y)*MAPWIDTH))=color */

/* DKS
#define SMySetPixel(screen,x,y,color) *(VScreenToBuffer(screen)+(x)+((y)&0xffffff00)+(((y)\
                                &0xffffff00)>>2))=color
DKS */


/*mysetpixel is fast setpixel for drawing movers y=realy*256 */

/* DKS - these aren't necessary, we can just use previos SSetPixel and
    screen contexts aren't necessary

#endif
#else
#define BSetPixel(bitmap,x,y,color) *(BitmapToBuffer(bitmap)+(x)+(y)*MAPWIDTH)=color
#define SMySetPixel(screen,x,y,color) *(VScreenToBuffer(screen)+(x)+((y)>>8)*MAPWIDTH)=color
#endif
#define SetScreen(screen) gl_setcontext (screen)
DKS */

//DKS - modified & replaced
//extern inline void
//DrawBlackMaskedText (int x, int y, char *text)
//{
//  gl_setwritemode (WRITEMODE_MASKED);
//  gl_setfont (8, 8, fontblack);
//  gl_write (x, y, text);
//}
//extern inline void
//DrawWhiteMaskedText (int x, int y, char *text)
//{
//  gl_setwritemode (WRITEMODE_MASKED);
//  gl_setfont (8, 8, fontwhite);
//  gl_write (x, y, text);
//}
//extern inline void
//DrawText (int x, int y, char *text)
//{
//  gl_setwritemode (WRITEMODE_OVERWRITE);
//  gl_setfont (8, 8, fontwhite);
//  gl_write (x, y, text);
//}

//DKS added as replacements:
#define DrawBlackMaskedText(passed_surface, x, y, text) \
  SFont_Write((passed_surface), fontblack, (x), (y), (text))

#define DrawWhiteMaskedText(passed_surface, x, y, text) \
  SFont_Write((passed_surface), fontwhite, (x), (y), (text))

#define DrawGrayMaskedText(passed_surface, x, y, text) \
  SFont_Write((passed_surface), fontgray, (x), (y), (text))

extern void DrawText (SDL_Surface *passed_surface, int x, int y, char *text);


//DKS - replaced w/ macro
//extern inline void
//DrawRectangle (int x1, int y1, int x2, int y2, int color)
//{
//  gl_line ((int) x1, (int) y1, (int) x1, (int) y2, color);
//  gl_line ((int) x1, (int) y2, (int) x2, (int) y2, color);
//  gl_line ((int) x2, (int) y2, (int) x2, (int) y1, color);
//  gl_line ((int) x2, (int) y1, (int) x1, (int) y1, color);
//}
//#define DrawRectangle(passed_surface, x1, y1, x2, y2, color) rectangleColor((passed_surface), (x1), (y1), (x2), (y2), colors_RGBA[(color)])
//DKS - actually, now it's a bit different
#define DrawRectangle(passed_surface, x1, y1, x2, y2, color) \
	sdl_rectangle((x1),(y1),(x2),(y2),(color))


//DKS
//extern inline char *
//CompileBitmap (int x, int y, char *bitmap)
//{
//  int             size1;
//  char           *cbitmap;
//  size1 = gl_compiledboxmasksize (x, y, bitmap);
//  if ((cbitmap = malloc (size1)) == NULL)
//    perror ("create_bitmap"), exit (1);
//  gl_compileboxmask (x, y, bitmap, cbitmap);
//  return (cbitmap);
//}

//DKS #define FillRectangle(x,y,x1,y1,color) gl_fillbox((x),(y),(x1),(y1),(color))
//DKS - doing away with SDL_gfx completely, using just SDL_FillRect now
//#define FillRectangle(passed_surface,x,y,x1,y1,color) boxColor(passed_surface, (x), (y), (x1), (y1), colors_RGBA[(color)])
#define FillRectangle(passed_surface,x,y,x1,y1,color) sdl_fillrect((passed_surface),(x),(y),(x1),(y1),(color))
extern void sdl_fillrect(SDL_Surface *passed_surface,int x, int y, int x1, int y1, int color);

//DKS #define Line(x1,y1,x2,y2,color) gl_line((x1),(y1),(x2),(y2),(color))
//DKS - changed to use SDL_prim library
#define Line(passed_surface,x1,y1,x2,y2,color) SDL_DrawLine((passed_surface),(x1),(y1),(x2),(y2),colors_RGBA[(color)])

//DKS
extern void sdl_rectangle(int x1, int y1, int x2, int y2, int color);

//DKS #define HLine(x1,y1,x2,color) gl_hline((x1),(y1),(x2),(color))
//DKS - changed to use SDL_prim library
#define HLine(passed_surface,x1,y1,x2,color) SDL_DrawHLine((passed_surface),(x1),(y1),(x2),colors_RGBA[(color)])

//DKS #define PutBitmap(x,y,xsize,ysize,bitmap) gl_putboxmaskcompiled((x),(y),(xsize),(ysize),(bitmap))
//DKS added replacement function:
extern void PutBitmap( int x, int y, SDL_Surface *source, SDL_Surface *destination);

//DKS #define WaitRetrace() vga_waitretrace()
//DKS #define EnableClipping() gl_enableclipping()
//DKS #define DisableClipping() gl_disableclipping()

extern void EnableClipping();

extern void DisableClipping();

//DKS #define ClearScreen() gl_clearscreen (0)
//DKS - modified for 32bpp compat. , i.e. GCW
#ifdef SDL32BPP
#define ClearScreen(passed_surface) SDL_FillRect( (passed_surface), NULL, colors_RGBA[0] )
#else
#define ClearScreen(passed_surface) SDL_FillRect( (passed_surface), NULL, colors_16bit[0] )
#endif

extern void flippage(void);

/* DKS
extern inline void
CopyVSToVS (VScreenType source, VScreenType destination)
{
  SetScreen (source);
  if(!flipping||(destination!=backscreen&&destination!=physicalscreen))
  gl_copyscreen (destination); else {
    gl_copyboxtocontext(0,0,MAPWIDTH,MAPHEIGHT+20,destination,0,0);
  }
}
*/
//DKS Added modified version:
extern void CopyVSToVS( SDL_Surface* source, SDL_Surface* destination);

//DKS replaced and modified
//extern inline void
//CopyToScreen (VScreenType source)
//{
//  SetScreen (source);
//  if(!flipping) gl_copyscreen (physicalscreen); else
//   flippage();
//}
//DKS - modified (this function i suppose is for copying back screens to main screen, might not
// need it
extern void CopyToScreen( SDL_Surface* source);

#define CharX 8
#define CharY 8
#define FAST_WIDELINE
//DKS:
#endif	// SDL_INTERFACE_H
