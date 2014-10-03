//DKS - modified
/***********************************************************
*                      K O U L E S                         *
*----------------------------------------------------------*
*  C1995 JAHUSOFT                                          *
*        Jan Hubicka                                       *
*        Dukelskych Bojovniku 1944                         *
*        390 03 Tabor                                      *
*        Czech Republic                                    *
*        Phone: 0041-361-32613                             *
*        eMail: hubicka@paru.cas.cz                        *
*----------------------------------------------------------*
*   Copyright(c)1995,1996 by Jan Hubicka.See README for    *
*                     Liecnece details.                    *
*----------------------------------------------------------*
*  cmap.c cmap routines for 256color librarys-svgalib style*
************************************************************
* Modified by DKS                                          *
***********************************************************/

#include "koules.h"

// Added by DKS: (originally cmap.c had no header file)
#include "cmap.h"

// These new arrays I added to koules are included via koules.h:
// they are necessary for using 16bpp surfaces which have no palette
// and I'm not gonna go changing code that uses palettes throughout
// if I don't have to.
//extern Uint32   colors_RGBA[256];  //Stores full RGBA representation of palette for use with SDL_gfx routines
//extern Uint16   colors_16bit[256]; //Stores 16-bit RGB representation of original palette for standard SDL pixel access on 16bpp machines

//DKS - modified
//static INLINE int
//col (int p, CONST float p1)
//{
//  p *= p1;
//  if (p > 63)
//    return (63);
//  if (p < 0)
//    return (0);
//  return (p);
//}

//DKS - modified this function to return values within 0-255 instead
// of the svgalib's 0-63 values.  I had to futz with it to return
// values sufficiently higher than 0 so SDL's RGBMap doesn't
// map any colors to 0, 0, 0 which would mess up blits.  If we ever
// move this code to a 32-bit SDL version, we might have to alter it
// as it's only been tested with 16-bit on GP2X.

static INLINE int
col (int p, const float p1)
{

  p *= p1;

  if (p > 63) {
    return(63*4 + 1);
  }

  if (p < 1) {
    return (8);
  }

  return (p*4+4);
}

int             fadedout = 0;


// DKS - Added


// DKS - replaced w/ modified version
//void
//setcustompalette (CONST int p, CONST float p1)
//{
//  /* 0-31 black to yellow for starwars scroller */
//  /* 32-63    black to red */
//  /* 64-96    for red koules */
//  /* 96-128   for yellow rockets */
//  /* 128-160   for green rockets */
//  /* 160-192   for blue rockets */
//  /* 192-256   gray cmap for stars */
//  Palette         pal;
//  int             i;
//  for (i = 0; i < 64; i++)
//    {
//      int             r, g, b;
//      r = g = b = 0;
//      if ((i & 32) > 0)
//	b = (i & 31) << 1;
//      if (i < 32)
//	{
//	  r = (i & 3) << 4;	/* 2 bits */
//	  g = (i & 4) << 3;	/* 1 bit */
//	  b = (i & 24) << 1;	/* 2 bits */
//	}
//      pal.color[i].red = col (r + p, p1);
//      pal.color[i].green = col (g + p, p1);
//      pal.color[i].blue = col (b + p, p1);
//    }
//  for (i = 64; i < 64 + 32; i++)
//    {
//      int             r, g, b;
//      r = g = b = 0;
//      r = (32 - (i - 63)) << 1;
//      if (i < 64 + 8)
//	{
//	  b = g = (((8 - (i - 63))) << 5) / 5;
//	}
//      pal.color[i].red = col (r + p, p1);
//      pal.color[i].green = col (g + p, p1);
//      pal.color[i].blue = col (b + p, p1);
//    }
//  for (i = 96; i < 96 + 32; i++)
//    {
//      int             r, g, b;
//      r = g = b = 0;
//      r = g = (32 - (i - 95)) << 1;
//      if (i < 96 + 8)
//	{
//	  b = ((8 - (i - 95))) << 3;
//	}
//      pal.color[i].red = col (r + p, p1);
//      pal.color[i].green = col (g + p, p1);
//      pal.color[i].blue = col (b + p, p1);
//    }
//  for (i = 128; i < 128 + 32; i++)
//    {
//      int             r, g, b;
//      r = g = b = 0;
//      g = (32 - (i - 127)) << 1;
//      if (i < 128 + 8)
//	{
//	  r = b = ((8 - (i - 127))) << 3;
//	}
//      pal.color[i].red = col (r + p, p1);
//      pal.color[i].green = col (g + p, p1);
//      pal.color[i].blue = col (b + p, p1);
//    }
//  for (i = 160; i < 160 + 32; i++)
//    {
//      int             r, g, b;
//      r = g = b = 0;
//      b = (32 - (i - 159)) << 1;
//      if (i < 160 + 8)
//	{
//	  r = g = (((8 - (i - 159))) << 3) / 2;
//	}
//      pal.color[i].red = col (r + p, p1);
//      pal.color[i].green = col (g + p, p1);
//      pal.color[i].blue = col (b + p, p1);
//    }
//  for (i = 0; i < 32; i++)
//    {
//      pal.color[i].red = col (i * 2 + p, p1);
//      pal.color[i].green = col (i * 2 + p, p1);
//      pal.color[i].blue = col (p, p1);
//    }
//  for (i = 0; i < 32; i++)
//    {
//      pal.color[192 + i].red = col (i * 2 + p, p1);
//      pal.color[192 + i].green = col (i * 2 + p, p1);
//      pal.color[192 + i].blue = col (i * 2 + p, p1);
//    }
//  pal.color[0].red = 0;
//  pal.color[0].green = 0;
//  pal.color[0].blue = 0;
//  pal.color[255].red = col (64 + p, p1);
//  pal.color[255].green = col (64 + p, p1);
//  pal.color[255].blue = col (64 + p, p1);
//  WaitRetrace ();
//  SetPalette (&pal);
//}


