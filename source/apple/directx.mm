//==========================================//
//											//
//		ROUTINES POUR LES JEUX				//
//		(C) 2002-2004 Jadeware				//
//		PTK (C) 2003 Phelios				//
//		iPTK (C) 2011 jmApp					//
//==========================================//

#include		"KPTK.h"
#include		"directx.h"

#import "KJoypad.h"


bool			fullscreen = false;
bool			shareware = true;
BOOL			KeySpacePressed = FALSE;
BOOL			KeySpacePressedJOY = FALSE;
BYTE			diks[256];
KInput			joystick;
KGraphic		*panel = NULL,
				*sprites = NULL,
				*table = NULL,
				*background = NULL,
				*paralax1 = NULL,
				*paralax2 = NULL,
				*toper = NULL,
				*monsters = NULL;
KWindow			*fenetre = NULL;



extern	float	gEffet_EarthQuake;
extern	float	gFrameTime_msec;
extern	char	appPath[MAX_PATH] ;
extern	int		gEpisodeID;
extern	long	Touche;
extern	float	MouseX;
extern	float	MouseY;
//extern	bool	IsJoypadOK;




//==============================================================================================//
//									PTK INPUT ROUTINES											//
//==============================================================================================//

bool InitializeInput(KWindow * window)
{
	KInput::initInput(ScreenX, ScreenY);
	//KInput::hidePointer();
	//IsJoypadOK = false;
	return true;
}

bool HasJoypad()
{
	bool is_joypad = (bool)[KJoypad Get_HasJoypad];
	return is_joypad;
}

long ReadJoypad()
{
	long joyTouche = [KJoypad Get_PadStatus];
	long input = 0;
	
	if(joyTouche == PAD_NONE)
	{
		
	}
	if(joyTouche & PAD_UP)
	{
		//joyTouche &= ~PAD_UP;
		input |= KEY_UP;
	}
	if(joyTouche & PAD_DOWN)
	{
		//joyTouche &= ~PAD_DOWN;
		input |= KEY_DOWN;
	}
	if(joyTouche & PAD_LEFT)
	{
		//joyTouche &= ~PAD_LEFT;
		input |= KEY_LEFT;
	}
	if(joyTouche & PAD_RIGHT)
	{
		//joyTouche &= ~PAD_RIGHT;
		input |= KEY_RIGHT;
	}
	
	if(joyTouche & PAD_BUTTON1)
	{
		input |= KEY_SPACE;
	}
	if(joyTouche & PAD_BUTTON2)
	{
		input |= KEY_RUN;
	}
	if(joyTouche & PAD_BUTTON3)
	{
		input |= KEY_FIRE;
	}
	if(joyTouche & PAD_BUTTON9)
	{
		input |= KEY_ESC;
	}
	if(joyTouche & PAD_BUTTON10)
	{
		input |= KEY_ENTER;
	}
	
	return input;
}

long ReadKeyboard()
{
	if( KInput::isPressed(K_VK_BACK ) )
		Touche |= KEY_BACK;
	else
		Touche &= ~KEY_BACK;

    if( KInput::isPressed( K_VK_F1) )
		Touche |= KEY_F1;
	else
		Touche &= ~KEY_F1;

    if( KInput::isPressed(K_VK_F2) )
		Touche |= KEY_F2;
	else
		Touche &= ~KEY_F2;

    if( KInput::isPressed(K_VK_LEFT) )
		Touche |= KEY_LEFT;
	else
		Touche &= ~KEY_LEFT;

    if( KInput::isPressed(K_VK_RIGHT) )
		Touche |= KEY_RIGHT;
	else
		Touche &= ~KEY_RIGHT;

    if( KInput::isPressed(K_VK_DOWN) )
		Touche |= KEY_DOWN;
	else
		Touche &= ~KEY_DOWN;

    if( KInput::isPressed(K_VK_UP) )
		Touche |= KEY_UP;
	else
		Touche &= ~KEY_UP;

    if( KInput::isPressed(K_VK_P) )
		Touche |= KEY_P;
	else
		Touche &= ~KEY_P;

    if( KInput::isPressed(K_VK_L_SHIFT) || KInput::isPressed(K_VK_R_SHIFT) )
		Touche |= KEY_RUN; 
	else
		Touche &= ~KEY_RUN;

	if (KInput::isPressed(K_VK_RETURN) )
		Touche |= KEY_ENTER; 
	else
		Touche &= ~KEY_ENTER;

	if( KInput::isPressed(K_VK_ESCAPE) )
		Touche |= KEY_ESC; 
	else
		Touche &= ~KEY_ESC;

	if( KInput::isPressed(K_VK_SPACE) || KInput::isPressed(K_VK_L_CONTROL) || KInput::isPressed(K_VK_R_CONTROL) )
	{
		if(!KeySpacePressed)
		{
			KeySpacePressed = TRUE;
			Touche |= KEY_SPACE;
		}
	}
	else
	{
		KeySpacePressed = FALSE;
		Touche &= ~KEY_SPACE;
	}
	return Touche;
}

