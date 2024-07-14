
#ifndef LIBASS_INCLUDED
#define LIBASS_INCLUDED


#define	MAX_SAMPLE		128
#define	MAX_CLONESAMPLE	16


#include "KSound.h"
#include "KMusic.h"
#include "KMiscTools.h"


typedef struct SNDOBJTAG
{
	int				nChannel;
	KSound			*bass_sample;

} SNDOBJ;



void	lb_Init();
void	lb_UnInit();
bool	lb_LoadSound(char *name, int nChannel, bool sound3D);
void	lb_FreeSound(int SoundID);
void	lb_PlaySound(int SoundID, int volume);
void	lb_StopSound( long	SoundID ) ;
bool	lb_PlayMP3(char *name, int volume, bool repeat);
bool	lb_IsMP3Playing();
void	lb_SetChannelVol(int volume);
void	lb_LoadMusic(char *name);
void	lb_UnLoadMusic();
void	lb_PlayMusic();
void	lb_StopMusic();
void	lb_ChangeMusicTrack();
void	lb_ReinitMusicTrack();
void	lb_MusicFadeOUT(float step);
void	lb_MusicFadeIN(float step);
void	lb_SetMusicVol(int volume);
void	lb_SetSoundVol(int volume);




#endif