//DKS - modified - this function assumes the screen *physicalscreen has
//  already been initialized so we can map colors from it to our palette.
void
setcustompalette (const int p, const float p1)
{
  /* 0-31 black to screenyellow for starwars scroller */
  /* 32-63    black to red */
  /* 64-96    for red koules */
  /* 96-128   for yellow rockets */
  /* 128-160   for green rockets */
  /* 160-192   for blue rockets */
  /* 192-256   gray cmap for stars */

	//DKS - alignment on ARM/MIPS
  Uint32 r, g, b;

  r = g = b = 0;

  int             i;
  for (i = 0; i < 64; i++)
    {
      r = g = b = 0;
      if ((i & 32) > 0)
	b = (i & 31) << 1;
      if (i < 32)
	{
	  r = (i & 3) << 4;	/* 2 bits */
	  g = (i & 4) << 3;	/* 1 bit */
	  b = (i & 24) << 1;	/* 2 bits */
	}

		//DKS - ok, this probably worked great under ARM but I am changing it to be safer under a real 32-bit screen
		//		(no unit to test on)
#ifdef SDL32BPP
      colors_RGBA[i] = SDL_MapRGBA(physicalscreen->format, col (r + p, p1),  col (g + p, p1) , col (b + p, p1), 255);
#else
      colors_16bit[i] = SDL_MapRGB(physicalscreen->format, col (r + p, p1),  col (g + p, p1) , col (b + p, p1));
      colors_RGBA[i] = ((Uint32)col(r + p, p1) << 24) + ((Uint32)col(g + p, p1) << 16) + ((Uint32)col (b + p, p1) << 8) + 0x000000FF;
#endif
    }
  
  for (i = 64; i < 64 + 32; i++)
    {
      r = g = b = 0;
      r = (32 - (i - 63)) << 1;
      if (i < 64 + 8)
	{
	  b = g = (((8 - (i - 63))) << 5) / 5;
	}
#ifdef SDL32BPP
      colors_RGBA[i] = SDL_MapRGBA(physicalscreen->format, col (r + p, p1),  col (g + p, p1) , col (b + p, p1), 255);
#else
      colors_16bit[i] = SDL_MapRGB(physicalscreen->format, col (r + p, p1),  col (g + p, p1) , col (b + p, p1));
      colors_RGBA[i] = ((Uint32)col(r + p, p1) << 24) + ((Uint32)col(g + p, p1) << 16) + ((Uint32)col (b + p, p1) << 8) + 0x000000FF;
#endif
  }

  for (i = 96; i < 96 + 32; i++)
    {
      r = g = b = 0;
      r = g = (32 - (i - 95)) << 1;
      if (i < 96 + 8)
	{
	  b = ((8 - (i - 95))) << 3;
	}
#ifdef SDL32BPP
      colors_RGBA[i] = SDL_MapRGBA(physicalscreen->format, col (r + p, p1),  col (g + p, p1) , col (b + p, p1), 255);
#else
      colors_16bit[i] = SDL_MapRGB(physicalscreen->format, col (r + p, p1),  col (g + p, p1) , col (b + p, p1));
      colors_RGBA[i] = ((Uint32)col(r + p, p1) << 24) + ((Uint32)col(g + p, p1) << 16) + ((Uint32)col (b + p, p1) << 8) + 0x000000FF;
#endif
    }
  for (i = 128; i < 128 + 32; i++)
    {
      r = g = b = 0;
      g = (32 - (i - 127)) << 1;
      if (i < 128 + 8)
	{
	  r = b = ((8 - (i - 127))) << 3;
	}
#ifdef SDL32BPP
      colors_RGBA[i] = SDL_MapRGBA(physicalscreen->format, col (r + p, p1),  col (g + p, p1) , col (b + p, p1), 255);
#else
      colors_16bit[i] = SDL_MapRGB(physicalscreen->format, col (r + p, p1),  col (g + p, p1) , col (b + p, p1));
      colors_RGBA[i] = ((Uint32)col(r + p, p1) << 24) + ((Uint32)col(g + p, p1) << 16) + ((Uint32)col (b + p, p1) << 8) + 0x000000FF;
#endif
    }
  for (i = 160; i < 160 + 32; i++)
    {
      r = g = b = 0;
      b = (32 - (i - 159)) << 1;
      if (i < 160 + 8)
	{
	  r = g = (((8 - (i - 159))) << 3) / 2;
	}
#ifdef SDL32BPP
		colors_RGBA[i] = SDL_MapRGBA(physicalscreen->format, col (r + p, p1),  col (g + p, p1) , col (b + p, p1), 255);
#else
      colors_16bit[i] = SDL_MapRGB(physicalscreen->format, col (r + p, p1),  col (g + p, p1) , col (b + p, p1));
      colors_RGBA[i] = ((Uint32)col(r + p, p1) << 24) + ((Uint32)col(g + p, p1) << 16) + ((Uint32)col (b + p, p1) << 8) + 0x000000FF;
#endif
    }
  for (i = 0; i < 32; i++)
    {
#ifdef SDL32BPP
      colors_RGBA[i] = SDL_MapRGBA(physicalscreen->format, col (i * 2 + p, p1), col (i * 2 + p, p1), col (p, p1), 255);
#else
      colors_16bit[i] = SDL_MapRGB(physicalscreen->format, col (i * 2 + p, p1), col (i * 2 + p, p1), col (p, p1));
      colors_RGBA[i] = ((Uint32)col(i * 2 + p, p1) << 24) + ((Uint32)col(i * 2 + p, p1) << 16) + ((Uint32)col(p, p1) << 8) + 0x000000FF;
#endif
    }
  for (i = 0; i < 32; i++)
    {
#ifdef SDL32BPP
      colors_RGBA[192 + i] = SDL_MapRGBA(physicalscreen->format, col (i * 2 + p, p1), col (i * 2 + p, p1), col (i * 2 + p, p1), 255);
#else
      colors_16bit[192 + i] = SDL_MapRGB(physicalscreen->format, col (i * 2 + p, p1), col (i * 2 + p, p1), col (i * 2 + p, p1));
      colors_RGBA[192 + i] = ((Uint32)col(i * 2 + p, p1) << 24) + ((Uint32)col(i * 2 + p, p1) << 16) + ((Uint32)col(i * 2 + p, p1) << 8) + 0x000000FF;
#endif
    }
#ifdef SDL32BPP
  colors_RGBA[0] = SDL_MapRGBA(physicalscreen->format, 0,0,0,255);
  colors_RGBA[255] = SDL_MapRGBA(physicalscreen->format, col (64 + p, p1), col (64 + p, p1), col (64 + p, p1),255);
#else
  colors_16bit[0] = 0;
  colors_RGBA[0] = 0x000000FF;
  colors_16bit[255] = SDL_MapRGB(physicalscreen->format, col (64 + p, p1), col (64 + p, p1), col (64 + p, p1));
  colors_RGBA[255] = ((Uint32)col(64 + p, p1) << 24) + ((Uint32)col(64 + p, p1) << 16) + ((Uint32)col(64 + p, p1) << 8) + 0x000000FF;
#endif
}


