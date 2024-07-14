#include <fcntl.h>
#include "KSound.h"
#include "sprite.h"
#include "directx.h"
#include "game.h"
#ifdef MAC_GRIZZ
#include "MKStore_interface.h"
#endif

#ifdef MAC_GRIZZ
void _strupr(char *tab)
{
   int i = 0;
  while(tab[i] != '\0') {
    tab[i] = toupper (tab[i]);
    i = i + 1;
  }
}
#endif



int		CharOffset[76] = {0,7,12,18,24,31,37,44,50,57,64,67,70,75,81,86,93,103,113,122,131,140,149,158,167,176,179,185,194,203,214,223,232,241,250,259,268,277,286,296,307,315,322,328,333,340,345,353,357,357,364,371,377,384,391,397,404,411,414,418,425,428,439,446,453,460,467,473,480,486,493,500,511,518,525,532};
int		gLanguage;
int		gMusicVol;
int		gSoundVol;
int		ProgramState;
int		PlayerStart_X;
int		PlayerStart_Y;
int		gEpisodeID = 1;
short	gLevelLoaded = 0;
float	scroll = 0;
float	scroll2 = 0;
float	scroll3 = 0;
float	scroll_Y = 0;
float	scroll2_Y = 0;
float	scroll3_Y = 0;
int		gOffset_X = 0;
int		gOffset_Y = 0;
float	gOffset_Xf = 0;
float	gOffset_Yf = 0;
int		gDebugVal = 0;
int		gBlocDefinition[MAX_TILES];
bool	gIsHeroCanJump;
bool	gJumpKeyPressed;
float	gJumpTime;
float	gLostLiveTime;
float	gPomme_DispTime;
int		gPomme;
int		gClee;
float	gClee_DispTime;
int		gVie;
float	gVie_DispTime;
float	gStartCursor_X;
float	gStartCursor_Y;
bool	gKeyState_UP;
bool	gKeyState_DW;
bool	gKeyState_LEFT;
bool	gKeyState_RIGHT;
bool	gKeyState_ENTER;
int		gMenuIndexX;
int		gMenuIndex;
int		level_id;
bool	gDispSharewareWarning;
bool	gDispTheme1Warning;
bool	gDispTheme2Warning;
bool	gEndLevelHit;
char	tprop[256];
bool	gIsDiamantGrabed;
int		gNinvinsibleStars;
int		world1_stat[GAME_MAXLEVELS];
int		world2_stat[GAME_MAXLEVELS];
int		world3_stat[GAME_MAXLEVELS];
int		gMaxUnlockedLevel_w1;
int		gMaxUnlockedLevel_w2;
int		gMaxUnlockedLevel_w3;
float	gGameOverTime;
float	gCongratTime;
bool	gPauseKey;
bool	gIsCheckPoint;
int		gCheckPointX;
int		gCheckPointY;
bool	gPanelIsActive;
float	gTransitionPos;
short	gTransitionDir;
float	gTransitionWaitTime;
bool	gGDlastdir;
float	gBzzTime;
float	gInvinsibleTime;
bool	gAnimatedSplashLoaded;
bool	gPlayerRun;
int		gThemeAccueil;
bool	IsGameFullyActive;
bool	RotationDir = true;

sHeaderGrizzly	gHeader;
sBlocGrizzly	*gLevel;
sBlocGrizzly	*gLevelLocal;

extern	KWindow	*fenetre;
extern	float	gTotalTime_msec;
extern	float	gEffet_EarthQuake;
extern	float	gFPSshowed;
extern	bool	fullscreen;
extern	float	gFrameTime_msec;
extern	pEnt	gEntityList;

extern	bool	gIsDWscroll;
extern	bool	gIsUPscroll;
extern	bool	gIsLFscroll;
extern	bool	gIsRGscroll;


extern KGraphic	*panel,
				*sprites,
				*table,
				*background,
				*paralax1,
				*paralax2,
				*toper,
				*monsters;

extern	bool			shareware;

extern	int				gNb_Sounds;
extern	SNDOBJ			gSoundObjet[MAX_SAMPLE];
extern	int				gNbTrack;



void g_InitTransition()
{
	gTransitionPos = 0;
	gPanelIsActive = false;
}

void g_StartTransition_Close()
{
	gTransitionPos = 0;
	gPanelIsActive = true;
	gTransitionDir = 0;
}

void g_StartTransition_Open()
{
	gTransitionPos = 240;
	gPanelIsActive = true;
	gTransitionDir = 1;
}

bool g_IsTransitionDone_Open()
{
	if(gTransitionPos == 0)
		return true;
	return false;
}

bool g_IsTransitionDone_Close()
{
	if(gTransitionPos < 240)
		return false;
	gTransitionWaitTime -= gFrameTime_msec;
	if(gTransitionWaitTime < 0)
		gTransitionWaitTime = 0;
	if(gTransitionWaitTime == 0)
		return true;
	return false;
}

void g_ScreenTransition_Process()
{
	if(!panel)
		return;
	if(!gPanelIsActive)
		return;

	if(gTransitionDir == 0)
	{
		if(gTransitionPos < 240)
		{
			gTransitionPos += 7;
			if(gTransitionPos >= 240)
			{
				gTransitionPos = 240;
				gTransitionWaitTime = GAME_TRANSWAIT;
			}
		}
	}
	else if(gTransitionDir == 1)
	{
		gTransitionPos -= 7;
		if(gTransitionPos < 0)
			gTransitionPos = 0;
	}

	blit(panel,0,240-(int)gTransitionPos+480,0,0,640,(int)gTransitionPos,false);
	blit(panel,0,240+480,0,480-(int)gTransitionPos,640,(int)gTransitionPos,false);
}





#define	MAX_TAILLE	3000

char * LoadFromFile(char * filename)
{
	char* path = KMiscTools::makeFilePath(filename);

#ifndef WIN32
	path = KMiscTools::makeTempFilePath(filename);

	FILE* f = fopen(path, "r");
	if (f == NULL)
	{
		return NULL;
	}
	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	fseek(f, 0, SEEK_SET);

	char* result = (char*)malloc(size + 1);
	if (size != fread(result, sizeof(char), size, f))
	{
		free(result);
		return NULL; // -2 means file reading fail 
	}
	fclose(f);
	result[size] = 0;
	return result;

	/*
	NSString *documentsDirectory = [NSString stringWithFormat:@"%s", path];

	NSFileManager *fileManager = [NSFileManager defaultManager];
	NSData *data = [fileManager contentsAtPath:documentsDirectory];
	char *string = (char *) [data bytes];

//	NSString *loadedStr = [[NSString alloc] initWithContentsOfFile:documentsDirectory];
//	char *string = (char *) [loadedStr UTF8String];
	return string;
	*/
#endif

	FILE* fh = fopen(path, "rb"); // WIN
	if (fh == NULL) {
		return NULL;
	}

	char buffer[1025];
	int gotten;
	int totalsize = 0;
	int chunksize = 1024;
	char* filebits = (char*)calloc(chunksize, 1);

	while ((gotten = (int)fread(buffer, sizeof(char), chunksize, fh)))
	{
		totalsize += chunksize;
		buffer[gotten] = '\0';
		filebits = (char*)realloc(filebits, (totalsize + chunksize + 1));
		strcat(filebits, buffer);
	}
	fclose(fh);

	return filebits;
}

bool WriteToFile(char * filename, char * datas)
{
	const char* path = KMiscTools::makeFilePath(filename);

	printf("WriteToFile : %s\n", path);

	FILE* f = fopen(path, "wb");
	if (f == NULL)
	{
		return false;
	}
	fwrite(datas, sizeof(char), strlen(datas), f);
	fclose(f);
	return true;
}

void fileWrite(int fh, const char * str)
{
	/*if(fh == -1) {
		return;
	}
	write(fh, str, strlen(str));*/
}

char* game_GetConfigPath()
{
	char* p = CONFIGFILEPATH;

#ifdef WIN_GRIZZ
	char fullPath[256];
	sprintf(fullPath, "datas/jade/%s", p);
	p = fullPath;
#endif
	return  KMiscTools::makeFilePath(p);
}


