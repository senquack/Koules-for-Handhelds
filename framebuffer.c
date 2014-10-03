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
*  framebuffer.c fast 8 bit framebuffer bitmap creation    *
*                routines                                  *
***********************************************************/
#include "koules.h"


#define NCOLORS 32

#define HOLE_XCENTER (2*HOLE_RADIUS-3*HOLE_RADIUS/4)
#define HOLE_YCENTER (2*HOLE_RADIUS-HOLE_RADIUS/4)
#define HOLE_MAX_RADIUS (HOLE_RADIUS/DIV+0.5*HOLE_RADIUS/DIV)
#define HOLE_SIZE_MAX (radius*radius)

//DKS
//#ifndef NODIRECT

/*
 * hardcoded bitmap drawing routines
 */

//DKS - modified & replaced below
//static char    *
//draw_ball_bitmap (int radius, CONST int color)
//{
//  char           *bitmap = NULL, *point;
//  int             x, y, r;
//  radius /= DIV;
//  if ((bitmap = alloca ((radius * 2) * (radius * 2) + 2)) == NULL)
//    perror ("create_ball_bitmap"), exit (-1);
//  point = bitmap;
//  for (y = 0; y < radius * 2; y++)
//    for (x = 0; x < radius * 2; x++, point++)
//      {
//	if ((x - radius) * (x - radius) + (y - radius) * (y - radius)
//	    < (radius - 0.5) * (radius - 0.5))
//	  {
//	    r = (x - 3 * radius / 4) * (x - 3 * radius / 4) +
//	      (y - radius / 4) * (y - radius / 4);
//	    r = r * 32 / (1.5 * radius) / (1.5 * radius);
//	    if (r > 31)
//	      r = 31;
//	    *point = color + r;
//	  }
//	else
//	  *point = 0;
//      }
//  return (CompileBitmap (radius * 2, radius * 2, (char *) bitmap));
//}

//DKS - modified
static SDL_Surface *draw_ball_bitmap (int radius, const int color)
{
  SDL_Surface *newsurface = NULL;

  int             x, y, r;
  radius /= DIV;

  if ((newsurface = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_RLEACCEL | SDL_SRCCOLORKEY, radius * 2, radius * 2, physicalscreen->format->BitsPerPixel,
      physicalscreen->format->Rmask, physicalscreen->format->Gmask, physicalscreen->format->Bmask, physicalscreen->format->Amask)) == NULL)
    printf("create_ball_bitmap error\n"), exit (-1);

  if SDL_MUSTLOCK(newsurface)
    SDL_LockSurface(newsurface);

  for (y = 0; y < radius * 2; y++)
    for (x = 0; x < radius * 2; x++)
      {
	if ((x - radius) * (x - radius) + (y - radius) * (y - radius)
	    < (radius - 0.5) * (radius - 0.5))
	  {
	    r = (x - 3 * radius / 4) * (x - 3 * radius / 4) +
	      (y - radius / 4) * (y - radius / 4);
	    r = r * 32 / (1.5 * radius) / (1.5 * radius);
	    if (r > 31)
	      r = 31;
	    //*point = color + r;
	    SSetPixel(newsurface, x, y, color + r);
	  }
	else
	  SSetPixel(newsurface, x, y, 0);
      }

  if SDL_MUSTLOCK(newsurface)
    SDL_UnlockSurface(newsurface);

  //DKS
#ifdef GCW
  SDL_SetColorKey(newsurface, SDL_SRCCOLORKEY, colors_RGBA[0]);
#else
  SDL_SetColorKey(newsurface, SDL_SRCCOLORKEY, colors_16bit[0]);
#endif

  return newsurface;
}

//DKS - modified and replaced
//static char    *
//draw_reversed_ball_bitmap (int radius, CONST int color)
//{
//  char           *bitmap = NULL, *point;
//  int             x, y, r;
//  radius /= DIV;
//  if ((bitmap = alloca ((radius * 2) * (radius * 2) + 2)) == NULL)
//    perror ("create_ball_bitmap"), exit (-1);
//  point = bitmap;
//  for (y = 0; y < radius * 2; y++)
//    for (x = 0; x < radius * 2; x++, point++)
//      {
//	if ((x - radius) * (x - radius) + (y - radius) * (y - radius)
//	    < (radius - 0.5) * (radius - 0.5))
//	  {
//	    r = (x - 3 * radius / 4) * (x - 3 * radius / 4) +
//	      (y - radius / 4) * (y - radius / 4);
//	    r = r * 32 / (1.5 * radius) / (1.5 * radius);
//	    if (r > 31)
//	      r = 31;
//	    *point = color + 16 + r / 2;
//	  }
//	else
//	  *point = 0;
//      }
//  return (CompileBitmap (radius * 2, radius * 2, (char *) bitmap));
//}