int di_ReadKey()
{
	int		letter;

	letter = 'A' - 1;
	letter++; if( KInput::isPressed( K_VK_A ) == ISDOWN ) return letter;
	letter++; if( KInput::isPressed( K_VK_B ) == ISDOWN ) return letter;
	letter++; if( KInput::isPressed( K_VK_C ) == ISDOWN ) return letter;
	letter++; if( KInput::isPressed( K_VK_D ) == ISDOWN ) return letter;
	letter++; if( KInput::isPressed( K_VK_E ) == ISDOWN ) return letter;
	letter++; if( KInput::isPressed( K_VK_F ) == ISDOWN ) return letter;
	letter++; if( KInput::isPressed( K_VK_G ) == ISDOWN ) return letter;
	letter++; if( KInput::isPressed( K_VK_H ) == ISDOWN ) return letter;
	letter++; if( KInput::isPressed( K_VK_I ) == ISDOWN ) return letter;
	letter++; if( KInput::isPressed( K_VK_J ) == ISDOWN ) return letter;
	letter++; if( KInput::isPressed( K_VK_K ) == ISDOWN ) return letter;
	letter++; if( KInput::isPressed( K_VK_L ) == ISDOWN ) return letter;
	letter++; if( KInput::isPressed( K_VK_M ) == ISDOWN ) return letter;
	letter++; if( KInput::isPressed( K_VK_N ) == ISDOWN ) return letter;
	letter++; if( KInput::isPressed( K_VK_O ) == ISDOWN ) return letter;
	letter++; if( KInput::isPressed( K_VK_P ) == ISDOWN ) return letter;
	letter++; if( KInput::isPressed( K_VK_Q ) == ISDOWN ) return letter;
	letter++; if( KInput::isPressed( K_VK_R ) == ISDOWN ) return letter;
	letter++; if( KInput::isPressed( K_VK_S ) == ISDOWN ) return letter;
	letter++; if( KInput::isPressed( K_VK_T ) == ISDOWN ) return letter;
	letter++; if( KInput::isPressed( K_VK_U ) == ISDOWN ) return letter;
	letter++; if( KInput::isPressed( K_VK_V ) == ISDOWN ) return letter;
	letter++; if( KInput::isPressed( K_VK_W ) == ISDOWN ) return letter;
	letter++; if( KInput::isPressed( K_VK_X ) == ISDOWN ) return letter;
	letter++; if( KInput::isPressed( K_VK_Y ) == ISDOWN ) return letter;
	letter++; if( KInput::isPressed( K_VK_Z ) == ISDOWN ) return letter;

	return 0;
}

DWORD ReadMouse()
{
	MouseX = KInput::getMouseX();
	MouseY = KInput::getMouseY();

	Touche =  0 ;

	if( KInput::getLeftButtonState() == ISDOWN )
	{
		Touche |= KEY_FIRE; 
		Touche |= KEY_ENTER;
	}
	else
	{
		Touche &= ~KEY_FIRE;
		Touche &= ~KEY_ENTER;
	}
	return Touche;
}

DWORD ReadGameInput()
{
	return ReadKeyboard();
}

BOOL ReacquireInput()
{
	return TRUE;
}

BOOL UnacquireInput()
{
	return TRUE;
}