// DKS - added to replace old fading routines
/*
  Fading with alphablending
  (c) Patrick Kooman, 2002
  contact: patrick@2dgame-tutorial.com
*/
/* Fades the given surface in or out to the given screen within the given time
 If the image surface is the screen surface (pointer are equal), a copy is made
 first. We must do that because we are overwriting the Screen Surface. */
void fade (SDL_Surface* p_surf_screen, SDL_Surface* p_surf_img, Uint32 ui_seconds, int b_fade_out) {
  /* Becomes the black surface */
  SDL_Surface* p_surf_black = NULL ;
  /* Used when the Screen Surface equals the Image Surface */
  SDL_Surface* p_surf_screen_copy = NULL ;
  /* Used to calculate the steps to make a fade in the given time: */
  Uint32 ui_old_time, ui_curr_time, ui_time_ms ;
  float f_alpha ;
  /* Becomes flag to pass when creating a Surface */
  Uint32 ui_flags = SDL_SRCALPHA ;
  /* Create the black surface in the format of the given screen */
  if ((p_surf_screen->flags & SDL_HWSURFACE)) {
    ui_flags |= SDL_HWSURFACE ;
  }
  else {
    ui_flags |= SDL_SWSURFACE ;
  }
  if ((p_surf_black = SDL_CreateRGBSurface (ui_flags,
    p_surf_screen->w, p_surf_screen->h,
    p_surf_screen->format->BitsPerPixel,
    p_surf_screen->format->Rmask, p_surf_screen->format->Gmask, p_surf_screen->format->Bmask,
    p_surf_screen->format->Amask)) == NULL) {
      /* Replace this line with you own error handling / logging */
      fprintf (stderr, "fade: could not create the black Surface. (%s)\n", SDL_GetError ()) ;
      return ;
  }
  /* Fill the Surface with black */
  SDL_FillRect (p_surf_black, NULL, SDL_MapRGB (p_surf_screen->format, 0, 0, 0)) ;
  /* Should we make a copy of the Screen? */
  if (p_surf_screen == p_surf_img) {
    if ((p_surf_screen_copy = SDL_CreateRGBSurface (ui_flags,
    p_surf_screen->w, p_surf_screen->h,
    p_surf_screen->format->BitsPerPixel,
    p_surf_screen->format->Rmask, p_surf_screen->format->Gmask, p_surf_screen->format->Bmask,
    p_surf_screen->format->Amask)) == NULL) {
      /* Replace this line with you own error handling / logging */
      fprintf (stderr, "fade: could not create a copy of the Screen Surface. (%s)\n", SDL_GetError ()) ;
      SDL_FreeSurface (p_surf_black) ;
      return ;
    }
    /* Ok, copy surfaces and set surfact pointer */
    SDL_BlitSurface (p_surf_screen, NULL, p_surf_screen_copy, NULL) ;
    p_surf_img = p_surf_screen_copy ;
  }
  /* Ok, we are now ready for the fade. These are the steps (looped):
  1. Draw p_surf_img onto p_surf_screen, just an ordinary blit.
  2. Decrease or increase (depends on fading in or out) the alpha value,
    based on the elapsed time since the previous loop-iteration.
  3. Draw p_surf_black onto p_surf_screen in the current alpha value.*/
  ui_old_time = SDL_GetTicks () ;
  ui_curr_time = ui_old_time ;
  /* Convert the given time in seconds into miliseconds. */
  ui_time_ms = ui_seconds * 1000 ;
  if (b_fade_out) {
    f_alpha = 0.0 ;
    /* Loop until the alpha value exceeds 255 (this is the maximum alpha value) */
    while (f_alpha < 255.0) {
      /* Draw the image onto the screen */
      SDL_BlitSurface (p_surf_img, NULL, p_surf_screen, NULL) ;
      /* Draw the black surface onto the screen */
      SDL_SetAlpha (p_surf_black, SDL_SRCALPHA, (Uint8) f_alpha) ;
      SDL_BlitSurface (p_surf_black, NULL, p_surf_screen, NULL) ;
      /* Update the timer variables */
      ui_old_time = ui_curr_time ;
      ui_curr_time = SDL_GetTicks () ;
      /* Flip the screen Surface */
      SDL_Flip (p_surf_screen) ;
      /* Calculate the next alpha value */
      f_alpha += 255 * ((float) (ui_curr_time - ui_old_time) / ui_time_ms) ;
    }
  }
  else {
    f_alpha = 255.0 ;
    /* Loop until the alpha value exceeds 255 */
    while (f_alpha > 0.0) {
      /* Draw the image onto the screen */
      SDL_BlitSurface (p_surf_img, NULL, p_surf_screen, NULL) ;
      /* Draw the black surface onto the screen */
      SDL_SetAlpha (p_surf_black, SDL_SRCALPHA, (Uint8) f_alpha) ;
      SDL_BlitSurface (p_surf_black, NULL, p_surf_screen, NULL) ;
      /* Update the timer variables */
      ui_old_time = ui_curr_time ;
      ui_curr_time = SDL_GetTicks () ;
      /* Flip the screen Surface */
      SDL_Flip (p_surf_screen) ;
      /* Calculate the next alpha value */
      f_alpha -= 255 * ((float) (ui_curr_time - ui_old_time) / ui_time_ms) ;
    }
  }
  /* Free the black Surface */
  SDL_FreeSurface (p_surf_black) ;
  /* Free the Screen copy, if used */
  if (p_surf_screen_copy != NULL) {
    SDL_FreeSurface (p_surf_screen_copy) ;
  }
}