void game_ResetConfig()
{
    int i;
    for(i=0 ; i<GAME_MAXLEVELS ; i++)
    {
        world1_stat[i] = 0;
        world2_stat[i] = 0;
        world3_stat[i] = 0;
    }
	
	const char* p = game_GetConfigPath();
	//WriteToFile(CONFIGFILEPATH, (char*)"0");
	FILE* fh = fopen(p, "w");
	if (fh == NULL) {
		return;
	}

	fwrite(CONFIGHEADER, strlen(CONFIGHEADER), 1, fh);
	fwrite("\n", strlen("\n"), 1, fh);
	fwrite("0\n", strlen("0\n"), 1, fh); // langue
    
    for(i=0 ; i<GAME_MAXLEVELS ; i++) 
    {
        char lvltab[128];

		sprintf(lvltab, "%d\n", world1_stat[i]);
		fwrite(lvltab, strlen(lvltab), 1, fh);

		sprintf(lvltab, "%d\n", world2_stat[i]);
		fwrite(lvltab, strlen(lvltab), 1, fh);

		sprintf(lvltab, "%d\n", world3_stat[i]);
		fwrite(lvltab, strlen(lvltab), 1, fh);
    }
	fwrite("\n", strlen("\n"), 1, fh);
	fwrite("0\n", strlen("50\n"), 1, fh);
	fwrite("0\n", strlen("50\n"), 1, fh);
	fwrite("0\n", strlen("1\n"), 1, fh);
	fwrite("@", strlen("@"), 1, fh);
	fclose(fh);
}

void g_Codage(char *nom_fichier, short mode)
{
	char	datas[MAX_TAILLE];
	int		i;
	char	c;

	memset(datas, 0, MAX_TAILLE * sizeof(char));

	FILE* fh = fopen(KMiscTools::makeFilePath(nom_fichier), "r"); // WIN
	if (fh == NULL) {
		return;
	}

	i = 0;
	int gotten;
	char buffer[1];
	while ((gotten = (int)fread(buffer, 1, sizeof(char), fh)))
	{
		c = buffer[0];
		if (!mode)
			c = c + 128;
		else
			c = c - 128;
		datas[i++] = c;
	}

	fclose(fh);

	fh = fopen(KMiscTools::makeFilePath(nom_fichier), "w");
	if (fh == NULL) {
		return;
	}
	fwrite(datas, sizeof(char), i, fh);
	fclose(fh);
}

void g_LoadConfig()
{
	BOOL	IsCreateNewConfig;
	char	header[255];

	IsCreateNewConfig = false;

	char buffer[1025];
	int gotten;
	int totalsize = 0;
	int chunksize = 1024;
	char * filebits = (char*)calloc(chunksize, 1);
	
	FILE* fh = fopen(game_GetConfigPath(), "r");
	if (fh == NULL)
	{
		IsCreateNewConfig = true;
	}
	else
	{
		fread(buffer, chunksize, sizeof(char), fh);
		fclose(fh);

		sscanf(buffer, "%s\n", header);
		if(strncmp(header, CONFIGHEADER, 2) != 0) {
			IsCreateNewConfig = true;
		}
	}

	if(IsCreateNewConfig)
	{
        game_ResetConfig();        
	}
	else
	{
		//g_Codage(KMiscTools::makeTempFilePath(CONFIGFILEPATH),1);
	}
    
	fh = fopen(game_GetConfigPath(), "r");
	if (fh == NULL) {
		return;
	}
	filebits = (char*)calloc(chunksize, 1);
	fread(filebits, chunksize, sizeof(char), fh);
	fclose(fh);
	
	char delimiter[] = "\n";
    
    char * pch = strtok (filebits, delimiter);
	
	if(pch) {
		sscanf(pch,"%s", header);
		pch = strtok (NULL, delimiter);
	}
	if(pch) {
		sscanf(pch,"%d", &gLanguage);
		pch = strtok (NULL, delimiter);
	}

    if(pch) 
	{
		for(int i=0 ; i<GAME_MAXLEVELS ; i++) 
		{
			sscanf(pch,"%d",&world1_stat[i]);
			pch = strtok (NULL, delimiter);
			sscanf(pch,"%d",&world2_stat[i]);
			pch = strtok (NULL, delimiter);
			sscanf(pch,"%d",&world3_stat[i]);
			pch = strtok (NULL, delimiter);
		}
	}
    
	if(pch) {
		sscanf(pch,"%d", &gSoundVol);
		pch = strtok (NULL, delimiter);
	}
    
	if(pch) {
		sscanf(pch,"%d", &gMusicVol);
		pch = strtok (NULL, delimiter);
	}
    
	if(pch) {
		int fullscreenInt;
		sscanf(pch,"%d", &fullscreenInt);
		fullscreen = (bool)fullscreenInt;
		pch = strtok (NULL, delimiter);
	}
}

void g_SaveConfig()
{
	FILE* fh = fopen(game_GetConfigPath(), "w+b");
	if (fh == NULL) {
		return;
	}
	fwrite(CONFIGHEADER, strlen(CONFIGHEADER), 1, fh);
	fwrite("\n", strlen("\n"), 1, fh);

	char token[128];
	sprintf(token, "%d\n", (int)gLanguage);
	fwrite(token, strlen(token), 1, fh);
	
	for(int i=0 ; i<GAME_MAXLEVELS ; i++) 
	{
		sprintf(token, "%d\n", world1_stat[i]);
		fwrite(token, strlen(token), 1, fh);

		sprintf(token, "%d\n", world2_stat[i]);
		fwrite(token, strlen(token), 1, fh);
		
		sprintf(token, "%d\n", world3_stat[i]);
		fwrite(token, strlen(token), 1, fh);
	}
	//fwrite("\n", strlen("\n"), 1, fh);
    
    sprintf(token, "%d\n", (int)gSoundVol);
	fwrite(token, strlen(token), 1, fh);
	
	sprintf(token, "%d\n", (int)gMusicVol);
	fwrite(token, strlen(token), 1, fh);
	
	sprintf(token, "%d\n", (int)fullscreen);
	fwrite(token, strlen(token), 1, fh);
    
    fclose(fh);

	//g_Codage(KMiscTools::makeTempFilePath(CONFIGFILEPATH),0);
}



void g_LoadSounds(char *FolderPath)
{
	char	path[260];

	FolderPath = "";
	char * audioExtension = ".wav.caf";
#ifdef WIN_GRIZZ
	FolderPath = "datas/audio_win/";
	audioExtension = ".wav";
#endif
	sprintf(path, "%s%s%s", FolderPath, "boing2", audioExtension);			lb_LoadSound(KMiscTools::makeFilePath(path), 2, false);	// 0
	sprintf(path, "%s%s%s", FolderPath, "fruit", audioExtension);				lb_LoadSound(KMiscTools::makeFilePath(path), 2, false);	// 1
	sprintf(path, "%s%s%s", FolderPath, "bee1", audioExtension);				lb_LoadSound(KMiscTools::makeFilePath(path), 2, false);	// 2
	sprintf(path, "%s%s%s", FolderPath, "breakjarre", audioExtension);		lb_LoadSound(KMiscTools::makeFilePath(path), 1, false);	// 3
	sprintf(path, "%s%s%s", FolderPath, "rebon", audioExtension);				lb_LoadSound(KMiscTools::makeFilePath(path), 2, false);	// 4
	sprintf(path, "%s%s%s", FolderPath, "menu", audioExtension);				lb_LoadSound(KMiscTools::makeFilePath(path), 2, false);	// 5
	sprintf(path, "%s%s%s", FolderPath, "click", audioExtension);				lb_LoadSound(KMiscTools::makeFilePath(path), 2, false);	// 6
	sprintf(path, "%s%s%s", FolderPath, "jump1", audioExtension);				lb_LoadSound(KMiscTools::makeFilePath(path), 2, false);	// 7
	sprintf(path, "%s%s%s", FolderPath, "lost", audioExtension);				lb_LoadSound(KMiscTools::makeFilePath(path), 1, false);	// 8
	sprintf(path, "%s%s%s", FolderPath, "lifeup", audioExtension);			lb_LoadSound(KMiscTools::makeFilePath(path), 1, false);	// 9
	sprintf(path, "%s%s%s", FolderPath, "grizzly_abeille", audioExtension);	lb_LoadSound(KMiscTools::makeFilePath(path), 3, false);	// 10
	sprintf(path, "%s%s%s", FolderPath, "grizzly_chenille", audioExtension);	lb_LoadSound(KMiscTools::makeFilePath(path), 3, false);	// 11
	sprintf(path, "%s%s%s", FolderPath, "grizzly_clef", audioExtension);		lb_LoadSound(KMiscTools::makeFilePath(path), 3, false);	// 12
	sprintf(path, "%s%s%s", FolderPath, "grizzly_cochon", audioExtension);	lb_LoadSound(KMiscTools::makeFilePath(path), 3, false);	// 13
	sprintf(path, "%s%s%s", FolderPath, "grizzly_fantome", audioExtension);	lb_LoadSound(KMiscTools::makeFilePath(path), 3, false);	// 14
	sprintf(path, "%s%s%s", FolderPath, "lifeup", audioExtension);			lb_LoadSound(KMiscTools::makeFilePath(path), 3, false);	// 15
	sprintf(path, "%s%s%s", FolderPath, "banana", audioExtension);			lb_LoadSound(KMiscTools::makeFilePath(path), 3, false);	// 16
	sprintf(path, "%s%s%s", FolderPath, "fruit2", audioExtension);			lb_LoadSound(KMiscTools::makeFilePath(path), 2, false);	// 17
	sprintf(path, "%s%s%s", FolderPath, "fruit3", audioExtension);			lb_LoadSound(KMiscTools::makeFilePath(path), 2, false);	// 18
	sprintf(path, "%s%s%s", FolderPath, "explosion", audioExtension);			lb_LoadSound(KMiscTools::makeFilePath(path), 2, false);	// 19
	sprintf(path, "%s%s%s", FolderPath, "rebon2", audioExtension);			lb_LoadSound(KMiscTools::makeFilePath(path), 2, false);	// 20
	sprintf(path, "%s%s%s", FolderPath, "breakjarre2", audioExtension);		lb_LoadSound(KMiscTools::makeFilePath(path), 1, false);	// 21
}

