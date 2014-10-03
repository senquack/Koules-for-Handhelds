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
*  cmap.h cmap routines for 256color librarys-svgalib style*
************************************************************
* Modified for SDL by DKS - cmap.h didn't originally have  *
*     a header file                                        *
***********************************************************/

#ifndef CMAP_H
#define CMAP_H

extern int             fadedout;

//DKS oopsie, I think the original dev screwed this declaration up:
//extern void setcustompalette ();
extern void setcustompalette (const int, const float);

extern void fade (SDL_Surface*, SDL_Surface*, Uint32, int);
extern void fadeout();
extern void fadein();
extern void fadein1();

#endif