// DKS - Replaced with one below
//void
//fadeout ()
//{
//  if (!fadedout)
//    {
//      float           i;
//      for (i = 1; i >= 0; i -= 0.1)
//	{
//	  setcustompalette (0, i);
//	  usleep (200), tbreak = 1;
//
//	}
//      setcustompalette (-65, 0);
//      fadedout = 1;
//    }
//}
void fadeout()
{
  if (!fadedout)
    {
      fade(physicalscreen, physicalscreen, 1, 1);
      fadedout = 1;
    }
}


//DKS - Gonna have to do something in other code to get the fade-in routines
//      to work right with SDL and screen modes without palettes..
//      I'll just leave these commented and have the two below them do nothing.
//
//void
//fadein ()
//{
//  if (fadedout)
//    {
//      float           i;
//      for (i = 0; i <= 1; i += 0.1)
//	{
//	  setcustompalette (0, i);
//	  usleep (200), tbreak = 1;
//
//	}
//      setcustompalette (0, 1);
//      fadedout = 0;
//    }
//}
//void
//fadein1 ()			/*better for star background */
//{
//  if (fadedout)
//    {
//      int             i;
//      for (i = -64; i <= 0; i += 6)
//	{
//	  setcustompalette (i, 1);
//	  usleep (200), tbreak = 1;
//
//	}
//      setcustompalette (0, 1);
//      fadedout = 0;
//    }
//}
void
fadein ()
{
      //UNTIL I FIX FADING IN, DO NOTHING
      if (fadedout) {
      //  printf("Fading in, but actually doing nothing, this code is a stub.\n");
        fadedout = 0;
      }
}
void
fadein1 ()			/*better for star background */
{
      //UNTIL I FIX FADING IN, DO NOTHING
      if (fadedout) {
        //printf("Fading in, but actually doing nothing, this code is a stub.\n");
        fadedout = 0;
      }
}