void g_InitCheckPoints()
{
	gIsCheckPoint = false;
	gCheckPointX = 0;
	gCheckPointY = 0;
}

void g_InitGame()
{
	char	level_name[255];

	srand((unsigned int)time(NULL));

	gAnimatedSplashLoaded = false;
	gOffset_X = 0;
	gOffset_Xf = 0;
	scroll_Y = 0;
	scroll2_Y = 0;
	scroll = scroll2 = scroll3 = 0;

	gPomme = 0;
	gVie = 4;
	gClee = 0;
	gIsDiamantGrabed = false;
	gGameOverTime = 0;
	gCongratTime = 0;

	// charge le fichier des propriete des tiles
	g_SetTilePropFileName(gEpisodeID);
	memset(gBlocDefinition, 0, sizeof(int)*MAX_TILES);
#ifdef MAC_GRIZZ
	sprintf(level_name,KMiscTools::makeFilePath("%s"), tprop);
#else
	sprintf(level_name, KMiscTools::makeFilePath("datas/jade/%s"), tprop);
#endif
    
	FILE* fh = fopen(KMiscTools::makeFilePath(level_name), "r");
	if (fh == NULL) {
		lb_PlayMP3("menu", gMusicVol, true);
		g_InitPresentation();
		return;
	}
	fread(gBlocDefinition, sizeof(int) * MAX_TILES, 1, fh);
	fclose(fh);

//	fread(gBlocDefinition, sizeof(int)*MAX_TILES, 1, file);
//	fclose(file);

	g_InitCheckPoints();

	if(!g_ReInitGame())
	{
		lb_PlayMP3("menu", gMusicVol, true);
		g_InitPresentation();
		return;
	}

	g_PlayMusic();

	// hack
	gTotalTime_msec = 666;
	GetFrameTime();
}

void g_PlayMusic()
{
	switch(gEpisodeID)
	{
		case 1:
		lb_PlayMP3("world1", gMusicVol, true);
		break;

		case 2:
		lb_PlayMP3("world2", gMusicVol, true);
		break;

		case 3:
		lb_PlayMP3("world3", gMusicVol, true);
		break;
	}
}

bool g_ReInitGame()
{
	char	level_name[255];

	if(!gIsCheckPoint)
	{
		gClee = 0;
	}

	gEndLevelHit = false;
	gJumpTime = 0;
	gGDlastdir = false;
	scroll = 0;
	scroll_Y = 0;
	scroll2_Y = 0;
	scroll3_Y = 0;
	gOffset_X = 0;
	gOffset_Y = 0;
	gOffset_Xf = 0;
	gOffset_Yf = 0;
	gEffet_EarthQuake = 0;
	gBzzTime = 0;
	gInvinsibleTime = 0;
	g_ShowItemcounter(0);
	g_ShowItemcounter(1);
	g_ShowItemcounter(2);

	g_CleanupSpriteList();

	// charge le fichier du level
	if(!lvl_LoadRaw(gEpisodeID, level_id, false)) {
		return false;
	}	
/*
	// charge le fichier du level
	sprintf(level_name, "e%ds%d.pak", gEpisodeID, level_id);
	if(!lvl_LoadGedit(level_name))
	{
		sprintf(level_name, "e%ds%d.mapd", gEpisodeID, level_id);
		if(!lvl_LoadMaped(level_name))
		{
			return false;
		}
	}
*/
	gLevelLoaded = 1;
	ProgramState = PS_ACTIVE;

	g_FindPlayerPos();
	g_CadrePlayer();

	// hack
	IsGameFullyActive = false;
	g_DrawScene(); // pour ajouter les sprites
	g_DrawScene(); // pour tout afficher
	IsGameFullyActive = true;
	g_ScreenTransition_Process();

	g_PlayMusic(); // sun 21 octobre 2004

	return true;
}

void g_UnInitGame()
{
	g_SetLevelCleared(gEpisodeID, level_id, false, gIsDiamantGrabed);
	gIsDiamantGrabed = false;
	g_FreeMap();
}

void g_LostLive()
{
//	g_InitGame();
	//ProgramState = PS_LOSTLIVE;

	g_ShowItemcounter(1);
	lb_PlaySound(8, gSoundVol);
	g_InitLostLive();
}

void g_InitLostLive()
{
	pEnt	PlayerEnt;

	ProgramState = PS_LOSTLIVE;
	gLostLiveTime = LOSTLIVE_T;

	PlayerEnt = g_FindSprite(ENT_PLAYER,1);
	if(!PlayerEnt)
		return;
	PlayerEnt->OnLift = false;
//	PlayerEnt->hit = true;	// hack
//	g_AddBumperJump(PlayerEnt);
}

void g_InitLoading()
{
	lb_PlayMP3("intro", gMusicVol, false);

	g_CleanupSpriteList();
	g_AddSprite(ENT_MONSTER5, 558, -40, 0, 0, false);
	//g_AddSprite(ENT_PLAYER, 10, 10, 0, 0, false);
	ProgramState = PS_LOADING;
}

void g_InitPresentation()
{
	//gThemeAccueil = 3; // 1, 2 ou 3

//		int i;
//		for(i=0 ; i<GAME_MAXLEVELS ; i++)
//		world3_stat[i] = 3;

	gThemeAccueil = 1;

	if(g_GetNBunlockedLevel(3) >= GAME_MAXLEVELS)
	{
		gThemeAccueil = 2;
	}
	else if(g_GetNBunlockedLevel(2) >= GAME_MAXLEVELS)
	{
		gThemeAccueil = 3;
	}
	else if(g_GetNBunlockedLevel(1) >= GAME_MAXLEVELS)
	{
		gThemeAccueil = 1;
	}

#ifdef DEMO_VERSION
	gThemeAccueil = 2;
	ProgramState = PS_PRES;
#endif
	
//	gThemeAccueil = 2; //

	g_LoadWorldGFX(gThemeAccueil);

	// completed game hack
	if(g_GetNBunlockedLevel(1) >= GAME_MAXLEVELS+1)
	if(g_GetNBunlockedLevel(2) >= GAME_MAXLEVELS+1)
	if(g_GetNBunlockedLevel(3) >= GAME_MAXLEVELS+1)
	{
		gThemeAccueil = 4;
	}
	
	

	gEndLevelHit = false;
	gEpisodeID = /*gThemeAccueil*/ -1;
	g_InitTransition();
	if(!gAnimatedSplashLoaded)
	{
		gAnimatedSplashLoaded = true;
		//g_LoadWorldGFX(gThemeAccueil);
		switch(gThemeAccueil)
		{
			case 1:
			scroll_Y = 20;
			scroll2_Y = 15;
			break;

			case 4:
			case 2:
			scroll_Y = 25;
			scroll2_Y = 40;
			break;

			case 3:
			scroll_Y = 0;
			scroll2_Y = 0;
			break;
		}
	}
	gStartCursor_X = 204;
	gStartCursor_Y = 356;
	gMenuIndex = 1;
	gDispSharewareWarning = false;
	gDispTheme1Warning = false;
	gDispTheme2Warning = false;
	ProgramState = PS_PRES;
}