//DKS - modified
static SDL_Surface    *
draw_reversed_ball_bitmap (int radius, CONST int color)
{
  SDL_Surface *newsurface = NULL;

  int             x, y, r;
  radius /= DIV;

  if ((newsurface = SDL_CreateRGBSurface(SDL_HWSURFACE  | SDL_RLEACCEL | SDL_SRCCOLORKEY,radius * 2, radius * 2, physicalscreen->format->BitsPerPixel,
      physicalscreen->format->Rmask, physicalscreen->format->Gmask, physicalscreen->format->Bmask, physicalscreen->format->Amask)) == NULL)
    printf("create_ball_bitmap error\n"), exit (-1);

  if SDL_MUSTLOCK(newsurface)
    SDL_LockSurface(newsurface);

  for (y = 0; y < radius * 2; y++)
    for (x = 0; x < radius * 2; x++)
      {
	if ((x - radius) * (x - radius) + (y - radius) * (y - radius)
	    < (radius - 0.5) * (radius - 0.5))
	  {
	    r = (x - 3 * radius / 4) * (x - 3 * radius / 4) +
	      (y - radius / 4) * (y - radius / 4);
	    r = r * 32 / (1.5 * radius) / (1.5 * radius);
	    if (r > 31)
	      r = 31;
      SSetPixel(newsurface, x, y, (color + 16 + r / 2));
	  }
	else
    SSetPixel(newsurface, x, y, 0);
      }

  if SDL_MUSTLOCK(newsurface)
    SDL_UnlockSurface(newsurface);

#ifdef SDL32BPP
  SDL_SetColorKey(newsurface, SDL_RLEACCEL | SDL_SRCCOLORKEY, colors_RGBA[0]);
#else
  SDL_SetColorKey(newsurface, SDL_RLEACCEL | SDL_SRCCOLORKEY, colors_16bit[0]);
#endif

  return newsurface;
}

//DKS - modified and replacede
//static char    *
//draw_apple_bitmap (int radius, CONST int color)
//{
//  char           *bitmap = NULL, *point;
//  int             x, y, r;
//  int             radius1;
//  radius /= DIV;
//  if ((bitmap = alloca ((radius * 2) * (radius * 2) + 2)) == NULL)
//    perror ("create_ball_bitmap"), exit (-1);
//  point = bitmap;
//  for (y = 0; y < radius * 2; y++)
//    for (x = 0; x < radius * 2; x++, point++)
//      {
//	if (DIV == 2)
//	  radius1 = radius * (abs (x - radius) / 2 + 25) / 30;
//	else
//	  radius1 = radius * (abs (x - radius) / 2 + 50) / 60;
//	if (radius1 > radius)
//	  radius1 = radius;
//	if ((x - radius) * (x - radius) + (y - radius) * (y - radius)
//	    < ((radius1) * (radius1)))
//	  {
//	    r = (x - 3 * radius / 4) * (x - 3 * radius / 4) +
//	      (y - radius / 4) * (y - radius / 4);
//	    r = 3 + r * 22 / (1.5 * radius1) / (1.5 * radius1);
//	    if (r > 31)
//	      r = 31;
//	    *point = color + r;
//	  }
//	else
//	  *point = 0;
//      }
//  return (CompileBitmap (radius * 2, radius * 2, (char *) bitmap));
//}

//DKS - modified
static SDL_Surface    *
draw_apple_bitmap (int radius, CONST int color)
{
  SDL_Surface *newsurface = NULL;

  int             x, y, r;
  int             radius1;
  radius /= DIV;

  if ((newsurface = SDL_CreateRGBSurface(SDL_HWSURFACE  | SDL_RLEACCEL | SDL_SRCCOLORKEY,radius * 2, radius * 2, physicalscreen->format->BitsPerPixel,
      physicalscreen->format->Rmask, physicalscreen->format->Gmask, physicalscreen->format->Bmask, physicalscreen->format->Amask)) == NULL)
    printf("create_ball_bitmap error\n"), exit (-1);

  if SDL_MUSTLOCK(newsurface)
    SDL_LockSurface(newsurface);
  for (y = 0; y < radius * 2; y++)
    for (x = 0; x < radius * 2; x++)
      {
	if (DIV == 2)
	  radius1 = radius * (abs (x - radius) / 2 + 25) / 30;
	else
	  radius1 = radius * (abs (x - radius) / 2 + 50) / 60;
	if (radius1 > radius)
	  radius1 = radius;
	if ((x - radius) * (x - radius) + (y - radius) * (y - radius)
	    < ((radius1) * (radius1)))
	  {
	    r = (x - 3 * radius / 4) * (x - 3 * radius / 4) +
	      (y - radius / 4) * (y - radius / 4);
	    r = 3 + r * 22 / (1.5 * radius1) / (1.5 * radius1);
	    if (r > 31)
	      r = 31;
      SSetPixel(newsurface, x, y, (color + r));
	  }
	else
    SSetPixel(newsurface, x, y, 0);
      }

  if SDL_MUSTLOCK(newsurface)
    SDL_UnlockSurface(newsurface);

#ifdef SDL32BPP
  SDL_SetColorKey(newsurface, SDL_RLEACCEL | SDL_SRCCOLORKEY, colors_RGBA[0]);
#else
  SDL_SetColorKey(newsurface, SDL_RLEACCEL | SDL_SRCCOLORKEY, colors_16bit[0]);
#endif

  return newsurface;
}

