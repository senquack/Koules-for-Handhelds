/*
 * sound.c - Platform Independant Sound Support - Apr. 1995
 *
 * Copyright 1994-1995 Sujal M. Patel (smpatel@wam.umd.edu)
 * Conditions in "copyright.h"
 *
 * Modified for koules and bugfixed by Jan Hubicka
 */
#ifdef SOUND

//DKS
//#include <stdio.h>
//#ifdef __STDC__
//#include <stdlib.h>
//#endif
//#include <unistd.h>
//#include <sys/time.h>
//#include <fcntl.h>
//#include <signal.h>
//#include <sys/stat.h>

//DKS
#include "SDL.h"
#include "SDL_mixer.h"
#include <stdio.h>
#include <stdlib.h>

//DKS from koules.c (gotta clean up header, can't include it or get failed compiles)
extern int sndinit;
extern int sound;

//DKS - pasted here from koules.h for reference.. these are the sound #'s
//#define S_START 0
//#define S_END 1
//#define S_COLIZE 2
//#define S_DESTROY_BALL 3
//#define S_DESTROY_ROCKET 4
//#define S_CREATOR1 5
//#define S_CREATOR2 6

//DKS
//char           *unixSoundPath = SOUNDDIR;
//char           *unixSoundDev = SOUNDDEV;

#define playSounds 1

//DKS - Added for SDL
#define NUM_SOUNDS 7
static Mix_Chunk *sounds[NUM_SOUNDS];  // Array holding our 7 sounds


#ifdef GP2X
// Volume gets set between 0-128
float cur_volume = 1;
#endif

//DKS
//static int      soundfd;
static Uint8 audioOK = 1;
//DKS static char     sound_flags[20];	/* Sound Flag for sound 1-19 */
//DKS int             child;

int music_enabled = 1;

////DKS - added music:
//#define NUM_SONGS	6
//static int cur_song = 0;
//static Mix_Music *songs[NUMSONGS];
//static char* song_names[NUM_SONGS] = {	"1_5.mp3", "2_0.mp3", "3_1.mp3", "4_0.mp3", "5_0.mp3", "6_0.mp3" };

//DKS - added music:
static Mix_Music *song = NULL;

static const char* songname = "sounds/music.ogg";

void init_music(void)
{
	if (audioOK)
	{
		printf("Initializing music.. Here is the list of available decoders:\n");
		// print music decoders available
		int i,max=Mix_GetNumMusicDecoders();
		for(i=0; i<max; ++i)
			printf("Music decoder %d is for %s\n",i, Mix_GetMusicDecoder(i));
		song = Mix_LoadMUS(songname);
//		Mix_VolumeMusic(MIX_MAX_VOLUME);
		Mix_VolumeMusic(80);
		if (song) {
			printf("Successfully loaded music file: %s\n", songname);
		} else {
			printf("Failed to load music file: %s\nMix Error:%s\n", songname,Mix_GetError());
		}
	}
}

void free_music(void)
{
	if (audioOK && song)
	{
		printf("Freeing music..\n");
		Mix_HaltMusic();
		Mix_FreeMusic(song);
	}
}

void start_music(void)
{
	if (music_enabled && audioOK && song)
	{
		if (Mix_PlayMusic(song, -1)==-1) {
			printf("Mix_FadeInMusic: %s\n", Mix_GetError());
			// well, there's no music, but most games don't break without music...
		}	
		Mix_VolumeMusic(MIX_MAX_VOLUME);
	}
}

void pause_music(void)
{
	if (audioOK)
		Mix_PauseMusic();
}

void resume_music(void)
{
	if (music_enabled && audioOK)
		Mix_ResumeMusic();
}

void stop_music(void)
{
	if (audioOK)
		Mix_HaltMusic();
}

void toggle_music(void)
{
	if (music_enabled)
	{
		music_enabled = 0;
		stop_music();
	} else {
		music_enabled = 1;
		start_music();
	}
}

//DKS - why do we need this??
//void
//test_sound ()
//{
//  if (audioOK)
//    if (kill (child, 0))
//      {
//	audioOK = 0;
//	close (soundfd);
//      };
//}

//void
//init_sound ()
//{
//  int             i, fd[2];
//  char           *argv[4];
//  char            filename[512];
//
//  signal (SIGCHLD, SIG_IGN);
//  signal (SIGPIPE, SIG_IGN);
//
//  if (unixSoundPath[0] == '?')
//    {
//      audioOK = 0;
//      return;
//    };
//
//  /*  Create a pipe, set the write end to close when we exec the sound server,
//     and set both (is the write end necessary?) ends to non-blocking   */
//  pipe (fd);
//  soundfd = fd[1];
//
//  if (!(child = fork ()))
//    {
//      close (fd[1]);
//      dup2 (fd[0], STDIN_FILENO);
//      close (fd[0]);
//      sprintf (filename, SOUNDSERVER);
//      argv[0] = filename;
//      argv[1] = unixSoundPath;
//      argv[2] = unixSoundDev;
//      argv[3] = NULL;
//      execvp (filename, argv);
//      fprintf (stderr, "Couldn't Execute Sound Server %s!\n", filename);
//      exit (0);
//    };
//  close (fd[0]);
//
//  for (i = 0; i < 19; i++)
//    sound_flags[i] = 0;
//}