void g_InitOptions()
{
	gKeyState_ENTER = true;
	gKeyState_LEFT = false;
	gKeyState_RIGHT = false;
	gStartCursor_X = 45;
	gStartCursor_Y = 300;
	gMenuIndex = 1;
	ProgramState = PS_OPTION;
}

void g_InitSelectEpisode()
{
	gKeyState_ENTER = true;
	gStartCursor_X = 190;
	gStartCursor_Y = 230;
	gMenuIndexX = 1;
	gMenuIndex = 1;
	gDispSharewareWarning = false;
	gDispTheme1Warning = false;
	gDispTheme2Warning = false;
	gMaxUnlockedLevel_w1 = g_GetNBunlockedLevel(1);
	gMaxUnlockedLevel_w2 = g_GetNBunlockedLevel(2);
	gMaxUnlockedLevel_w3 = g_GetNBunlockedLevel(3);
	ProgramState = PS_EPISODE;
}

void g_InitQuit()
{
	gKeyState_ENTER = true;
	ProgramState = PS_CREDIT;
}

void g_ShowItemcounter(int item_id)
{
	if(item_id == 0)
	{
		gPomme_DispTime = POMMEDISP_T;
	}
	else if(item_id == 1)
	{
		gVie_DispTime = POMMEDISP_T;
	}
	else if(item_id == 2)
	{
		gClee_DispTime = POMMEDISP_T;
	}
}

void g_AfficheHUD()
{
	static float pomme_dispy = 5;
	static float vie_dispy = 5;
	static float clee_dispy = 5;

	if(ProgramState == PS_PAUSE)
		goto no_HUDupdate;


	if(gPomme_DispTime)
	{
		if(pomme_dispy < 5)
		{
			pomme_dispy += gFrameTime_msec * 0.10f;
			if(pomme_dispy > 5)
				pomme_dispy = 5;
		}
		else
		{
			if(0) // hack
			{
				gPomme_DispTime -= gFrameTime_msec;
				if(gPomme_DispTime < 0)
					gPomme_DispTime = 0;
			}
		}
	}
	else
	{
		if(pomme_dispy > -27)
		{
			pomme_dispy -= gFrameTime_msec * 0.10f;
			if(pomme_dispy < -27)
				pomme_dispy = -27;
		}
	}



	if(gVie_DispTime)
	{
		if(vie_dispy < 5)
		{
			vie_dispy += gFrameTime_msec * 0.10f;
			if(vie_dispy > 5)
				vie_dispy = 5;
		}
		else
		{
			gVie_DispTime -= gFrameTime_msec;
			if(gVie_DispTime < 0)
				gVie_DispTime = 0;
		}
	}
	else
	{
		if(vie_dispy > -27)
		{
			vie_dispy -= gFrameTime_msec * 0.10f;
			if(vie_dispy < -27)
				vie_dispy = -27;
		}
	}



	if(gClee_DispTime)
	{
		if(clee_dispy < 5)
		{
			clee_dispy += gFrameTime_msec * 0.10f;
			if(clee_dispy > 5)
				clee_dispy = 5;
		}
		else
		{
			if(gClee == 0)
			{
				gClee_DispTime -= gFrameTime_msec;
				if(gClee_DispTime < 0)
					gClee_DispTime = 0;
			}
		}
	}
	else
	{
		if(clee_dispy > -32)
		{
			clee_dispy -= gFrameTime_msec * 0.10f;
			if(clee_dispy < -32)
				clee_dispy = -32;
		}
	}

no_HUDupdate:

	blit(sprites, POMME_X, POMME_Y, 2, (int)pomme_dispy-5, POMME_FW, POMME_FH, true);
	g_DrawNum(40,(int)pomme_dispy+5,gPomme);

	blit(sprites, LIFELOGO_X, LIFELOGO_Y, 515, (int)vie_dispy, LIFELOGO_FW, LIFELOGO_FH, true);
	g_DrawNum(570,(int)vie_dispy+5,gVie);

	blit(sprites, KEY_X, KEY_Y, 304, (int)clee_dispy, KEY_FW, KEY_FH, true);
	g_DrawNum(346,(int)clee_dispy+5,gClee);
}

void g_MenuSys(DWORD input, int X_offset, int Y_offset, int nMaxItemX, int nMaxItemY)
{
	if(X_offset != 0)
	{
		if(input & KEY_LEFT || input & KEY_LEFTJP)
		{
			if(!gKeyState_LEFT)
			{
				lb_PlaySound(5, gSoundVol);
				gKeyState_LEFT = true;
				gMenuIndexX--;
				if(gMenuIndexX <= 0)
				{
					gStartCursor_X += (nMaxItemX-1) * X_offset;
					gMenuIndexX = nMaxItemX;
				}
				else
				{
					gStartCursor_X -= X_offset;
				}
			}
		}
		else
		{
			gKeyState_LEFT = false;
		}

		if(input & KEY_RIGHT || input & KEY_RIGHTJP)
		{
			if(!gKeyState_RIGHT)
			{
				lb_PlaySound(5, gSoundVol);
				gKeyState_RIGHT = true;
				gMenuIndexX++;
				if(gMenuIndexX > nMaxItemX)
				{
					gStartCursor_X -= (gMenuIndexX-2) * X_offset;
					gMenuIndexX = 1;
				}
				else
				{
					gStartCursor_X += X_offset;
				}
			}
		}
		else
		{
			gKeyState_RIGHT = false;
		}
	}


	if(Y_offset != 0)
	{
		if(input & KEY_UP || input & KEY_UPJP)
		{
			if(!gKeyState_UP)
			{
				lb_PlaySound(5, gSoundVol);
				gKeyState_UP = true;
				gMenuIndex--;
				if(gMenuIndex <= 0)
				{
					gStartCursor_Y += (nMaxItemY-1) * Y_offset;
					gMenuIndex = nMaxItemY;
				}
				else
				{
					gStartCursor_Y -= Y_offset;
				}
			}
		}
		else
		{
			gKeyState_UP = false;
		}

		if(input & KEY_DOWN || input & KEY_DOWNJP)
		{
			if(!gKeyState_DW)
			{
				lb_PlaySound(5, gSoundVol);
				gKeyState_DW = true;
				gMenuIndex++;
				if(gMenuIndex > nMaxItemY)
				{
					gStartCursor_Y -= (gMenuIndex-2) * Y_offset;
					gMenuIndex = 1;
				}
				else
				{
					gStartCursor_Y += Y_offset;
				}
			}
		}
		else
		{
			gKeyState_DW = false;
		}
	}
}

void g_EndLevel_Process()
{
	if(!gEndLevelHit)
		return;

	if(g_IsTransitionDone_Close() && lb_IsMP3Playing() == false)
	{
		g_SaveConfig();
		g_StartTransition_Open();
		level_id++;
		if(shareware)
		{
			if(level_id >= MAXDEMOLEVELS)
			{
				lb_PlayMP3("menu", gMusicVol, true);
				g_InitPresentation();
				g_InitQuit();
				return;
			}
		}
		if(!g_ReInitGame())
		{
			lb_PlayMP3("menu", gMusicVol, true);
			g_InitPresentation();
		}
	}
}

void g_EndLevelHit()
{
	if(gEndLevelHit)
		return;

	g_InitCheckPoints();
	player_MakeInvinsible();	// hack

	lb_PlayMP3("level_finish", gMusicVol, false);

	if(level_id == 10)
	{
		g_SetLevelCleared(gEpisodeID, level_id, true, gIsDiamantGrabed);
		gIsDiamantGrabed = false;
		g_InitCongratulation();
		return;
	}


	gEndLevelHit = true;
	g_StartTransition_Close();
	g_SetLevelCleared(gEpisodeID, level_id, true, gIsDiamantGrabed);
	gIsDiamantGrabed = false;
}

bool g_DrawScene()
{
	// affiche le decor
	w_DrawWorld();

	// affiche les tiles
	level_Affiche((int)gOffset_X, (int)gOffset_Y);

	// affiche les sprites
	g_DrawSprites();

	w_DrawToper((int)gOffset_X, (int)gOffset_Y);

	// affiche les compteurs
	g_AfficheHUD();

	// affiche les items attrapé par dessus tout
	g_DrawOntopSprites();

	g_AnimateSprite();

	g_SmokeProcess();

	g_PangProcess();

	return true;
}