//DKS #else
//DKS static float    err = 0.0;

//DKS
//#ifdef XSUPPORT
//static INLINE int
//errdist (int c)
//{
//  float           sat, merr;
//  int             i;
//  int             p;
//  c &= 0xff;
//  sat = opixels[c] + err;
//  merr = sat - spixels[c];
//  p = c;
//  if (sat < spixels[c])
//    {
//      int             max = c / 32;
//      max = (max + 1) * 32;
//      for (i = c; i < max && i < 255; i++)
//	{
//	  if (fabs (merr) > fabs (spixels[i] - sat))
//	    {
//	      p = i;
//	      merr = sat - spixels[i];
//	    }
//	}
//    }
//  if (sat > spixels[c])
//    {
//      int             min = c / 32;
//      min *= 32;
//      for (i = c; i > min && i > 0; i--)
//	{
//	  if (fabs (merr) > fabs (spixels[i] - sat))
//	    {
//	      p = i;
//	      merr = sat - spixels[i];
//	    }
//	}
//    }
//  if (fabs (merr) > fabs (64 - sat))
//    {
//      p = 255;
//      merr = sat - 64;
//    }
//  if (fabs (merr) > fabs (sat))
//    {
//      p = 32;
//      merr = sat;
//    }
//  err = merr;
//  return (p);
//}
//DKS #else
#define errdist(c) c
//DKS#endif

