
#ifndef GAME_INC__
#define GAME_INC__


#define	TILE_X		48
#define	TILE_Y		48
#define	MAX_TILES	120


#include "grizzly.h"

#ifdef WIN_GRIZZ
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "KSound.h"
#include "libbass.h"
#include "level.h"
#include "player.h"
#include "world.h"
#include "sprite_process.h"
#include "monster.h"



#define KEY_MAX			12
#define	MAXPLAYERNAME	255
#define	GAME_MAXLEVELS	10
#define	MAXDEMOLEVELS	5 // 3
#define	MUSICFADE		0.12f
#define	MUSICFADE_MIN	0
#define	CONFIGFILEPATH	"grizzly.cfg"
#define	PLAYER_JUMP		8.2f
#define	PLAYER_VEL		0.25f //0.20f
#define	PLAYERJMP_T		75.0f
#define	LOSTLIVE_T		2500.0f
#define	EARTHQUAKE_T	300.0f
#define	CONFIGHEADER	"AK"
#define	CONFIGHEADER_E	"ак"
#define	BZZ_TIME		1000.0f
#define	POMMEDISP_T		2500.0f
#define	INVISIBLE_T		7000.0f
#define	GAME_TRANSITION	0.30f
#define	GAME_TRANSWAIT	1200.0f
#define	MONS_FATIGUE_T	3000.0f



// definition des tiles
#define	VOID_ID			0
#define	MUR_ID			1
#define	SMUR_ID			2
#define	PORTE_ID		5
#define	CHECKPOINT_ID	6
#define	HLIFT_ID		8
#define	BUMPER_ID		9
#define	SPECIAL_ID		15
#define	PVERTE_ID		16
#define	POMME_ID		19
#define	POW_ID			25
#define	PJAUNE_ID		26
#define	RLIFT_ID		28
#define	BANANE_ID		29
#define	LIFEUP_ID		38
#define	EXIT_ID			39
#define	EXITAD_ID		48
#define	DIAMANT_ID		49
#define	KEY_ID			58
#define	MONSTER1_ID		59
#define	PIC_ID			70
#define	PLAYER_ID		79
#define	MONSTER2_ID		89
#define	MONSTER3_ID		69
#define	MONSTER2b_ID	99
#define	MONSTER6_ID		108
#define	MONSTER2c_ID	109
#define	MONSTER5_ID		118
#define	MONSTER4_ID		119



#define	TITLE_X			664
#define	TITLE_Y			82
#define	TITLE_W			350
#define	TITLE_H			133

#define	PRESSENTER_X	85
#define	PRESSENTER_Y	23
#define	PRESSENTER_W	200
#define	PRESSENTER_H	25

#define NOGAMEPAD_X		437
#define NOGAMEPAD_Y		107
#define NOGAMEPAD_W		64
#define NOGAMEPAD_H		54

#define	SMILEY_X		478
#define	SMILEY_Y		80
#define	SMILEY_W		23
#define	SMILEY_H		25

#define	PRESSUP_X		289
#define	PRESSUP_Y		1
#define	PRESSUP_W		153
#define	PRESSUP_H		25

#define	NEEDKEY_X		289
#define	NEEDKEY_Y		27
#define	NEEDKEY_W		153
#define	NEEDKEY_H		25

#define	FONT_X			8
#define	FONT_Y			66

#define PAUSE_X			904
#define	PAUSE_Y			363
#define	PAUSE_W			115
#define	PAUSE_H			37

#define NUM_X			0
#define	NUM_Y			1
#define	NUM_FW			27
#define	NUM_FH			21

#define LIFELOGO_X		967
#define	LIFELOGO_Y		4
#define	LIFELOGO_FW		56
#define	LIFELOGO_FH		18

#define MENU1_X			117
#define	MENU1_Y			248
#define	MENU1_W			141
#define	MENU1_H			110

#define MENU2_X			481
#define	MENU2_Y			220
#define	MENU2_W			140
#define	MENU2_H			118

#define MENU3_X			145
#define	MENU3_Y			362
#define	MENU3_W			529
#define	MENU3_H			136

#define MENU4_X			623
#define	MENU4_Y			256
#define	MENU4_W			391
#define	MENU4_H			33

#define LEVEL_X			683
#define	LEVEL_Y			469
#define	LEVEL_W			282
#define	LEVEL_H			26

#define CURSOR_X		517
#define	CURSOR_Y		81
#define	CURSOR_W		23
#define	CURSOR_H		21

#define SUN_X			549
#define	SUN_Y			2
#define	SUN_W			102
#define	SUN_H			100

#define JADE_X			465
#define	JADE_Y			1
#define	JADE_W			79
#define	JADE_H			61

#define CREDIT_X		283
#define	CREDIT_Y		229
#define	CREDIT_W		189
#define	CREDIT_H		124


#define	PLAYER_X		6
#define	PLAYER_Y		518
#define	PLAYER_FRAME	11
#define	PLAYER_FTIME	45
#define	PLAYER_FW		38
#define	PLAYER_FH		64
#define	PLAYER_CW		36
#define	PLAYER_CH		62