void g_DrawLoadingLogo()
{
	static float total_posy = 0;
	static double valuex = 0;
	static float logo_posx = 0;
	static double valuey = 0;
	static float logo_posy = 0;

	total_posy += gFrameTime_msec * 0.028f;
		
	logo_posx += 0.012f * gFrameTime_msec * (float)sin(valuex); // 0.008
	valuex += (0.001 * (double)gFrameTime_msec);
	if(valuex >= 6.25)
		valuex -= 6.25;
	logo_posy += 0.012f * gFrameTime_msec * (float)sin(valuey);
	valuey += (0.003 * (double)gFrameTime_msec);
	if(valuey >= 6.25)
		valuey -= 6.25;
	blit(sprites, JADE_X, JADE_Y, 535+(int)logo_posx, (int)total_posy+(int)logo_posy, JADE_W, JADE_H, true);

	// hack : place l'arraigné au dessus du logo jadeware
	{
		pEnt ArraigneeEnt = g_FindSprite(ENT_MONSTER5, 1);
		if(ArraigneeEnt)
		{
			ArraigneeEnt->x = 535+logo_posx + 15;
			ArraigneeEnt->y = total_posy+logo_posy - 23;
		}
	}
}

void g_DrawTitle()
{
	blit(sprites, TITLE_X, TITLE_Y, 60, 15, TITLE_W, TITLE_H, true);
}

void g_DrawSplashPres()
{
	static double valuex = 0;
	static float sun_posx = 0;
	static double valuey = 0;
	static float sun_posy = 0;
	static float sinux = 0;

	blit(background, 0, 0, 0, 0, ScreenX, ScreenY, false);
	
	if(gThemeAccueil != 4)
	{
		w_Draw2Paralax();
	}
	switch(gThemeAccueil)
	{
		case 1:
		w_DrawToper2(0, 0, gThemeAccueil);
		break;

		case 2:
		sun_posx += 0.008f * gFrameTime_msec * (float)sin(valuex);
		valuex += (0.001 * (double)gFrameTime_msec);
		if(valuex >= 6.25)
			valuex -= 6.25;
		sun_posy += 0.012f * gFrameTime_msec * (float)sin(valuey);
		valuey += (0.003 * (double)gFrameTime_msec);
		if(valuey >= 6.25)
			valuey -= 6.25;
		blit(sprites, SUN_X, SUN_Y, 500+(int)sun_posx, 10+(int)sun_posy, SUN_W, SUN_H, true);
		break;

		case 3:
		w_DrawToper2(0, 0, gThemeAccueil);
		break;

		case 4:
		sun_posx += 0.008f * gFrameTime_msec * (float)sin(valuex);
		valuex += (0.001 * (double)gFrameTime_msec);
		if(valuex >= 6.25)
			valuex -= 6.25;
		sun_posy += 0.012f * gFrameTime_msec * (float)sin(valuey);
		valuey += (0.003 * (double)gFrameTime_msec);
		if(valuey >= 6.25)
			valuey -= 6.25;
		blit(sprites, SUN_X, SUN_Y+105, 500+(int)sun_posx, 200+(int)sun_posy, SUN_W, SUN_H, true);
		w_Draw2Paralax();
	//	g_DrawSmiley();	
		gNinvinsibleStars = 6;
		gInvinsibleTime = INVISIBLE_T/2;
		player_DrawCircleStarsPos(192, 64, &sinux);
		break;
	}
	g_DrawTitle();
	blit(sprites, JADE_X, JADE_Y, 554, 418, JADE_W, JADE_H, true);
	gOffset_X = 100;
	g_Scroll_Left(gFrameTime_msec * SCROLL_SPEED/3);
	g_DispSharewareWarning();
}

void g_OscilleCurseur()
{
	static double value = 0;
		
	gStartCursor_X -= 0.02f * gFrameTime_msec * (float)sin(value);
	value += (0.005 * (double)gFrameTime_msec);
	if(value >= 6.25)
		value = 0;
}

void g_PauseProcess(long kb_input)
{
	if(ProgramState != PS_PAUSE && ProgramState != PS_ACTIVE)
		return;
	if(kb_input & KEY_P || kb_input & KEY_PAUSEJP)
	{
		if(!gPauseKey)
		{
			gPauseKey = true;
			if(ProgramState == PS_PAUSE)
			{
				lb_SetChannelVol(gMusicVol);
				ProgramState = PS_ACTIVE;
			}
			else if(ProgramState == PS_ACTIVE)
			{
				lb_SetChannelVol(gMusicVol/4);
				ProgramState = PS_PAUSE;
			}
		}
	}
	else
	{
		gPauseKey = false;
	}
}

void g_InitCongratulation()
{
	player_MakeInvinsible();
	gCongratTime = 14000.0f;
	ProgramState = PS_CONGRAT;
}

void g_InitGameover()
{
	lb_PlayMP3("gameover", gMusicVol, false);
	gGameOverTime = 10000.0f;
	ProgramState = PS_GAMEOVER;
}

void g_WaitKeyReleased(int key)
{
	long input;
	long inputJOY;

	do
	{
		input = ReadKeyboard();
		inputJOY = ReadJoypad();
		if(inputJOY) {
			input = input | inputJOY;
		}
	}
	while(input == key);
}

void g_DrawGamepadIcon()
{
	int	pos_x;

	pos_x = 0;
	if(ProgramState == PS_LOADING)
		pos_x = 70; // 40
	else if(ProgramState == PS_OPTION)
		pos_x = (ScreenX - NOGAMEPAD_W)/2;
	
	bool is_joypad = HasJoypad();

	if(!is_joypad)
	{
		blit(sprites, NOGAMEPAD_X, NOGAMEPAD_Y, pos_x, ScreenY - NOGAMEPAD_H, NOGAMEPAD_W, NOGAMEPAD_H, true);
	}
	else
	{
		blit(sprites, NOGAMEPAD_X, NOGAMEPAD_Y+NOGAMEPAD_H+1, pos_x, ScreenY - NOGAMEPAD_H, NOGAMEPAD_W, NOGAMEPAD_H, true);
	}
}

void g_DrawSmiley()
{
	blit(sprites, SMILEY_X, SMILEY_Y, 219, 15, SMILEY_W, SMILEY_H, true);
}

