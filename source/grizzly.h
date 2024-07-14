
#ifndef GRIZZLY_INC__
#define GRIZZLY_INC__


#define	NAME	"Grizzly"


//#define	DEMO_VERSION
//#define	WIN_GRIZZ
#define	MAC_GRIZZ

#define	MAX_PATH	260


#ifdef MAC_GRIZZ
#define DWORD 	long
#define	BOOL	bool
#define	BYTE	unsigned char
#endif

#define		ScreenX			640
#define		ScreenY			480



void	initGame();
void	loopGame();
void	cleanupGame();


int		GRIZZMain(void);
void	GetFrameTime();
void	sys_InitMillisecond( void );
unsigned long sys_GetMilliseconds( void );
void	sys_DoGame();
bool	Complain(char *pszMessage );
void EndGame();




#endif // GRIZZLY_INC__