#define	MONSTER1_X		5
#define	MONSTER1_Y		3
#define	MONSTER1_FRAME	8
#define	MONSTER1_FTIME	45
#define	MONSTER1_FW		40
#define	MONSTER1_FH		40
#define	MONSTER1_CW		40
#define	MONSTER1_CH		38

#define	MONSTER2_X		224 //8
#define	MONSTER2_Y		394 //105
#define	MONSTER2_FRAME	4//1
#define	MONSTER2_FTIME	30
#define	MONSTER2_FW		78
#define	MONSTER2_FH		56
#define	MONSTER2_CW		74
#define	MONSTER2_CH		52

#define	MONSTER3_X		8
#define	MONSTER3_Y		227
#define	MONSTER3_FRAME	12
#define	MONSTER3_FTIME	40
#define	MONSTER3_FW		50
#define	MONSTER3_FH		40
#define	MONSTER3_CW		15
#define	MONSTER3_CH		38

#define	MONSTER4_X		93
#define	MONSTER4_Y		94
#define	MONSTER4_FRAME	10
#define	MONSTER4_FTIME	50
#define	MONSTER4_FW		47
#define	MONSTER4_FH		62
#define	MONSTER4_CW		34
#define	MONSTER4_CH		50

#define	MONSTER5_X		586
#define	MONSTER5_Y		4
#define	MONSTER5_FRAME	1
#define	MONSTER5_FTIME	50
#define	MONSTER5_FW		46
#define	MONSTER5_FH		60
#define	MONSTER5_CW		40
#define	MONSTER5_CH		50

#define	MONSTER6_X		17
#define	MONSTER6_Y		396
#define	MONSTER6_FRAME	4
#define	MONSTER6_FTIME	110
#define	MONSTER6_FW		48
#define	MONSTER6_FH		40
#define	MONSTER6_CW		40
#define	MONSTER6_CH		32

#define	PANG_X			61
#define	PANG_Y			160
#define	PANG_FRAME		1
#define	PANG_FTIME		10
#define	PANG_FW			39
#define	PANG_FH			42
#define	PANG_CW			39
#define	PANG_CH			42

#define	HLIFT_X			10
#define	HLIFT_Y			23
#define	HLIFT_FRAME		1
#define	HLIFT_FTIME		40
#define	HLIFT_FW		70
#define	HLIFT_FH		34
#define	HLIFT_CW		70
#define	HLIFT_CH		34

#define	LIFEUP_X		986
#define	LIFEUP_Y		38
#define	LIFEUP_FRAME	1
#define	LIFEUP_FTIME	40
#define	LIFEUP_FW		24
#define	LIFEUP_FH		20
#define	LIFEUP_CW		24
#define	LIFEUP_CH		20

#define	SMOKE_X			258
#define	SMOKE_Y			81
#define	SMOKE_FRAME		9
#define	SMOKE_FTIME		60
#define	SMOKE_FW		16
#define	SMOKE_FH		16
#define	SMOKE_CW		16
#define	SMOKE_CH		16

#define	KEY_X			9
#define	KEY_Y			257
#define	KEY_FRAME		1
#define	KEY_FTIME		666
#define	KEY_FW			32
#define	KEY_FH			32
#define	KEY_CW			32
#define	KEY_CH			32

#define	DIAMANT_X		16
#define	DIAMANT_Y		219
#define	DIAMANT_FRAME	1
#define	DIAMANT_FTIME	666
#define	DIAMANT_FW		24
#define	DIAMANT_FH		26
#define	DIAMANT_CW		22
#define	DIAMANT_CH		24

#define	STAR_X			17
#define	STAR_Y			105
#define	STAR_FRAME		1
#define	STAR_FTIME		666
#define	STAR_FW			13
#define	STAR_FH			14
#define	STAR_CW			13
#define	STAR_CH			14

#define	REDSTAR_X		17
#define	REDSTAR_Y		120
#define	REDSTAR_FRAME	1
#define	REDSTAR_FTIME	666
#define	REDSTAR_FW		13
#define	REDSTAR_FH		14
#define	REDSTAR_CW		13
#define	REDSTAR_CH		14

#define	GREENSTAR_X		17
#define	GREENSTAR_Y		135
#define	GREENSTAR_FRAME	1
#define	GREENSTAR_FTIME	666
#define	GREENSTAR_FW	13
#define	GREENSTAR_FH	14
#define	GREENSTAR_CW	13
#define	GREENSTAR_CH	14

#define	EXITAD_X		39
#define	EXITAD_Y		102
#define	EXITAD_FRAME	1
#define	EXITAD_FTIME	666
#define	EXITAD_FW		80
#define	EXITAD_FH		55
#define	EXITAD_CW		80
#define	EXITAD_CH		55

#define	POMME_X			9
#define	POMME_Y			344
#define	POMME_FRAME		1
#define	POMME_FTIME		40
#define	POMME_FW		28
#define	POMME_FH		30
#define	POMME_CW		26
#define	POMME_CH		28