void g_UpdateFrame()
{
	long	input;
	long	inputJOY;

	input = ReadKeyboard();
	inputJOY = ReadJoypad();
	if(inputJOY)
		input = input | inputJOY;

	switch(ProgramState)
	{
		case PS_LOADING:
		{
			pEnt Ent = gEntityList;

			fenetre->setWorldView( 0, 0, 0, 1 , true );
			g_DrawTitle();
			blit(sprites, PRESSENTER_X, PRESSENTER_Y, (ScreenX-PRESSENTER_W)/2, ScreenY - PRESSENTER_H - 8, PRESSENTER_W, PRESSENTER_H, true);
			//ptk logo
		//	blit(sprites, 422, 78, 4, ScreenY - 32, 55, 28, false);
			g_DrawGamepadIcon();
			g_DrawLoadingLogo();
			g_AnimateSprite();
			g_DrawSprites();

			if(input & KEY_ENTER || input & KEY_ENTERJP)
			{
				g_WaitKeyReleased(KEY_ENTER);
				g_WaitKeyReleased(KEY_ENTERJP);
				if(!gKeyState_ENTER)
				{
					gKeyState_ENTER = true;
					lb_PlayMP3("menu", gMusicVol, true);
					g_InitPresentation();
				}
			}
			else
			{
				gKeyState_ENTER = false;
			}

			if(input & KEY_ESC || input & KEY_ESCJP)
			{
				g_WaitKeyReleased(KEY_ESC);
				g_WaitKeyReleased(KEY_ESCJP);
				lb_PlayMP3("menu", gMusicVol, true);
				g_InitPresentation();
			}

			if(lb_IsMP3Playing() == false)
			{
				lb_PlayMP3("menu", gMusicVol, true);
				g_InitPresentation();
			}
		}
		break;

		case PS_CREDIT:
		{
			if(input & KEY_ENTER || input & KEY_ENTERJP)
			{
				if(!gKeyState_ENTER)
				{
					gKeyState_ENTER = true;
					ProgramState = PS_QUIT;
				}
			}
			else
			{
				gKeyState_ENTER = false;
			}

			if(input & KEY_ESC || input & KEY_ESCJP)
			{
				ProgramState = PS_QUIT;
			}
			g_DrawSplashPres();
			if(shareware)
			{
				blit(panel, 0, 0, 0, 0, 640, 480, true);
			}
			else
			{
				blit(sprites, CREDIT_X, CREDIT_Y, 220, 190+30, CREDIT_W, CREDIT_H, true);
			}
		}
		break;

		case PS_OPTION:
		{
			g_OscilleCurseur();
			g_DrawSplashPres();
			blit(sprites, MENU3_X, MENU3_Y, 55, 280, MENU3_W, MENU3_H, true);
			blit(sprites, LEVEL_X, LEVEL_Y, 291, 294, 14*(gMusicVol/5), LEVEL_H, true);
			blit(sprites, LEVEL_X, LEVEL_Y, 291, 341, 14*(gSoundVol/5), LEVEL_H, true);
			g_DrawGamepadIcon();
			
			g_MenuSys(input, 0, 43, 0, 3);
			blit(sprites, CURSOR_X, CURSOR_Y, (int)gStartCursor_X, (int)gStartCursor_Y, CURSOR_W, CURSOR_H, true);

			
			if(input & KEY_LEFT || input & KEY_LEFTJP)
			{
				if(!gKeyState_LEFT)
				{
					gKeyState_LEFT = true;
					lb_PlaySound(5, gSoundVol);
					switch(gMenuIndex)
					{
						case 1:
						gMusicVol -= 5;
						if(gMusicVol < 0)
							gMusicVol = 0;
						lb_SetChannelVol(gMusicVol);
						break;

						case 2:
						gSoundVol -= 5;
						if(gSoundVol < 0)
							gSoundVol = 0;
						lb_SetSoundVol(gSoundVol);
						break;
					}
				}
			}
			else
			{
				gKeyState_LEFT = false;
			}

			if(input & KEY_RIGHT || input & KEY_RIGHTJP)
			{
				if(!gKeyState_RIGHT)
				{
					gKeyState_RIGHT = true;
					lb_PlaySound(5, gSoundVol);
					switch(gMenuIndex)
					{
						case 1:
						gMusicVol += 5;
						if(gMusicVol > 100)
							gMusicVol = 100;
						lb_SetChannelVol(gMusicVol);
						break;

						case 2:
						gSoundVol += 5;
						if(gSoundVol > 100)
							gSoundVol = 100;
						break;
					}
				}
			}
			else
			{
				gKeyState_RIGHT = false;
			}

			if(input & KEY_ENTER || input & KEY_ENTERJP)
			{
				if(!gKeyState_ENTER)
				{
					lb_PlaySound(6, gSoundVol);
					gKeyState_ENTER = true;
					switch(gMenuIndex)
					{
						case 1:
						return;

						case 2:
						return;

						case 3:
						g_InitPresentation();
						return;
					}
				}
			}
			else
			{
				gKeyState_ENTER = false;
			}

			if(input & KEY_ESC || input & KEY_ESCJP)
			{
				g_InitPresentation();
			}

		}
		break;

		case PS_EPISODE:
		{
			g_OscilleCurseur();
			g_DrawSplashPres();
			blit(sprites, MENU2_X, MENU2_Y, 50, 220, MENU2_W, MENU2_H, true);
			
			{
				int	i;

				blit(sprites, MENU4_X, MENU4_Y, 190, 220, MENU4_W, MENU4_H, true);
				blit(sprites, MENU4_X, MENU4_Y, 190, 220+37, MENU4_W, MENU4_H, true);
				blit(sprites, MENU4_X, MENU4_Y, 190, 220+74, MENU4_W, MENU4_H, true);

				for(i=0 ; i<GAME_MAXLEVELS ; i++)
				{
					switch(world1_stat[i])
					{
						case 1:
						blit(sprites, MENU4_X+32*i, MENU4_Y-34, 190+32*i, 220, 30, MENU4_H, true);
						break;

						case 2:
						blit(sprites, MENU4_X+32*i, MENU4_Y+68, 190+32*i, 220, 30, MENU4_H, true);
						break;

						case 3:
						blit(sprites, MENU4_X+32*i, MENU4_Y+34, 190+32*i, 220, 30, MENU4_H, true);
						break;
					}

					switch(world2_stat[i])
					{
						case 1:
						blit(sprites, MENU4_X+32*i, MENU4_Y-34, 190+32*i, 220+37, 30, MENU4_H, true);
						break;

						case 2:
						blit(sprites, MENU4_X+32*i, MENU4_Y+68, 190+32*i, 220+37, 30, MENU4_H, true);
						break;

						case 3:
						blit(sprites, MENU4_X+32*i, MENU4_Y+34, 190+32*i, 220+37, 30, MENU4_H, true);
						break;
					}

					switch(world3_stat[i])
					{
						case 1:
						blit(sprites, MENU4_X+32*i, MENU4_Y-34, 190+32*i, 220+74, 30, MENU4_H, true);
						break;

						case 2:
						blit(sprites, MENU4_X+32*i, MENU4_Y+68, 190+32*i, 220+74, 30, MENU4_H, true);
						break;

						case 3:
						blit(sprites, MENU4_X+32*i, MENU4_Y+34, 190+32*i, 220+74, 30, MENU4_H, true);
						break;
					}
				}
				
				
				// Cadenas locked inapp purchases levels
#ifdef MAC_GRIZZ
				if( MKStoreAction(MK_ACTION_ISBOUGHT_PACK1) == 0)
				{
					blit(sprites,CADENAS_X,CADENAS_Y, 166, MENU4_Y+9, CADENAS_W, CADENAS_H, true);
				}
				if( MKStoreAction(MK_ACTION_ISBOUGHT_PACK2) == 0)
				{
					blit(sprites,CADENAS_X,CADENAS_Y, 166, MENU4_Y+45, CADENAS_W, CADENAS_H, true);
				}
#endif
				
				if(gMaxUnlockedLevel_w1 == GAME_MAXLEVELS+1)
				{
					blit(sprites, MENU4_X+32*10, MENU4_Y-34, 190+32*10, 220, 68, MENU4_H, true);
				}
				if(gMaxUnlockedLevel_w2 == GAME_MAXLEVELS+1)
				{
					blit(sprites, MENU4_X+32*10, MENU4_Y-34, 190+32*10, 220+37, 68, MENU4_H, true);
				}
				if(gMaxUnlockedLevel_w3 == GAME_MAXLEVELS+1)
				{
					blit(sprites, MENU4_X+32*10, MENU4_Y-34, 190+32*10, 220+74, 68, MENU4_H, true);
				}
			}

//			g_DispSharewareWarning();
			if(gDispTheme1Warning == true) {
				blit(sprites, THEME1_X, THEME1_Y, 170, 200, THEME1_W, THEME1_H, false);
			}
			if(gDispTheme2Warning == true) {
				blit(sprites, THEME2_X, THEME2_Y, 170, 200, THEME2_W, THEME2_H, false);
			}

			{
				int	Old_gMenuIndex;

				Old_gMenuIndex = gMenuIndex;

				switch(gMenuIndex)
				{
					case 1:
					g_MenuSys(input, 33, 36, gMaxUnlockedLevel_w1, 3);
					break;

					case 2:
					g_MenuSys(input, 33, 36, gMaxUnlockedLevel_w2, 3);
					break;

					case 3:
					g_MenuSys(input, 33, 36, gMaxUnlockedLevel_w3, 3);
					break;
				}
				

				if(gMenuIndex != Old_gMenuIndex)
				{
					int	n;
					
					gDispTheme1Warning = false;
					gDispTheme2Warning = false;

				//	gStartCursor_X -= (gMenuIndexX-1) * 33;
				//	gMenuIndexX = 1;
					
					switch(gMenuIndex)
					{
						case 1:
						if(gMenuIndexX > gMaxUnlockedLevel_w1)
						{
							n = gMenuIndexX - gMaxUnlockedLevel_w1;
							gStartCursor_X -= n * 33;
							gMenuIndexX = gMaxUnlockedLevel_w1;
						}
						break;

						case 2:
						if(gMenuIndexX > gMaxUnlockedLevel_w2)
						{
							n = gMenuIndexX - gMaxUnlockedLevel_w2;
							gStartCursor_X -= n * 33;
							gMenuIndexX = gMaxUnlockedLevel_w2;
						}
						break;

						case 3:
						if(gMenuIndexX > gMaxUnlockedLevel_w3)
						{
							n = gMenuIndexX - gMaxUnlockedLevel_w3;
							gStartCursor_X -= n * 33;
							gMenuIndexX = gMaxUnlockedLevel_w3;
						}
						break;
					}
					
				}
			}

			blit(sprites, CURSOR_X, CURSOR_Y, (int)gStartCursor_X, (int)gStartCursor_Y, CURSOR_W, CURSOR_H, true);
			g_ScreenTransition_Process();
			
			if(input & KEY_ESC || input & KEY_ESCJP)
			{
				g_InitPresentation();
			}
			
			if(g_IsTransitionDone_Close())
			{
				g_StartTransition_Open();
				//g_InitTransition();
				g_LoadWorldGFX(gEpisodeID);
				g_InitGame();
				return;
			}
			
			if(!gPanelIsActive)
			{
				if(input & KEY_ENTER || input & KEY_ENTERJP)
				{
					if(!gKeyState_ENTER)
					{
						lb_PlaySound(6, gSoundVol);
						gKeyState_ENTER = true;
						switch(gMenuIndex)
						{
							case 1:
							{
								level_id = gMenuIndexX;
								/*if(shareware && level_id >= MAXDEMOLEVELS)
								{
									gDispSharewareWarning = true;
								}
								else
								{*/
									gEpisodeID = 1;
									g_StartTransition_Close();
								//}
							}
							break;
								
							case 2:
							{
#ifdef MAC_GRIZZ
								if( MKStoreAction(MK_ACTION_ISBOUGHT_PACK1) == 0)
								{
									if(gDispTheme1Warning == false)
									{
										gDispTheme1Warning = true;
									}
									else
									{
										MKStoreAction(MK_ACTION_BUY_PACK1);
									}
								}
								else
								{
									level_id = gMenuIndexX;
									gEpisodeID = 2;
									g_StartTransition_Close();
									return;
								}
#else
								level_id = gMenuIndexX;
								gEpisodeID = 2;
								g_StartTransition_Close();
								return;
#endif
							}
							break;
								
							case 3:
							{
#ifdef MAC_GRIZZ
								if( MKStoreAction(MK_ACTION_ISBOUGHT_PACK2) == 0)
								{
									if(gDispTheme2Warning == false)
									{
										gDispTheme2Warning = true;
									}
									else
									{
										MKStoreAction(MK_ACTION_BUY_PACK2);
									}
								}
								else
								{
									level_id = gMenuIndexX;
									gEpisodeID = 3;
									g_StartTransition_Close();
									return;
								}
#else
								level_id = gMenuIndexX;
								gEpisodeID = 3;
								g_StartTransition_Close();
								return;
#endif
							}
							break;
						}
					}
				}
				else
				{
					gKeyState_ENTER = false;
				}
			}
		}
		break;

		case PS_PRES:
		{
			g_OscilleCurseur();
			g_DrawSplashPres();
			blit(sprites, MENU1_X, MENU1_Y, 244, 352, MENU1_W, MENU1_H, true);
			g_MenuSys(input, 0, 37, 0, 3);
			blit(sprites, CURSOR_X, CURSOR_Y, (int)gStartCursor_X, (int)gStartCursor_Y, CURSOR_W, CURSOR_H, true);

			if(input & KEY_ENTER || input & KEY_ENTERJP)
			{
				if(!gKeyState_ENTER)
				{
					lb_PlaySound(6, gSoundVol);
					gKeyState_ENTER = true;
					switch(gMenuIndex)
					{
						case 1:
						g_InitSelectEpisode();
						break;

						case 2:
						g_InitOptions();
						break;

						case 3:
						g_InitQuit();
						return;
					}
				}
			}
			else
			{
				gKeyState_ENTER = false;
			}
		}
		break;

		case PS_CONGRAT:
		{
			if(input & KEY_ESC || input & KEY_ESCJP)
			{
				g_UnInitGame();
				//lb_PlayMP3("menu", gMusicVol);
				g_InitPresentation();
				return;
			}

			if(!g_DrawScene())
				return;
			monster_Update();
			player_Update(0);
			g_CongratProcess();
			g_ScreenTransition_Process();
		}
		break;

		case PS_GAMEOVER:
		{
			if(input & KEY_ESC || input & KEY_ESCJP)
			{
				g_UnInitGame();
				//lb_PlayMP3("menu", gMusicVol);
				g_InitPresentation();
				return;
			}

			if(!g_DrawScene())
				return;
			monster_Update();
	//		player_Update(input);
			g_GameOverProcess();
			g_ScreenTransition_Process();
		}
		break;

		case PS_PAUSE:
		{
			if(input & KEY_ESC || input & KEY_ESCJP)
			{
				g_UnInitGame();
				lb_PlayMP3("menu", gMusicVol, true);
				g_InitPresentation();
				return;
			}

			g_PauseProcess(input);
			g_EndLevel_Process();
			if(!g_DrawScene())
				return;
			blit(sprites, PAUSE_X, PAUSE_Y, (ScreenX-PAUSE_W)/2, (ScreenY-PAUSE_H)/2, PAUSE_W, PAUSE_H, true);
			g_ScreenTransition_Process();
		}
		break;

		case PS_ACTIVE:
		{
			g_PauseProcess(input);
			g_EndLevel_Process();
			if(!g_DrawScene())
				return;
			//g_DispDebugInfos(); // debug infos
			monster_Update();
			player_Update(input);
			g_ScreenTransition_Process();
			
			if(input & KEY_ESC || input & KEY_ESCJP)
			{
				g_SaveConfig(); //
				g_UnInitGame();
				lb_PlayMP3("menu", gMusicVol, true);
				g_InitPresentation();
				return;
			}
		}
		break;

		case PS_LOSTLIVE:
		{
			if(input & KEY_ESC || input & KEY_ESCJP)
			{
				lb_PlayMP3("menu", gMusicVol, true);
				g_InitPresentation();
				return;
			}

			g_DrawScene();
			//player_Update(input);

			gLostLiveTime -= gFrameTime_msec;
			if(gLostLiveTime < 0)
			{
				gLostLiveTime = 0;
				gVie--;
				g_ShowItemcounter(1);
				if(gVie < 0)
				{
					gVie = 0;
					g_InitGameover();
					//ProgramState = PS_QUIT;
					return;
				}
				g_ReInitGame();
				return;
			}
			g_ScreenTransition_Process();
		}
		break;
	}
}