//DKS
//static          BitmapType
//draw_ball_bitmap (int radius, CONST int color)
//{
//  RawBitmapType   bbitmap;
//  int             x, y, r, c;
//  radius /= DIV;
//  err = 0.0;
//  bbitmap = CreateBitmap ((radius * 2), (radius * 2));
//  for (y = 0; y < radius * 2; y++)
//    for (x = 0; x < radius * 2; x++)
//      {
//	if ((x - radius) * (x - radius) + (y - radius) * (y - radius)
//	    < (radius - 0.5) * (radius - 0.5))
//	  {
//	    r = (x - 3 * radius / 4) * (x - 3 * radius / 4) +
//	      (y - radius / 4) * (y - radius / 4);
//	    r = r * 32 / (1.5 * radius) / (1.5 * radius);
//	    if (r > 31)
//	      r = 31;
//	    c = errdist (color + r);
//	    BSetPixel (bbitmap, x, y, c);
//	  }
//	else
//	  {
//	    err = 0.0;
//	    BSetPixel (bbitmap, x, y, 0);
//	  }
//      }
//  return (CompileBitmap (radius * 2, radius * 2, bbitmap));
//}
//static          BitmapType
//draw_reversed_ball_bitmap (int radius, CONST int color)
//{
//  RawBitmapType   bbitmap;
//  int             x, y, r, c;
//  radius /= DIV;
//  err = 0.0;
//  bbitmap = CreateBitmap ((radius * 2), (radius * 2));
//  for (y = 0; y < radius * 2; y++)
//    for (x = 0; x < radius * 2; x++)
//      {
//	if ((x - radius) * (x - radius) + (y - radius) * (y - radius)
//	    < (radius - 0.5) * (radius - 0.5))
//	  {
//	    r = (x - 3 * radius / 4) * (x - 3 * radius / 4) +
//	      (y - radius / 4) * (y - radius / 4);
//	    r = r * 32 / (1.5 * radius) / (1.5 * radius);
//	    if (r > 31)
//	      r = 31;
//	    c = errdist (color + 16 + r / 2);
//	    BSetPixel (bbitmap, x, y, c);
//	  }
//	else
//	  {
//	    err = 0.0;
//	    BSetPixel (bbitmap, x, y, 0);
//	  }
//      }
//  return (CompileBitmap (radius * 2, radius * 2, bbitmap));
//}
//static          BitmapType
//draw_apple_bitmap (int radius, CONST int color)
//{
//  RawBitmapType   bitmap;
//  int             x, y, r, c;
//  int             radius1;
//  radius /= DIV;
//  bitmap = CreateBitmap ((radius * 2), (radius * 2));
//  err = 0;
//  for (y = 0; y < radius * 2; y++)
//    for (x = 0; x < radius * 2; x++)
//      {
//	if (DIV == 2)
//	  radius1 = radius * (abs (x - radius) / 2 + 25) / 30;
//	else
//	  radius1 = radius * (abs (x - radius) / 2 + 50) / 60;
//	if (radius1 > radius)
//	  radius1 = radius;
//	if ((x - radius) * (x - radius) + (y - radius) * (y - radius)
//	    < ((radius1) * (radius1)))
//	  {
//	    r = (x - 3 * radius / 4) * (x - 3 * radius / 4) +
//	      (y - radius / 4) * (y - radius / 4);
//	    r = 3 + r * 22 / (1.5 * radius1) / (1.5 * radius1);
//	    if (r > 31)
//	      r = 31;
//	    c = errdist (color + r);
//	    BSetPixel (bitmap, x, y, c);
//	  }
//	else
//	  {
//	    BSetPixel (bitmap, x, y, 0);
//	  }
//      }
//  return (CompileBitmap (radius * 2, radius * 2, bitmap));
//}
//#endif
//
//void
//create_bitmap ()
//{
//  int             x, y, r, po, radius;
//#ifndef NODIRECT
//  char            hole_data[HOLE_RADIUS * 2][HOLE_RADIUS * 2];
//  char            ehole_data[HOLE_RADIUS * 2][HOLE_RADIUS * 2];
//#else
//  int             c;
//  RawBitmapType   hole_data, ehole_data;
//#endif
//  printf ("creating bitmaps...\n");
//#ifndef NODIRECT
//  for (x = 0; x < HOLE_RADIUS * 2; x++)
//    for (y = 0; y < HOLE_RADIUS * 2; y++)
//      {
//	if (DIV == 1)
//	  radius = HOLE_RADIUS / 2 + (int) (atan (fabs (x - HOLE_RADIUS + 0.5) / fabs (y - HOLE_RADIUS + 0.5)) * HOLE_RADIUS / 2) % (HOLE_RADIUS / 2);
//	else
//	  radius = HOLE_RADIUS / 4;
//	if ((x - HOLE_RADIUS / DIV) * (x - HOLE_RADIUS / DIV) + (y - HOLE_RADIUS / DIV) * (y - HOLE_RADIUS / DIV)
//	    < radius * radius)
//	  {
//	    r = (x - HOLE_RADIUS / DIV) * (x - HOLE_RADIUS / DIV) +
//	      (y - HOLE_RADIUS / DIV) * (y - HOLE_RADIUS / DIV);
//	    r = r * 24 / HOLE_SIZE_MAX;
//	    if (r > 23)
//	      r = 23;
//	    hole_data[x][y] = 64 + r + 1;
//	    ehole_data[x][y] = 128 + r + 1;
//	  }
//	else
//	  hole_data[x][y] = 0,
//	    ehole_data[x][y] = 0;
//      }
//  hole_bitmap = (CompileBitmap (HOLE_RADIUS * 2, HOLE_RADIUS * 2, (char *) hole_data));
//  ehole_bitmap = (CompileBitmap (HOLE_RADIUS * 2, HOLE_RADIUS * 2, (char *) ehole_data));
//#else
//  ehole_data = CreateBitmap ((HOLE_RADIUS * 2), (HOLE_RADIUS * 2));
//  hole_data = CreateBitmap ((HOLE_RADIUS * 2), (HOLE_RADIUS * 2));
//  err = 0;
//  for (x = 0; x < HOLE_RADIUS * 2; x++)
//    for (y = 0; y < HOLE_RADIUS * 2; y++)
//      {
//	if (DIV == 1)
//	  radius = HOLE_RADIUS / 2 + (int) (atan (fabs (x - HOLE_RADIUS + 0.5) / fabs (y - HOLE_RADIUS + 0.5)) * HOLE_RADIUS / 2) % (HOLE_RADIUS / 2);
//	else
//	  radius = HOLE_RADIUS / 4;
//	if ((x - HOLE_RADIUS / DIV) * (x - HOLE_RADIUS / DIV) + (y - HOLE_RADIUS / DIV) * (y - HOLE_RADIUS / DIV)
//	    < radius * radius)
//	  {
//	    r = (x - HOLE_RADIUS / DIV) * (x - HOLE_RADIUS / DIV) +
//	      (y - HOLE_RADIUS / DIV) * (y - HOLE_RADIUS / DIV);
//	    r = r * 24 / HOLE_SIZE_MAX;
//	    if (r > 23)
//	      r = 23;
//	    c = errdist (64 + r + 1);
//	    BSetPixel (hole_data, x, y, c);
//	    c = errdist (128 + r + 1);
//	    BSetPixel (ehole_data, x, y, c);
//	  }
//	else
//	  {
//	    BSetPixel (hole_data, x, y, 0);
//	    BSetPixel (ehole_data, x, y, 0);
//	  }
//      }
//  ehole_bitmap = (CompileBitmap ((HOLE_RADIUS * 2), (HOLE_RADIUS * 2), ehole_data));
//  hole_bitmap = (CompileBitmap (HOLE_RADIUS * 2, HOLE_RADIUS * 2, hole_data));
//#endif
//
//  for (po = 0; po < MAXROCKETS; po++)
//    {
//      eye_bitmap[po] = draw_ball_bitmap (EYE_RADIUS, 32 + 32 * po);
//    }
//  ball_bitmap = draw_ball_bitmap (BALL_RADIUS, ball (0));
//  mouse_bitmap = draw_ball_bitmap (MOUSE_RADIUS * DIV, 32 + 32 * 2);
//  bball_bitmap = draw_ball_bitmap (BBALL_RADIUS, 4 * 32);
//  apple_bitmap = draw_apple_bitmap (APPLE_RADIUS, ball (0));
//  inspector_bitmap = draw_ball_bitmap (INSPECTOR_RADIUS, 160);
//  lunatic_bitmap = draw_reversed_ball_bitmap (LUNATIC_RADIUS, 192);
//  lball_bitmap[0] = draw_ball_bitmap (BALL_RADIUS, 4 * 32);
//  lball_bitmap[1] = draw_ball_bitmap (BALL_RADIUS, 5 * 32);
//  lball_bitmap[2] = draw_reversed_ball_bitmap (BALL_RADIUS, 192);
//  lball_bitmap[3] = draw_reversed_ball_bitmap (BALL_RADIUS, 0);
//  lball_bitmap[4] = draw_reversed_ball_bitmap (BALL_RADIUS, 3 * 32 - 5);
// /* lball_bitmap[5] = draw_reversed_ball_bitmap (BALL_RADIUS, 4 * 32 - 5);*/
//
//  for (x = 0; x < 5; x++)
//    rocket_bitmap[x] = draw_ball_bitmap (ROCKET_RADIUS, rocketcolor[x]);
//
//
//
//}