void CleanupInput(void)
{
	KInput::showPointer();
}



//==============================================================================================//
//									PTK GRAPHIC ROUTINES										//
//==============================================================================================//

static int bltOffset_X = 0;
static int bltOffset_Y = 0;

void sys_FlipScreen()
{
	if(gEffet_EarthQuake > 0)
	{
		gEffet_EarthQuake -= gFrameTime_msec;
		if(gEffet_EarthQuake < 0)
			gEffet_EarthQuake = 0;
		bltOffset_X = -5 + rand() % 10;
		bltOffset_Y = -5 + rand() % 10;
	}
	else
	{
		bltOffset_X = 0;
		bltOffset_Y = 0;
	}
	fenetre->flipBackBuffer();
}

void DeleteSurface(KGraphic *surface)
{
	delete	surface ;
	surface = NULL ;
}

void CleanupGraphics()
{
	DeleteSurface(panel);
	DeleteSurface(sprites);
	DeleteSurface(monsters);
	DeleteSurface(background);
	DeleteSurface(paralax1);
}

bool InitializeGraphics()
{
	bool	res;
	bool	windowed = false;

#ifdef _DEBUG
	windowed = true;
#endif

	fenetre = KPTK::createKWindow( K_MAC, ScreenX, ScreenY );
	res = fenetre->createGameWindow(ScreenX,ScreenY,32,windowed,NAME);
	//fenetre->setMaxFrameRate( 80 );
	fenetre->setClearColor(0, 0, 0, 1);

	return res;
}

bool LoadSurfaces()
{
    if(RestoreSurfaces() == false )
	{
		Complain("RestoreSurfaces Failed!");
		return false;
	}
    return true;
}

bool RestoreSurfaces()
{
	if(!panel)
    panel = DDLoadBitmap((char*)"panels.png");
    if(!panel) return Complain( "On a pas pu charger: panel.png" );

	if(!sprites)
	sprites = DDLoadBitmap((char*)"sprites.png");
    if(!sprites) return Complain( "On a pas pu charger: sprites.png" );

	if(!monsters)
	monsters = DDLoadBitmap((char*)"monsters_toppers.png");
    if(!monsters) return Complain( "On a pas pu charger: monsters.png" );

	g_LoadWorldGFX(gEpisodeID);
	//g_RestoreLevel();
    return TRUE;
}

KGraphic * DDLoadBitmap(char* szBitmap)
{
	KGraphic *kg = KPTK::createKGraphic( ) ; 
	//kg->setColorKey(true, 0, 0, 255);
	//if(kg->loadPicture( KMiscTools::makeFilePath(szBitmap), true, true ) == false)
	if(kg->loadPicture( szBitmap, true, true ) == false)
	{
		delete kg;
		kg = NULL;
	}
 	return kg ; 
}

KGraphic * DDReLoadBitmap(KGraphic *surface, char* szBitmap)
{
	if(surface)
	{
		DeleteSurface(surface);
	}
	surface = DDLoadBitmap(szBitmap);
 	return surface;
}

bool	gFlipY = false;

bool blit(KGraphic *surface1, int x1, int y1, int x2, int y2, int largeur, int hauteur, bool mode)
{
	int		i;

	if(surface1 == NULL)
		return false;

	if(gFlipY)
	{
		gFlipY = false;
		for(i=0 ; i<hauteur ; i++)
		{
//			if(mode)
			{
				surface1->blitAlphaRect(x1, y1+hauteur-i-1, x1+largeur, y1+hauteur-i, x2+bltOffset_X, y2+bltOffset_Y+i);
			}
//			else
//			{
//				surface1->blitRect(x1, y1+hauteur-i-1, x1+largeur, y1+hauteur-i, x2+bltOffset_X, y2+bltOffset_Y+i);
//			}
		}
	}
	else
	{
//		if(mode)
		{
			surface1->blitAlphaRect(x1, y1, x1+largeur, y1+hauteur, x2+bltOffset_X, y2+bltOffset_Y);
		}
//		else
//		{
//			surface1->blitRect(x1, y1, x1+largeur, y1+hauteur, x2+bltOffset_X, y2+bltOffset_Y);
//		}
	}
	return true;
}