//DKS - new
void adjust_volume() {
#ifdef GP2X
  if (audioOK)
    Mix_Volume(-1, (int) (cur_volume));
#endif
}

//DKS modified and replaced
void
init_sound ()
{

  //DKS - init SDL audio and check if it's OK.  audioOK defaults to 1
  // which means it's OK.
  if (SDL_InitSubSystem(SDL_INIT_AUDIO) == -1) {
    printf("Problem initializing SDL audio. Sound disabled.\n");
    audioOK = 0;
    return;
  } else {

    // Initialize SDL_mixer for GP2X, buffer is set lower than PC
    #ifdef GP2X
      if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, AUDIO_S16, MIX_DEFAULT_CHANNELS, 128) == -1) {
        audioOK = 0;
        return;
      }
    #else
      if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, AUDIO_S16, MIX_DEFAULT_CHANNELS, 1024) == -1) {
        audioOK = 0;
        return;
      }
    #endif

	int errors = 0;
	if (!(sounds[0] = Mix_LoadWAV("sounds/start.wav")))
	{
		errors++;
		printf("Error: Mix_LoadWAV: %s\n", Mix_GetError());
	}
	if (!(sounds[1] = Mix_LoadWAV("sounds/end.wav")))
	{
		errors++;
		printf("Error: Mix_LoadWAV: %s\n", Mix_GetError());
	}
	if (!(sounds[2] = Mix_LoadWAV("sounds/colize.wav")))
	{
		errors++;
		printf("Error: Mix_LoadWAV: %s\n", Mix_GetError());
	}
	if (!(sounds[3] = Mix_LoadWAV("sounds/destroy1.wav")))
	{
		errors++;
		printf("Error: Mix_LoadWAV: %s\n", Mix_GetError());
	}
	if (!(sounds[4] = Mix_LoadWAV("sounds/destroy2.wav")))
	{
		errors++;
		printf("Error: Mix_LoadWAV: %s\n", Mix_GetError());
	}
	if (!(sounds[5] = Mix_LoadWAV("sounds/creator1.wav")))
	{
		errors++;
		printf("Error: Mix_LoadWAV: %s\n", Mix_GetError());
	}
	if (!(sounds[6] = Mix_LoadWAV("sounds/creator2.wav")))
	{
		errors++;
		printf("Error: Mix_LoadWAV: %s\n", Mix_GetError());
	}

	if (errors) audioOK = 0;

//	 //DKS - for GCW, set Mixer channel volumes to less than max
#ifdef GCW
	Mix_Volume(-1,24); // The game's stock sounds are VERY loud.
#else
	Mix_Volume(-1,MIX_MAX_VOLUME/2);
#endif

    if (!audioOK) {
      printf("Problem loading WAVs from sound/ dir. Sound disabled.\n");
    } else {
      // OK, everything is fine, set the volume to our reasonable default
#ifdef GP2X
      adjust_volume();
#endif
    }
  }

  sndinit = audioOK;  //sndinit tells rest of program whether sound is functional
  sound = audioOK; // Disable sound in rest of program if necessary.

	if (audioOK)
		init_music();

  return;

}

//DKS
//int
//play_sound (k)
//     int             k;
//{
//  char            c;
//  test_sound ();
//  c = k;
//  if ((playSounds) && (audioOK))
//    write (soundfd, &c, sizeof (c));
//  return 0;
//}

//DKS
void play_sound (int k)
{
  //test_sound ();

  if (audioOK) {
    Mix_PlayChannel(-1, sounds[k], 0);
  }

  return;
}


//DKS - do we need this??
//void
//maybe_play_sound (k)
//     int             k;
//{
//  char            c;
//
//  if (sound_flags[k] & 1)
//    return;
//
//  sound_flags[k] |= 1;
//  test_sound ();
//  c = (unsigned char) (k);
//  if ((playSounds) && (audioOK))
//    write (soundfd, &c, sizeof (c));
//}


//DKS - do we need this?
//void
//sound_completed (k)
//     int             k;
//{
//  sound_flags[k] &= ~1;
//}


//void
//kill_sound ()
//{
//  signed char            c;
//
//  c = -1;
//  test_sound ();
//  if ((playSounds) && (audioOK))
//    write (soundfd, &c, sizeof (c));
//}

//DKS - new
void pause_sound() {
	Mix_Pause(-1);
}

//DKS - new
void resume_sound() {
	Mix_Resume(-1);
}


//DKS - rewrote for SDL
void kill_sound () {
  Mix_CloseAudio();
  Mix_FreeChunk(sounds[0]);
  Mix_FreeChunk(sounds[1]);
  Mix_FreeChunk(sounds[2]);
  Mix_FreeChunk(sounds[3]);
  Mix_FreeChunk(sounds[4]);
  Mix_FreeChunk(sounds[5]);
  Mix_FreeChunk(sounds[6]);
	free_music();
  return;
}

#endif