void
create_bitmap ()
{
  int             x, y, r, po, radius;
//#ifndef NODIRECT

  if ((hole_bitmap = SDL_CreateRGBSurface(SDL_HWSURFACE  | SDL_RLEACCEL | SDL_SRCCOLORKEY, (HOLE_RADIUS * 2 / DIV), (HOLE_RADIUS * 2 / DIV), physicalscreen->format->BitsPerPixel,
      physicalscreen->format->Rmask, physicalscreen->format->Gmask, physicalscreen->format->Bmask, physicalscreen->format->Amask)) == NULL)
    printf("create_bitmap() framebuffer.c error"), exit (-1);
  if ((ehole_bitmap = SDL_CreateRGBSurface(SDL_HWSURFACE  | SDL_RLEACCEL | SDL_SRCCOLORKEY,(HOLE_RADIUS * 2 / DIV), (HOLE_RADIUS * 2 / DIV), physicalscreen->format->BitsPerPixel,
      physicalscreen->format->Rmask, physicalscreen->format->Gmask, physicalscreen->format->Bmask, physicalscreen->format->Amask)) == NULL)
    printf("create_bitmap() framebuffer.c error"), exit (-1);

  if SDL_MUSTLOCK(hole_bitmap)
    SDL_LockSurface(hole_bitmap);

  // This code looks a lil' wacky.. - DKS
  // The original code made a bitmap twice as big as it needed
  // for cases where DIV = 2.  let's assume they meant otherwise
//  char            hole_data[HOLE_RADIUS * 2][HOLE_RADIUS * 2];
//  char            ehole_data[HOLE_RADIUS * 2][HOLE_RADIUS * 2];
//#else

  int             c;
  //RawBitmapType   hole_data, ehole_data;
//#endif
  printf ("creating bitmaps...\n");
//#ifndef NODIRECT
  for (x = 0; x < HOLE_RADIUS * 2 / DIV; x++)
    for (y = 0; y < HOLE_RADIUS * 2 / DIV; y++)
      {
	if (DIV == 1)
	  radius = HOLE_RADIUS / 2 + (int) (atan (fabs (x - HOLE_RADIUS + 0.5) / fabs (y - HOLE_RADIUS + 0.5)) * HOLE_RADIUS / 2) % (HOLE_RADIUS / 2);
	else
	  radius = HOLE_RADIUS / 4;
	if ((x - HOLE_RADIUS / DIV) * (x - HOLE_RADIUS / DIV) + (y - HOLE_RADIUS / DIV) * (y - HOLE_RADIUS / DIV)
	    < radius * radius)
	  {
	    r = (x - HOLE_RADIUS / DIV) * (x - HOLE_RADIUS / DIV) +
	      (y - HOLE_RADIUS / DIV) * (y - HOLE_RADIUS / DIV);
	    r = r * 24 / HOLE_SIZE_MAX;
	    if (r > 23)
	      r = 23;
//	    hole_data[x][y] = 64 + r + 1;
//	    ehole_data[x][y] = 128 + r + 1;
  SSetPixel(hole_bitmap, x, y, 64 + r + 1);

	  }
	else
	  SSetPixel(hole_bitmap, x, y, 0);
//      hole_data[x][y] = 0,
//	    ehole_data[x][y] = 0;
    }

  if SDL_MUSTLOCK(hole_bitmap)
    SDL_UnlockSurface(hole_bitmap);


  if SDL_MUSTLOCK(ehole_bitmap)
    SDL_LockSurface(ehole_bitmap);


  for (x = 0; x < HOLE_RADIUS * 2 / DIV; x++)
    for (y = 0; y < HOLE_RADIUS * 2 / DIV; y++)
      {
	if (DIV == 1)
	  radius = HOLE_RADIUS / 2 + (int) (atan (fabs (x - HOLE_RADIUS + 0.5) / fabs (y - HOLE_RADIUS + 0.5)) * HOLE_RADIUS / 2) % (HOLE_RADIUS / 2);
	else
	  radius = HOLE_RADIUS / 4;
	if ((x - HOLE_RADIUS / DIV) * (x - HOLE_RADIUS / DIV) + (y - HOLE_RADIUS / DIV) * (y - HOLE_RADIUS / DIV)
	    < radius * radius)
	  {
	    r = (x - HOLE_RADIUS / DIV) * (x - HOLE_RADIUS / DIV) +
	      (y - HOLE_RADIUS / DIV) * (y - HOLE_RADIUS / DIV);
	    r = r * 24 / HOLE_SIZE_MAX;
	    if (r > 23)
	      r = 23;
//	    hole_data[x][y] = 64 + r + 1;
//	    ehole_data[x][y] = 128 + r + 1;
  SSetPixel(ehole_bitmap, x, y, 128 + r + 1);
	  }
	else
	  SSetPixel(ehole_bitmap, x, y, 0);
//      hole_data[x][y] = 0,
//	    ehole_data[x][y] = 0;
    }

  if SDL_MUSTLOCK(ehole_bitmap)
    SDL_UnlockSurface(ehole_bitmap);

#ifdef SDL32BPP
  SDL_SetColorKey(hole_bitmap, SDL_RLEACCEL | SDL_SRCCOLORKEY, colors_RGBA[0]);
  SDL_SetColorKey(ehole_bitmap, SDL_RLEACCEL | SDL_SRCCOLORKEY, colors_RGBA[0]);
#else
  SDL_SetColorKey(hole_bitmap, SDL_RLEACCEL | SDL_SRCCOLORKEY, colors_16bit[0]);
  SDL_SetColorKey(ehole_bitmap, SDL_RLEACCEL | SDL_SRCCOLORKEY, colors_16bit[0]);
#endif

//  hole_bitmap = (CompileBitmap (HOLE_RADIUS * 2, HOLE_RADIUS * 2, (char *) hole_data));
//  ehole_bitmap = (CompileBitmap (HOLE_RADIUS * 2, HOLE_RADIUS * 2, (char *) ehole_data));
//#else
//  ehole_data = CreateBitmap ((HOLE_RADIUS * 2), (HOLE_RADIUS * 2));
//  hole_data = CreateBitmap ((HOLE_RADIUS * 2), (HOLE_RADIUS * 2));
//  err = 0;
//  for (x = 0; x < HOLE_RADIUS * 2; x++)
//    for (y = 0; y < HOLE_RADIUS * 2; y++)
//      {
//	if (DIV == 1)
//	  radius = HOLE_RADIUS / 2 + (int) (atan (fabs (x - HOLE_RADIUS + 0.5) / fabs (y - HOLE_RADIUS + 0.5)) * HOLE_RADIUS / 2) % (HOLE_RADIUS / 2);
//	else
//	  radius = HOLE_RADIUS / 4;
//	if ((x - HOLE_RADIUS / DIV) * (x - HOLE_RADIUS / DIV) + (y - HOLE_RADIUS / DIV) * (y - HOLE_RADIUS / DIV)
//	    < radius * radius)
//	  {
//	    r = (x - HOLE_RADIUS / DIV) * (x - HOLE_RADIUS / DIV) +
//	      (y - HOLE_RADIUS / DIV) * (y - HOLE_RADIUS / DIV);
//	    r = r * 24 / HOLE_SIZE_MAX;
//	    if (r > 23)
//	      r = 23;
//	    c = errdist (64 + r + 1);
//	    BSetPixel (hole_data, x, y, c);
//	    c = errdist (128 + r + 1);
//	    BSetPixel (ehole_data, x, y, c);
//	  }
//	else
//	  {
//	    BSetPixel (hole_data, x, y, 0);
//	    BSetPixel (ehole_data, x, y, 0);
//	  }
//      }
//  ehole_bitmap = (CompileBitmap ((HOLE_RADIUS * 2), (HOLE_RADIUS * 2), ehole_data));
//  hole_bitmap = (CompileBitmap (HOLE_RADIUS * 2, HOLE_RADIUS * 2, hole_data));
//#endif

  for (po = 0; po < MAXROCKETS; po++)
    {
      eye_bitmap[po] = draw_ball_bitmap (EYE_RADIUS, 32 + 32 * po);
    }
  ball_bitmap = draw_ball_bitmap (BALL_RADIUS, ball (0));
  mouse_bitmap = draw_ball_bitmap (MOUSE_RADIUS * DIV, 32 + 32 * 2);
  bball_bitmap = draw_ball_bitmap (BBALL_RADIUS, 4 * 32);
  apple_bitmap = draw_apple_bitmap (APPLE_RADIUS, ball (0));
  inspector_bitmap = draw_ball_bitmap (INSPECTOR_RADIUS, 160);
  lunatic_bitmap = draw_reversed_ball_bitmap (LUNATIC_RADIUS, 192);
  lball_bitmap[0] = draw_ball_bitmap (BALL_RADIUS, 4 * 32);
  lball_bitmap[1] = draw_ball_bitmap (BALL_RADIUS, 5 * 32);
  lball_bitmap[2] = draw_reversed_ball_bitmap (BALL_RADIUS, 192);
  lball_bitmap[3] = draw_reversed_ball_bitmap (BALL_RADIUS, 0);
  lball_bitmap[4] = draw_reversed_ball_bitmap (BALL_RADIUS, 3 * 32 - 5);
 /* lball_bitmap[5] = draw_reversed_ball_bitmap (BALL_RADIUS, 4 * 32 - 5);*/

  for (x = 0; x < 5; x++)
    rocket_bitmap[x] = draw_ball_bitmap (ROCKET_RADIUS, rocketcolor[x]);
}

