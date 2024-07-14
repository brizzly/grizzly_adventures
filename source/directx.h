
#ifndef GRAPHICS_INC__
#define GRAPHICS_INC__

#define INITGUID

#include <stdlib.h>
#include "KInput.h"
#include "KMiscTools.h"
#include "game.h"
#include "grizzly.h"


#ifdef WIN_GRIZZ
#include <windows.h>
#include "KGraphic.h"
#include "KWindow.h"
#else
#include "KGraphic.h"
#include "KWindow.h"
#endif



extern	bool	gFlipY;


//==========================//
//	DIRECT INPUT HEADER		//
//==========================//

#define KEY_STOP		0x00000001l
#define KEY_DOWN		0x00000002l
#define KEY_LEFT		0x00000004l
#define KEY_RIGHT		0x00000008l
#define KEY_UP			0x00000010l
#define KEY_FIRE		0x00000020l
#define KEY_V			0x00000040l	//
#define KEY_ESC			0x00000080l
#define KEY_ENTER		0x00000100l
#define KEY_RUN			0x00000200l
#define KEY_C			0x00000400l	//
#define KEY_F			0x00000800l	//
#define KEY_S			0x00001000l	//
#define	KEY_SPACE		0x00002000l
#define	KEY_CLEAR		0x00004000l
#define	KEY_P			0x00008000l
#define	KEY_F1			0x00010000l	//
#define	KEY_F2			0x00020000l	//
#define	KEY_BACK		0x00040000l

#define	KEY_LEFTJP		0x00080000l
#define	KEY_RIGHTJP		0x00100000l
#define	KEY_UPJP		0x00200000l
#define	KEY_DOWNJP		0x00400000l
#define	KEY_SPACEJP		0x00800000l
#define	KEY_PAUSEJP		0x01000000l
#define	KEY_ENTERJP		0x02000000l
#define KEY_RUNJP		0x04000000l
#define KEY_ESCJP		0x08000000l


bool					InitializeInput(KWindow * window);
bool					HasJoypad();
DWORD					ReadGameInput();
long					ReadJoypad();
long					ReadKeyboard();
int						di_ReadKey();
DWORD					ReadMouse();
BOOL					ReacquireInput();
BOOL					UnacquireInput();
void					CleanupInput(void);


//==========================//
//	GRAPHIC HEADER			//
//==========================//

bool					InitializeGraphics();
void					CleanupGraphics();
void					sys_FlipScreen();
KGraphic			*	DDLoadBitmap(char* szBitmap);
KGraphic			*	DDReLoadBitmap(KGraphic *surface, char* szBitmap);
void					DeleteSurface(KGraphic *surface);
bool					LoadSurfaces();
bool					RestoreSurfaces();
bool					blit(KGraphic *surface1, int x1, int y1, int x2, int y2, int largeur, int hauteur, bool mode);




#endif // GRAPHICS_INC__
