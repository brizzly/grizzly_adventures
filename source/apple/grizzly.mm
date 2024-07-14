
#include "libbass.h"
#include "directx.h"
#include "grizzly.h"
#include "KPTK.h"
#include "KMiscTools.h"
#include "MKStore_interface.h"
//#include "GameAnalytics.h"

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
	return GRIZZMain();
}

#endif
								
int GRIZZMain(void)
{
	KMiscTools::initMiscTools(NAME);
	
	g_LoadConfig();
	
	MKStoreAction(MK_ACTION_LOAD);


#ifdef MAC_GRIZZ
	#ifdef DEMO_VERSION
		shareware = true;
	#else
		shareware = false;
	#endif
#else
//#ifdef WIN_UX
	#ifdef DEMO_VERSION
		shareware = true;
	#else
		hWndDialog = CreateDialog (ghInstance,MAKEINTRESOURCE(IDD_DIALOG1),NULL,DialogProc);
		ShowWindow(hWndDialog, true);

		while(gEndDialog == false)
		{
			if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				//if(IsDialogMessage(hWndDialog, &msg) == false)
				{
					if(msg.message==WM_QUIT)
						break;
					else
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}
			}
			else
			{
			}
		}
		ShowWindow(hWndDialog, false);
		EnableWindow(hWndDialog, false);
		//CloseWindow(hWndDialog);
		DestroyWindow(hWndDialog);

		if(g_MakeKey(bufferFirstName, bufferLastName, bufferKey))
		{
			shareware = false;
		}
		else
		{
			//shareware = true;
			goto ending;
		}
	#endif
//#endif
#endif

	// GameAnalytics -------------------------------------
/*
	gameanalytics::GameAnalytics::setEnabledInfoLog(true);
	gameanalytics::GameAnalytics::setEnabledVerboseLog(true);
	
	NSString * version = [[[NSBundle mainBundle] infoDictionary] valueForKey:@"CFBundleShortVersionString"];
	const char *cfilename=[version UTF8String];
	char buildVersion[64];
	sprintf(buildVersion, "GrizzlyMac %s", cfilename);
	gameanalytics::GameAnalytics::configureBuild(buildVersion);
	
	gameanalytics::GameAnalytics::initialize("200ea2ae986995826cf931f3a23ab634", "a3aba471299ad18f54747910aba8b61d0ddbcea5");
*/
	

    if( false == InitializeGraphics() )
		return Complain("InitializeGraphics Failed!");

	fenetre->setTitle(NAME);

    if( LoadSurfaces() == false )
		return Complain("LoadSurfaces Failed!");
   

	if( InitializeInput(fenetre) == false )				
		return Complain("InitializeInput Failed!");

	lb_Init();
	g_LoadSounds("");
	gAnimatedSplashLoaded = false;
	sys_InitMillisecond();
	g_InitTransition();
	g_InitLoading();
	
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