// Modified - DKS
#ifndef SDL32BPP
static void
createbackground ()

// DKS - just can't get this to work, color 41 will suffice.
{
//
///* Create fancy dark red background */
//  int             x, y;
//
//
////DKS - new
//  Uint8 tmp_bitmap[MAPWIDTH * (MAPHEIGHT + 20)];
//
//  // Lock surface for direct access
//  if SDL_MUSTLOCK(background)
//    SDL_LockSurface(background);
//
//  for (y = 0; y < MAPHEIGHT + 20; y++)
//    for (x = 0; x < MAPWIDTH; x++)
//      {
//        int             i = 0;
//        int             n = 0;
//        int             c;
//        if (x > 0)
//          {
//            //DKS
//            //i += SGetPixel (tmp_surface, x - 1, y);
//            i += tmp_bitmap[(x - 1) + (y * MAPWIDTH)];
//            n++;
//          }
//        if (y > 0)
//          {
//            //DKS
//            //i += SGetPixel (tmp_surface, x, y - 1);
//            i += tmp_bitmap[ x + ((y - 1) * MAPWIDTH)];
//            n++;
//          }
//        c = (i + (rand () % 16)) / (n + 1);
//        //DEBUG printf("c: %d\b", c);
//        if (c > 9)
//          c = 9;
//        //DKS
//        //SSetPixel (tmp_surface, x, y, back (0) + c);
//        tmp_bitmap[ x + (y * MAPWIDTH)] = (Uint8) (back (0) + c);
//      }
//
//  //DKS - new
//  for (y = 0; y < MAPHEIGHT + 20; y++)
//    for (x = 0; x < MAPWIDTH; x++)
//      {
//        //DEBUG printf("%d\n", tmp_bitmap[x + (y * MAPWIDTH)]);
//        SSetPixel (background, x, y, tmp_bitmap[x + (y * MAPWIDTH)]);
//        //SSetPixel (background, x, y, 41);
//      }
//
//  if SDL_MUSTLOCK(background)
//    SDL_UnlockSurface(background);
//
//  SDL_SetColorKey(background, 0, 0);

//#ifdef GCW
//  SDL_FillRect(background, NULL, colors_RGBA[41]);
//#else
	// Generic ugly blue background for 16bpp platforms.. When I get to doing another 16bpp port, I'll adapt the
	//		plasma code below this #ifdef to work on 16bpp too..
  SDL_FillRect(background, NULL, colors_16bit[41]);
//#endif
}
#else
// On 32BPP SDL platforms, we'll do a fancy palette-rotating plasma background to spice things up:
static unsigned int palette[256];