void g_DispDebugInfos()
{
	int	txt_y = 0;
	int	offset_y = 12;

	g_DrawString(0, 0, "%dFPS", (int)gFPSshowed);
	return;

	pEnt Player = g_FindSprite(ENT_PLAYER,1);
	if(!Player)
		return;

	g_DrawString(0,txt_y,"player_x: %d", (int)Player->x + gOffset_X);	txt_y += offset_y;
	g_DrawString(0,txt_y,"player_y: %d", (int)Player->y + gOffset_Y);	txt_y += offset_y;

}

void g_AddCompteur_Pomme(int n_pomme)
{
	gPomme += n_pomme;
	if(gPomme > 99)
	{
		gPomme -= 100;
		gVie++;
		lb_PlaySound(9,gSoundVol);
		g_ShowItemcounter(1);
	}
}

void g_DrawString(int x, int y, char *string, ...)
{
	int		num;
	int		l;
	int		size;
	int		s;
	va_list	msg;
    char	buffer[256] = {'\0'};

	va_start(msg,string);
	vsprintf(buffer, string, msg);	
	va_end(msg);


	size = strlen(buffer);

	for(s=0 ; s<size ; s++)
	{
		num = (int)buffer[s];
		if(num == ' ')
		{
			x += 6;
			continue;
		}
		num -= '0';
		l = CharOffset[num+1] - CharOffset[num];
		blit(sprites, FONT_X+CharOffset[num],FONT_Y,x,y,l,12,true);
		x += l;
	}
}

void g_DrawNum(int x, int y, int number)
{
	int	num;
	int	size;
	int	s;

	size = 0;
	num = number;
	while(num > 0)
	{
		num /= 10;
		size++;
	}

	if(!size)
		size = 1;

	while(size)
	{
		s = size;
		num = number;
		while(s > 1)
		{
			num /= 10;
			s--;
		}
		num %= 10;

		blit(sprites, NUM_X+num*NUM_FW, NUM_Y, x, y, NUM_FW, NUM_FH, true);
		x += (NUM_FW-2); // HACK
		size--;
	}
}

void g_DispSharewareWarning()
{
	if(!gDispSharewareWarning)
		return;
	blit(sprites, 654, 1, 170, 200, 300, 80, false);
}

