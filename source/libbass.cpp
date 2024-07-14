
#include "libbass.h"
#include "game.h"


extern	int		gMusicVol;


int		gNb_Sounds;
SNDOBJ	gSoundObjet[MAX_SAMPLE];
int		gNbTrack;
KSound	*gHstream = 0;
KMusic	*gHmusic = 0;
float	gFade;
int		zik_id = 0;


void lb_Init()
{
	gHmusic = new KMusic();
	//gMusicVol = (int)(gHmusic->getVolume() * 100);
}

void lb_UnInit()
{
	if(gHstream)
	{
		delete gHstream;
		gHstream = NULL;
	}
}

bool lb_LoadSound(char *name, int nChannel, bool sound3D)
{
	int	SoundID;
	KSound	*smp = NULL;

	if(gNb_Sounds >= MAX_SAMPLE)
		return FALSE;
	SoundID = gNb_Sounds++;
	gSoundObjet[SoundID].nChannel = nChannel;
	bool res;
	if(sound3D)
	{
		smp = new KSound;
		res = smp->loadSample(name);
	}
	else
	{
		smp = new KSound;
		res = smp->loadSample(name);
	}
	if (res == false)
	{
		return false;
	}
	gSoundObjet[SoundID].bass_sample = smp;
//	gSoundObjet[SoundID].hc = 0 ;
	if(!gSoundObjet[SoundID].bass_sample)
		return FALSE;
	return TRUE;
}

void lb_FreeSound(int SoundID)
{
	if(gSoundObjet[SoundID].bass_sample)
	{
		delete (gSoundObjet[SoundID].bass_sample);
		gSoundObjet[SoundID].bass_sample = NULL ;
	}
}

void lb_PlaySound(int SoundID, int volume)
{
	if(volume <= 0)
		return;
//	if(sound_pos>=gSoundObjet[SoundID].nChannel)
//		return;
	if(gSoundObjet[SoundID].bass_sample)
	{
		gSoundObjet[SoundID].bass_sample->playSample( ) ;
		gSoundObjet[SoundID].bass_sample->setVolume( volume );
	}
}

//stoppe un son
void lb_StopSound( long	id )
{
	if ( gSoundObjet[id].bass_sample )
	{
		gSoundObjet[id].bass_sample->stopSample( ) ;
	}
}

void lb_LoadMusic(char *name)
{
//	gHmusic = new KSound;
//	gHmusic->loadModule( name );
}

void lb_UnLoadMusic()
{
}

void lb_PlayMusic()
{
}

void lb_StopMusic()
{
}

void lb_ChangeMusicTrack()
{
	zik_id++;
	if(zik_id > 3)
		zik_id = 0;
}

void lb_ReinitMusicTrack()
{
	zik_id = 0;
}

void lb_MusicFadeOUT(float step)
{
	if(gFade <= MUSICFADE_MIN)
		return;
	lb_SetMusicVol((int)gFade);
	gFade -= step;
	if(gFade < 0)
		gFade = 0;
	if(gFade <= MUSICFADE_MIN)
		lb_SetMusicVol(MUSICFADE_MIN);
}

void lb_MusicFadeIN(float step)
{
	if((int)gFade == gMusicVol)
		return;
	lb_SetMusicVol((int)gFade);
	gFade += step;
	if(gFade > gMusicVol)
	{
		gFade = (float)gMusicVol;
		lb_SetMusicVol((int)gFade);
	}
}

void lb_SetMusicVol(int volume)
{
	if(gHmusic) {
		gHmusic->setVolume(volume);
	}
}

bool lb_PlayMP3(char *name, int volume, bool repeat)
{
	char * audioExtension = ".mp3";
	char * audioFolder = "music";
#ifdef WIN_GRIZZ
	audioExtension = ".ogg";
	audioFolder = "datas/music_win/";
#endif

	char fullPath[256];
	//sprintf(fullPath, "%s%s%s", audioFolder, name, audioExtension);
    sprintf(fullPath, "%s", name);

	if(gHmusic) {
		//gMusicVol = 100;
		gHmusic->setVolume((float)gMusicVol);
		//gHmusic->setVolume((float)gMusicVol / 100.0f); // MAC ?
		gHmusic->playMusic(KMiscTools::makeFilePath(fullPath));
	}
	return true;
}

bool lb_IsMP3Playing()
{
	if(!gHstream)
		return false;
//	if(gHstream->isPlaying())
//		return true;
	return false;
}

void lb_SetChannelVol(int volume)
{
	if(gHmusic == NULL )
		return;
	//gHmusic->setVolume((float)gMusicVol / 100.0f);
	gHmusic->setVolume((float)gMusicVol);
}

void lb_SetSoundVol(int volume)
{
	int	i;
	
	for(i=0 ; i<MAX_SAMPLE ; i++)
	{
		if(gSoundObjet[i].bass_sample)
		{
			gSoundObjet[i].bass_sample->setVolume( volume );
		}
	}
}