static void generate_palette(void)
{
	unsigned int value = 0;
	for (int i = 0; i < 128; i++) {
		palette[i] = ((value>>1) << 16) ^ (value>>1);	// magenta
		value += 1; 
	}
	for (int i=128; i < 256; i++) {
		palette[i] = ((value>>1) << 16) ^ (value>>1);	// magenta
		value -= 1; 
	}
}

#define Plasma_SSetPixel(passed_surface, x, y, color) (((Uint32 *)(passed_surface->pixels))[ ( (y) * passed_surface->w ) + (x) ] = (Uint32) (color))
// pretty plasma-generating routine by DKS:
static void
createbackground ()
{
	const int w = background->w;
	const int h = background->h;
	generate_palette();
	if SDL_MUSTLOCK(background)
		SDL_LockSurface(background);
	srand(time(NULL));
	int circle_centerx = rand() % w;
	int circle_centery = rand() % h;
	int circle_centerx2 = rand() % w;
	int circle_centery2 = rand() % h;
	int circle_centerx3 = rand() % w;
	int circle_centery3 = rand() % h;
	float circle_freq = rand() % 30 + 25;
	float circle_freq2 = rand() % 30 + 35;
	float circle_freq3 = rand() % 30 + 45;

	for(int y = 0; y < h; y++)
		for(int x = 0; x < w; x++)
		{
 			unsigned int color = (int)( 
				+ 128.0 + (128.0 * cos(sqrt((x - circle_centerx) * (x - circle_centerx) + (y - circle_centery) * (y - circle_centery)) / circle_freq))
				+ 128.0 + (128.0 * cos(sqrt((x - circle_centerx2) * (x - circle_centerx2) + (y - circle_centery2) * (y - circle_centery2)) / circle_freq2))
				+ 128.0 + (128.0 * cos(sqrt((x - circle_centerx3) * (x - circle_centerx3) + (y - circle_centery3) * (y - circle_centery3)) / circle_freq3))
				) / 3;
			color &= 0xFF;
			unsigned int color32 = (color << 16 ^  color );
			Plasma_SSetPixel(background, x, y, color32);
		}
	if SDL_MUSTLOCK(background)
		SDL_UnlockSurface(background);
}

