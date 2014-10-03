/*
 * sound.h - Platform Independant Sound Support - Dec. 1994
 *
 * Copyright 1994 Sujal M. Patel (smpatel@wam.umd.edu)
 * Conditions in "copyright.h"
 */

#if defined(SOUND) || defined(NAS_SOUND) || defined(RSOUND)

//DKS #ifdef __STDC__
void            init_sound ();	/* Init Sound System                          */
int             play_sound (int k);	/* Play a Sound                               */
//DKS void            maybe_play_sound (int k);	/* Play sound if the last 'k' sound_completed */
//DKS void            sound_completed (int k);	/* Complete a sound 'k'                       */
void            kill_sound ();	/* Terminate a sound unpredictably :)         */
//DKS #endif

//DKS - added:
void pause_sound(void);
void resume_sound(void);
extern int music_enabled;

//DKS - added music:
void init_music(void);
void free_music(void);
void start_music(void);
void pause_music(void);
void resume_music(void);
void stop_music(void);
void toggle_music(void);

#ifdef GP2X
//DKS added these two:
extern void adjust_volume ();  // adjust volume to cur_volume
extern float cur_volume; // 0-30 current volume setting
#endif

#endif /* SOUND */
