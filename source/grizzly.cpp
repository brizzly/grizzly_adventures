
#include "libbass.h"
#include "directx.h"
#include "grizzly.h"
#include "KPTK.h"
#include "KMiscTools.h"
#include "GameAnalytics.h"
#ifdef MAC_GRIZZ
#include "MKStore_interface.h"
#endif

float		gFrameTime_msec = 0;
float		gTotalTime_msec = 0;
float		gFPSshowed = 0;
long		Touche;
float		MouseX;
float		MouseY;
long		lastTickCount;

extern	KWindow	*fenetre;
extern	char	appPath[MAX_PATH] ;
extern	bool	gAnimatedSplashLoaded;
extern	int		gMusicVol;

extern	DWORD           ScreenBpp;
extern	bool			fullscreen;
extern	int				ProgramState;
extern	float			gLogoTime;
extern	bool			shareware;

extern KGraphic	*panel,
				*sprites,
				*table,
				*background,
				*paralax1,
				*paralax2,
				*toper,
				*monsters;



void initGame()
{
	GRIZZMain();
}

void loopGame()
{
	if(fenetre->isQuit() == true || ProgramState == PS_QUIT)
    {
		cleanupGame();
		return;
	}
	fenetre->setDefaultWorldView();
	sys_DoGame();	
}

void cleanupGame()
{
	g_SaveConfig();
	EndGame();
}


void EndGame()
{
	lb_UnInit();
	CleanupInput();
	CleanupGraphics();
	//g_UnInitGame();
	exit(0);
}

#ifdef WIN_GRIZZ

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPreInst, LPSTR lpszCmdLine, int nCmdShow)
{
	// GameAnalytics -------------------------------------

	gameanalytics::GameAnalytics::setEnabledInfoLog(true);
	gameanalytics::GameAnalytics::setEnabledVerboseLog(true);

	/*TCHAR moduleName[MAX_PATH + 1];
	GetModuleFileName(NULL, moduleName, MAX_PATH);
	DWORD dummyZero;
	DWORD versionSize = GetFileVersionInfoSize(moduleName, &dummyZero);
	if (versionSize != 0)
	{
		void* pVersion = malloc(versionSize);
		if (pVersion)
		{
			if (GetFileVersionInfo(moduleName, NULL, versionSize, pVersion))
			{*/
				const char* cfilename = (const char*)"GrizzlyPC 1.3.1"; // pVersion;
				char buildVersion[64];
				sprintf(buildVersion, "U4Mac %s", cfilename);
				gameanalytics::GameAnalytics::configureBuild(buildVersion);

				gameanalytics::GameAnalytics::initialize("b6bf44aae6e4d1843a77ffa2f0e5e02d", "419806e007af4ddf48622b487eb180d4d9da3861");

				/*free(pVersion);
			}
		}
	}*/

	return GRIZZMain();
}

#endif
								
int GRIZZMain(void)
{
	srand((unsigned int)time(0));
	KMiscTools::initMiscTools(NAME);
	g_LoadConfig();
	
#ifdef MAC_GRIZZ
	MKStoreAction(MK_ACTION_LOAD);
#endif

#ifdef DEMO_VERSION
	shareware = true;
#else
	shareware = false;
#endif

	if (InitializeGraphics() == false) {
		cleanupGame();
		return Complain("InitializeGraphics Failed!");
	}

	fenetre->setTitle(NAME);

	if (LoadSurfaces() == false) {
		cleanupGame();
		return Complain("LoadSurfaces Failed!");
	}

//	if( InitializeInput(fenetre) == false )
//		return Complain("InitializeInput Failed!");

	lb_Init();
	g_LoadSounds("");
	gAnimatedSplashLoaded = false;
	sys_InitMillisecond();
	g_InitTransition();
	g_InitLoading();

#ifdef WIN_GRIZZ
	while (1)
	{
		loopGame();
	}
#endif
	
    return 0;
}

void sys_DoGame()
{
	gFrameTime_msec = 16.66667f;
	/*
	#ifdef MAC_GRIZZ
	gFrameTime_msec = 16.66667f;
	#else
	KMiscTools::getMilliseconds();
	#endif
	*/
	
	//GetFrameTime();

	g_UpdateFrame();
	//g_DrawString(0,0,"gFrameTime_msec: %d", (int)gFrameTime_msec);
	sys_FlipScreen();
}

void sys_InitMillisecond( void ) 
{
} 

//#ifdef WIN_GRIZZ
//LARGE_INTEGER _unitsPerSecond;
//#endif

unsigned long sys_GetMilliseconds( void ) 
{
	return KMiscTools::getMilliseconds();
	/*
#ifdef WIN_GRIZZ
	LARGE_INTEGER performanceCount ; 
	double microseconds; 
  
	if( QueryPerformanceCounter( &performanceCount ) == false )
		return 0;

	microseconds = (double)performanceCount.LowPart  /  (double)_unitsPerSecond.LowPart;
	return (long)(microseconds*1000);
#else
	return clock();
#endif
	*/
}


/*
double g_GetClock()
{
	#ifdef WIN_GRIZZ
	return clock();
	#endif

	#ifdef MAC_GRIZZ
	return KMiscTools::getMilliseconds();
	#endif

	//	return timeGetTime();
//	return (double)sys_GetMilliseconds();
}
*/

void GetFrameTime()
{
	static	double			start = -1;
	static	long			nbFrame = 0;
	double					frametime;
	float					FPS;

	nbFrame++;
	if(start == -1)
		start = sys_GetMilliseconds();

	frametime = (sys_GetMilliseconds() - start)/(double)nbFrame;
	gFrameTime_msec = (float)frametime;

	gTotalTime_msec += gFrameTime_msec;
	FPS = 1000.0f/gFrameTime_msec;

	if(gTotalTime_msec > 100) // 250 1000
	{
		gFPSshowed = FPS;
		gTotalTime_msec=0;
		nbFrame = 0;
		start = sys_GetMilliseconds();
	}	
}

bool Complain(char *pszMessage)
{
	//KMiscTools::messageBox("Problem" , pszMessage);
	printf("%s\n", pszMessage);
	return false;
}