void g_AddParticles(int partcle_id, int x, int y)
{
	float	chutte_grav;

	if(y < 0)
		return;

	chutte_grav = (float)(rand() % 100) / 100.0f;
	if(partcle_id == ENT_GREENSTAR) chutte_grav /= 4;
	g_AddSprite(partcle_id, x, y, -0.3f, -3+chutte_grav,true);

	chutte_grav = (float)(rand() % 100) / 100.0f;
	if(partcle_id == ENT_GREENSTAR) chutte_grav /= 4;
	g_AddSprite(partcle_id, x, y, 0, -3+chutte_grav,true);

	chutte_grav = (float)(rand() % 100) / 100.0f;
	if(partcle_id == ENT_GREENSTAR) chutte_grav /= 4;
	g_AddSprite(partcle_id, x, y, 0.3f, -3+chutte_grav,true);
}

void g_AddSpecialBlocItem(int objid, int x, int y)
{
	float	chutte_grav;
	float	vel_x;

	if(objid == 1)
	{
		vel_x = (float)(rand() % 200) / 100.0f - 1;
		chutte_grav = (float)(rand() % 100) / 100.0f;
		g_AddSprite(ENT_POMMEJ, x, y, vel_x, -5+chutte_grav,false);
	}
	else if(objid == 2)
	{
		vel_x = (float)(rand() % 200) / 100.0f - 1;
		chutte_grav = (float)(rand() % 100) / 100.0f;
		g_AddSprite(ENT_POMMEV, x, y, vel_x, -5+chutte_grav,false);
	}
	else
	{
		switch(rand()%4)
		{
			case 0:
				chutte_grav = (float)(rand() % 100) / 100.0f;
				g_AddSprite(ENT_BANANE, x, y, -1.0f, -5+chutte_grav,false);

				chutte_grav = (float)(rand() % 100) / 100.0f;
				g_AddSprite(ENT_BANANE, x, y, 0, -5+chutte_grav,false);

				chutte_grav = (float)(rand() % 100) / 100.0f;
				g_AddSprite(ENT_BANANE, x, y, 1.0f, -5+chutte_grav,false);
			break;

			case 1:
				chutte_grav = (float)(rand() % 100) / 100.0f;
				g_AddSprite(ENT_FEUILLE, x, y, -0.3f, -5+chutte_grav,false);

				chutte_grav = (float)(rand() % 100) / 100.0f;
				g_AddSprite(ENT_FEUILLE, x, y, 0, -5+chutte_grav,false);

				chutte_grav = (float)(rand() % 100) / 100.0f;
				g_AddSprite(ENT_FEUILLE, x, y, 0.3f, -5+chutte_grav,false);
			break;

			case 2:
				vel_x = (float)(rand() % 200) / 100.0f - 1;
				chutte_grav = (float)(rand() % 100) / 100.0f;
				g_AddSprite(ENT_POMMEV, x, y, vel_x, -5+chutte_grav,false);
			break;

			case 3:
				vel_x = (float)(rand() % 200) / 100.0f - 1;
				chutte_grav = (float)(rand() % 100) / 100.0f;
				g_AddSprite(ENT_POMMEJ, x, y, vel_x, -5+chutte_grav,false);
			break;
		}
	}
}

void g_LoadWorldGFX(int world_id)
{
	char	path[255];

	if(world_id < 1 || world_id > 4)
		world_id = 1;

//	if(background)		DeleteSurface(background);
//	if(paralax1)		DeleteSurface(paralax1);
//	if(paralax2)		DeleteSurface(paralax2);
//	if(toper)			DeleteSurface(toper);
//	if(table)			DeleteSurface(table);

	if(background)
	{
		delete background;		
		background = NULL;
	}
	if(paralax1)
	{
		delete paralax1;		
		paralax1 = NULL;
	}

	
#ifdef DEMO_VERSION
	//if(ProgramState == PS_PRES && world_id == 2)
	//	world_id = 1;
#endif
	

	// chargement de la surface du BACKGROUND + tiles
	sprintf(path, "world%d", world_id);
//	if(!background)
//	{
		background = DDLoadBitmap(path);
		if(!background)
			Complain( "On a pas pu charger : background" );
//	}
//	else
//		DDReLoadBitmap(background, path);
    

	// chargement de la surface du PARALAX_A
	//sprintf(path, KMiscTools::makeFilePath("gfx\\paralaxs%d.png"), world_id);
	sprintf(path, "paralaxs%d", world_id);
//	if(!paralax1)
//	{
		paralax1 = DDLoadBitmap(path);
		if(!paralax1) 
			Complain( "On a pas pu charger : paralax1" );
//	}
//	else
//		DDReLoadBitmap(paralax1, KMiscTools::makeFilePath(path));

	paralax2 = paralax1;
	toper = panel;
	table = background;
}

void g_CongratProcess()
{
//	pEnt	PlayerEnt;

	gCongratTime -= gFrameTime_msec;
	if(gCongratTime < 0)
	{
		gCongratTime = 0;
		g_UnInitGame();
		//lb_PlayMP3("menu", gMusicVol);
		g_InitPresentation();
		return;
	}

	/*
	// grizzly saute de joie
	PlayerEnt = g_FindSprite(ENT_PLAYER, 1);
	if(PlayerEnt)
	{
		if(PlayerEnt->vely == 0)
		if((int)gCongratTime % 800 == 0)
		{
			g_AddBumperJump(PlayerEnt, PLAYER_JUMP);
		}
	}
	*/

	blit(sprites, 686, 401, 212, 250, 215, 41, true);
}

void g_GameOverProcess()
{
	gGameOverTime -= gFrameTime_msec;
	if(gGameOverTime < 0)
	{
		gGameOverTime = 0;
		g_UnInitGame();
		lb_PlayMP3("menu", gMusicVol, true);
		g_InitPresentation();
		return;
	}

	blit(sprites, 686, 363, 212, 250, 215, 37, true);
}


bool g_MakeKey(char *prenom, char *nom, char *key_code)
{
	short	i;
	long 	l;
	long	skulen;
#ifdef DEMO_VERSION
	char	skuID[MAX_PATH] = "GRIZZLOUUU";
#else
	char	skuID[MAX_PATH] = "GRIZLAILLE";
#endif
	char	compname[512], gencode[256];


	memset( gencode , 0 , 256 ) ;
	memset( compname , 0 , 512 ) ;
	
	sprintf(compname, "%s%s", nom, prenom);

	if(strlen(compname) == 0)
	{
		//MessageBox( 0 , "Invalid Code." , "Operation Failed !" , 0 ) ;		
		return false;
	}

	l = strlen( compname ) ;
	while ( l <= 8 )	
	{
		strcat( compname , nom );
		strcat( compname , prenom );
		l = strlen( compname );
	}
	
	l = strlen( compname );
	if ( l <= 8 )	
	{
		//MessageBox( 0 , "Invalid Code." , "Operation Failed !", 0 ) ;		
		return false;
	}

	_strupr( compname ) ;
	
	skulen = strlen( skuID )-1 ;
	
	for ( i = 0 ; i< l ; i++ )
	{
#ifdef DEMO_VERSION
		gencode[i] =  74+(( compname[i]*skuID[i % skulen] ) % 14 ) ;
#else
		gencode[i] =  65+(( compname[i]*skuID[i % skulen] ) % 20 ) ;
#endif
	}

	gencode[9] = '\0' ;
//	strcpy(key_code, gencode);

	if ( strncmp( gencode , key_code , 9 ) == 0 )
	{
		return true ;
	}

	return false ;
}


/*
bool g_CheckKey(char *key)
{
	int i1,i2,i3,i4;
	int divide = 10,length = 9 ;
	int	paramspe = 2 ;		//pour la version finale, mettre 0

	if(!key)
		return false;

	//i2 va contenir la somme des characteres
	i2=0;
	for(i1=0;i1<KEY_MAX;i1++)		i2+=key[i1];

	//on va ajouter tout les chiffres un à un etc..
	for(;i2>length;)
	{
		i4=0;
		for(;i2>0;)
		{
			i3=i2;
			i1=0;
			for(;i3>length;)
			{
				i3/=divide;
				i1++;
			}
			i4+=i3;
			for(;i1>0;i1--)				i3*=divide;
			i2-=i3;
		}
		i2=i4;
	}

	//on regarde si c'est la même valeur qu'a la fin..
	if((key[KEY_MAX]-48-paramspe)==i2)
		return true;
	else
		return false;
}
*/