void copybackground(SDL_Surface *src, SDL_Surface *dest)
{
	const float idx_increment = 1.0;	// how much palette changes each call
	static float palette_idx = 0;
	unsigned int local_palette[256];

	if ((int)palette_idx > 255)
		palette_idx = 0;
	unsigned int local_idx = (int)palette_idx;
	for (unsigned int ctr = 0; ctr < 256; ctr++) {
		if (local_idx > 255)
			local_idx = 0;
		local_palette[ctr] = palette[local_idx++];
	}
	
	if SDL_MUSTLOCK(src)
		SDL_LockSurface(src);
	if SDL_MUSTLOCK(dest)
		SDL_LockSurface(dest);
	
	unsigned int *srcpixel = src->pixels;
	unsigned int *dstpixel = dest->pixels;
	unsigned int srcw = src->w;
	unsigned int srch = src->h;
	for (int y = 0; y < srch; y++) 
		for (int x = 0; x < srcw; x++) {
			*dstpixel++ = local_palette[(*srcpixel++) & 0xFF];
		}	
	palette_idx += idx_increment;

	if SDL_MUSTLOCK(src)
		SDL_UnlockSurface(src);
	if SDL_MUSTLOCK(dest)
		SDL_UnlockSurface(dest);
}
#endif

//DKS
void drawbottom_bar()
{
	if (bottom_bar) {
//		SDL_FillRect(bottom_bar, NULL, 0xFF000000);
#ifdef SDL32BPP
		SDL_FillRect(bottom_bar, NULL, SDL_MapRGBA(physicalscreen->format,0,0,0,255));
#else
      SDL_FillRect(bottom_bar, NULL, SDL_MapRGB(physicalscreen->format,0,0,0));
#endif
		SDL_SetAlpha(bottom_bar, SDL_SRCALPHA, 170);
		SDL_Rect dest_rect = {.x=0, .y=MAPHEIGHT, .w=320, .h=320-MAPHEIGHT};
		SDL_BlitSurface(bottom_bar, NULL, physicalscreen, &dest_rect);
	}
}

void
drawbackground ()
{				/*int i; */
/* Build up background from map data */

  //DKS
  //SetScreen (background);
  //ClearScreen ();
  createbackground ();
  //EnableClipping ();
//  //DKS
//  //Line (0, MAPHEIGHT, MAPWIDTH - 1, MAPHEIGHT, back (16));
//
//  Line (background, 0, MAPHEIGHT, MAPWIDTH - 1, MAPHEIGHT, back (16));
  //DisableClipping ();
}


//DKS - modified
void
drawstarbackground ()
{
/* Create fancy dark red background */
  int             x;
  int             x1, y1, c1;

  /* DKS
  SetScreen (starbackground);
  ClearScreen ();
  */
  ClearScreen(starbackground);


  // lock surface for pixel access
  if SDL_MUSTLOCK(starbackground)
    SDL_LockSurface(starbackground);

  for (x = 0; x < 700 / DIV / DIV; x++)
    {
      x1 = rand () % MAPWIDTH;
      y1 = rand () % (MAPHEIGHT + 20);
      c1 = rand () % 32 + 192;

      SSetPixel (starbackground, x1, y1, c1);
    }

 // unlock surface
  if SDL_MUSTLOCK(starbackground)
    SDL_UnlockSurface(starbackground);


}