#define	POMMEJ_X		14
#define	POMMEJ_Y		382
#define	POMMEJ_FRAME	1
#define	POMMEJ_FTIME	40
#define	POMMEJ_FW		28
#define	POMMEJ_FH		30
#define	POMMEJ_CW		26
#define	POMMEJ_CH		28

#define	POMMEV_X		11
#define	POMMEV_Y		416
#define	POMMEV_FRAME	1
#define	POMMEV_FTIME	40
#define	POMMEV_FW		28
#define	POMMEV_FH		30
#define	POMMEV_CW		26
#define	POMMEV_CH		28

#define	BANANE_X		9
#define	BANANE_Y		291
#define	BANANE_FRAME	1
#define	BANANE_FTIME	40
#define	BANANE_FW		29
#define	BANANE_FH		27
#define	BANANE_CW		29
#define	BANANE_CH		27

#define	FEUILLE_X		9
#define	FEUILLE_Y		320
#define	FEUILLE_FRAME	1
#define	FEUILLE_FTIME	40
#define	FEUILLE_FW		19
#define	FEUILLE_FH		22
#define	FEUILLE_CW		19
#define	FEUILLE_CH		22

#define	BUMPER_X		137
#define	BUMPER_Y		113
#define	BUMPER_FRAME	6
#define	BUMPER_FTIME	40
#define	BUMPER_FW		48
#define	BUMPER_FH		48
#define	BUMPER_CW		48
#define	BUMPER_CH		48

#define	POTMIEL_X		8
#define	POTMIEL_Y		213
#define	POTMIEL_FRAME	12
#define	POTMIEL_FTIME	50
#define	POTMIEL_FW		48
#define	POTMIEL_FH		48
#define	POTMIEL_CW		48
#define	POTMIEL_CH		48

#define	COFFRE_X		8
#define	COFFRE_Y		159
#define	COFFRE_FRAME	1
#define	COFFRE_FTIME	30
#define	COFFRE_FW		48
#define	COFFRE_FH		48
#define	COFFRE_CW		48
#define	COFFRE_CH		48

#define CADENAS_X		517
#define CADENAS_Y		103
#define CADENAS_W		22
#define CADENAS_H		26

#define THEME1_X		654
#define THEME1_Y		1
#define THEME1_W		300
#define THEME1_H		80

#define THEME2_X		695
#define THEME2_Y		509
#define THEME2_W		300
#define THEME2_H		80





struct sHeaderGrizzly
{
	int	level_w;
	int	level_h;

};

struct sBlocGrizzly
{ 
	unsigned short	blocID;
	short			action;
 
};

typedef struct scrtype
{
	int		score;
	char	name[MAXPLAYERNAME+1];

} Score_t;


enum
{
	PS_LOADING,
	PS_PRES,
	PS_HELP,
	PS_OPTION,
	PS_EPISODE,
	PS_GETREADY,
	PS_ACTIVE,
	PS_PAUSE,
	PS_WINLEVEL,
	PS_CONGRAT,
	PS_LOSTLIVE,
	PS_ENTERYOURNAME,
	PS_GAMEOVER,
	PS_CREDIT,
	PS_QUIT
};


void g_LoadSounds(char *FolderPath);
void g_Codage(char *nom_fichier, short mode);
char* game_GetConfigPath();
void g_LoadConfig();
void g_SaveConfig();
void g_MenuSys(DWORD input, int X_offset, int Y_offset, int nMaxItemX, int nMaxItemY);
void g_InitLoading();
void g_InitPresentation();
void g_InitOptions();
void g_InitSelectEpisode();
void g_InitQuit();
void g_InitGame();
bool g_ReInitGame();
void g_UnInitGame();
void g_DrawNum(int x, int y, int number);
void g_LostLive();
void g_InitLostLive();
void g_AfficheHUD();
void g_UpdateFrame();
void g_DispDebugInfos();
void g_DrawString(int x, int y, char *string, ...);
void g_AddSpecialBlocItem(int objid, int x, int y);
void g_LoadWorldGFX(int world_id);
void g_DispSharewareWarning();
bool g_DrawScene();
void g_GetVelocityDir(float *velx, float *vely, float posx, float posy, float destx, float desty);
void g_AddCompteur_Pomme(int n_pomme);
void g_AddParticles(int partcle_id, int x, int y);
void g_ShowItemcounter(int item_id);
void g_EndLevelHit();
void g_DrawLoadingLogo();
void g_DrawSplashPres();
void g_OscilleCurseur();
void g_EndLevel_Process();
void g_GameOverProcess();
void g_CongratProcess();
void g_InitCongratulation();
void g_WaitKeyReleased(int key);
bool g_MakeKey(char *prenom, char *nom, char *key_code);
void g_PauseProcess(long kb_input);
void g_InitCheckPoints();
void g_DrawTitle();
void g_DrawGamepadIcon();
void g_DrawSmiley();
void g_PlayMusic();




void g_ScreenTransition_Process();
void g_StartTransition_Open();
void g_StartTransition_Close();
bool g_IsTransitionDone_Done();
bool g_IsTransitionDone_Close();
bool g_IsTransitionDone_Open();
void g_InitTransition();







#endif GAME_INC__
