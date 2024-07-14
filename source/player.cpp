
#include "game.h"
#include "sprite.h"
#include "world.h"
#include "sprite_process.h"
#include "collision.h"
#include "directx.h" // 
#include "player.h"


extern	float	gFrameTime_msec;
extern	bool	gEndLevelHit;
extern	bool	gIsDWscroll;
extern	bool	gIsUPscroll;
extern	bool	gIsLFscroll;
extern	bool	gIsRGscroll;
extern	bool	gJumpKeyPressed;
extern	bool	gIsHeroCanJump;
extern	int		gSoundVol;
extern	int		gClee;
extern	float	gJumpTime;
extern	bool	gGDlastdir;
extern	float	gInvinsibleTime;
extern	int		gOffset_X;
extern	sHeaderGrizzly	gHeader;
extern	pEnt	gEntityList;
extern	int		gNinvinsibleStars;
extern	int		ProgramState;
extern	bool	gPlayerRun;
extern	KGraphic	*sprites, *monsters;

static float accel_left_x = 0;
static float accel_right_x = 0;


bool player_AutoCadreX(pEnt PlayerEnt, DWORD input)
{
	bool	IsScroll;
	int		scroll_xmax = TILE_X*gHeader.level_w - ScreenX;
	float	xpos;

	if(!PlayerEnt)
		return false;
	IsScroll = false;
	xpos = PlayerEnt->x - ScreenX/2.8f;
	if(xpos > 0)
//	if(PlayerEnt->x > ScreenX/2 /*&& ((input & KEY_RIGHT || input & KEY_RIGHTJP) || PlayerEnt->OnLift)*/)
	{
		if(gOffset_X < scroll_xmax)
		{
			IsScroll = true;
	//		g_Scroll_Right(gFrameTime_msec * SCROLL_SPEED);
			g_Scroll_Right(gFrameTime_msec * 0.002f*xpos);
		}		
	}
//	else if(PlayerEnt->x < ScreenX/3 && ((input & KEY_LEFT || input & KEY_LEFTJP) || PlayerEnt->OnLift))
	else if(PlayerEnt->x < ScreenX/3 )
	{
		if(gOffset_X > 0)
		{
			IsScroll = true;
	//		g_Scroll_Left(gFrameTime_msec * SCROLL_SPEED);
			g_Scroll_Left(gFrameTime_msec * 0.002f*(ScreenX/3 - PlayerEnt->x));
		}		
	}
	return IsScroll;
}

bool player_AutoCadreY(pEnt PlayerEnt)
{
	float	ScrollSpeed;
	bool	IsScroll;

	if(!PlayerEnt)
		return false;

	ScrollSpeed = gFrameTime_msec * SCROLL_SPEED;
	ScrollSpeed = player_CadreY(PlayerEnt, ScrollSpeed);

	IsScroll = false;
	if(gIsDWscroll)
	{
		g_Scroll_Down(ScrollSpeed);
		if(gIsDWscroll)
			IsScroll = true;
	}

	if(gIsUPscroll)
	{
		// hack : accélere le cadrage vertical sur les extreme (haut)
		if(PlayerEnt->y <= ScreenY/6)
			ScrollSpeed *= 3.0f;
		else if(PlayerEnt->y <= ScreenY/4)
			ScrollSpeed *= 2.0f;

		g_Scroll_Up(ScrollSpeed);
		if(gIsUPscroll)
			IsScroll = true;
	}

	return IsScroll;
}

float player_CadreY(pEnt PlayerEnt, float ScrollSpeed)
{
	if(!PlayerEnt)
		return 0;

	if(PlayerEnt->vely >= 0)
	{
		gIsUPscroll = false;

		if(PlayerEnt->y > ScreenY/2)
		{
			gIsDWscroll = true;
			ScrollSpeed += 0.05f * (PlayerEnt->y - ScreenY/2);
		}
		else if(PlayerEnt->y < 2*ScreenY/3)
		{
			gIsDWscroll = false;
		}
	}
	else
	{
		gIsDWscroll = false;

		if(PlayerEnt->y < ScreenY/3)
		{
			gIsUPscroll = true;
		}
		else if(PlayerEnt->y > ScreenY/2)
		{
			gIsUPscroll = false;
		}
	}
	return ScrollSpeed;
}

void player_Update(DWORD input)
{
	pEnt	PlayerEnt;
	float	MoveStep;
	int		middle;

	// sun 21 oct 04
	if(gEndLevelHit)
		return;

	PlayerEnt = g_FindSprite(ENT_PLAYER,1);
	if(!PlayerEnt)
		return;
	
	g_UpdatePlayer_ItemCollision(PlayerEnt);	// regarde les collisions avec les objets de la map
	player_DrawCircleStars();					// effet d'invinsibilité si c'est activé
	
	player_AutoCadreX(PlayerEnt, input);		//	scrolling forcé de la caméra en X
//	player_AutoCadreY(PlayerEnt);				//	scrolling forcé de la caméra en Y


#if 1
	// scrolling fluide de la caméra
	#define	CAM_AROUND	10

	if(!gIsDWscroll && !gIsUPscroll)
	{
		if(PlayerEnt->y > CAM_AROUND + ScreenY/2)
		{
		//	g_Scroll_Down(gFrameTime_msec * SCROLL_SLOWSPD);
			g_Scroll_Down(gFrameTime_msec * 0.006f * (PlayerEnt->y - (CAM_AROUND + ScreenY/2)));
		}
		else if(PlayerEnt->y < -CAM_AROUND + ScreenY/2)
		{
		//	g_Scroll_Up(gFrameTime_msec * SCROLL_SLOWSPD);
			g_Scroll_Up(gFrameTime_msec * 0.002f * ((-CAM_AROUND + ScreenY/2) - PlayerEnt->y));
		}
	}

#endif



	// JUMP PLAYER ////////////////////////////////////////
	if(input & KEY_SPACE || input & KEY_SPACEJP)
	{
		if(!gJumpKeyPressed)
		{
			gJumpKeyPressed = true;
			if(gIsHeroCanJump)
			{
				lb_PlaySound(7, gSoundVol);
				gIsHeroCanJump = false;
				g_AddBumperJump(PlayerEnt, PLAYER_JUMP);
				gJumpTime = PLAYERJMP_T;
			}
		}
		else
		{
			if(PlayerEnt->vely < 0)
			{
				gJumpTime -= gFrameTime_msec * 0.3f;
				if(gJumpTime < 0)
					gJumpTime = 0;
			}
		}
	}
	else
	{
		gJumpKeyPressed = false;
	}


	// breathing animation
	if(!gGDlastdir)
	{
		PlayerEnt->dir = 2;
	}
	else
	{
		PlayerEnt->dir = 3;
	}


	//
	// Gestion des deplacements gauche-droite du joueur
	//
	MoveStep = gFrameTime_msec * PLAYER_VEL;

	
	if(input & KEY_RUN || input & KEY_RUNJP)
	{
		gPlayerRun = true;
		MoveStep *= 1.5f;
	}
	else
	{
		gPlayerRun = false;
	}

	middle = TILE_X/2;
	if(MoveStep > middle)
		MoveStep = (float)middle;

	
	
	bool go_left = true;
	
	if(input & KEY_LEFT || input & KEY_LEFTJP)
	{
		accel_left_x += gFrameTime_msec * 0.004;
		if(accel_left_x > 1) {
			accel_left_x = 1;
		}
	}
	else
	{
		accel_left_x -= gFrameTime_msec * 0.004;
		if(accel_left_x <= 0) {
			accel_left_x = 0;
			go_left = false;
		}
	}
	if(go_left == true)
	{
		MoveStep *= accel_left_x;
		gGDlastdir = true;
		if(player_MoveLeft(MoveStep, PlayerEnt))
		{
			PlayerEnt->dir = 1;
		}
		else
		{
			accel_left_x = 0;
		}
	}
	
	
	bool go_right = true;
	
	if(input & KEY_RIGHT || input & KEY_RIGHTJP)
	{
		accel_right_x += gFrameTime_msec * 0.004;
		if(accel_right_x > 1) {
			accel_right_x = 1;
		}
	}
	else
	{
		accel_right_x -= gFrameTime_msec * 0.004;
		if(accel_right_x <= 0) {
			accel_right_x = 0;
			go_right = false;
		}
	}
	if(go_right == true)
	{
		MoveStep *= accel_right_x;
		gGDlastdir = false;
		if(player_MoveRight(MoveStep, PlayerEnt))
		{
			PlayerEnt->dir = 0;
		}
		else
		{
			accel_right_x = 0;
		}
	}
	
	
	
	// Gestion de la tauche HAUT pour passer les portes
	if(g_MordTile((int)PlayerEnt->x+PLAYER_FW/2, (int)PlayerEnt->y+PLAYER_FH/2) == PORTE_ID)
	{
		if(gClee <= 0)
		{
			blit(sprites, NEEDKEY_X, NEEDKEY_Y, (ScreenX-NEEDKEY_W)/2, ScreenY - NEEDKEY_H - 8, NEEDKEY_W, NEEDKEY_H, true);
		}
		else
		{
			blit(sprites, PRESSUP_X, PRESSUP_Y, (ScreenX-PRESSUP_W)/2, ScreenY - PRESSUP_H - 8, PRESSUP_W, PRESSUP_H, true);
		}
		g_ShowItemcounter(2);
		if(input & KEY_UP || input & KEY_UPJP)
		if(gClee)		
		{
			PlayerEnt->vely = 0;
			g_EndLevelHit();
		}
	}

	// frame de saut du grizzly
	if(PlayerEnt->vely != 0)
	{
		if(!gGDlastdir)
		{
			PlayerEnt->dir = 4;
		}
		else
		{
			PlayerEnt->dir = 5;
		}
	}
}


bool player_MoveLeft(float MoveOffset, pEnt PlayerEnt)
{
	int	tile_val;

	if(!PlayerEnt)
		return false;

	tile_val = g_MurGauche(	(int)PlayerEnt->x + (PlayerEnt->sprite_w - PlayerEnt->sprite_cw)/2,
							(int)PlayerEnt->y + (PlayerEnt->sprite_h - PlayerEnt->sprite_ch)/2,
							PlayerEnt->sprite_cw,
							PlayerEnt->sprite_ch);

	if(tile_val == 0 || tile_val == SMUR_ID)
	{
		PlayerEnt->x -= MoveOffset;
		return true;
	}
	else
	{
		g_FixeGauche(&PlayerEnt->x, (PlayerEnt->sprite_w - PlayerEnt->sprite_cw)/2);
	}
	return false;
}

bool player_MoveRight(float MoveOffset, pEnt PlayerEnt)
{
	int	tile_val;

	if(!PlayerEnt)
		return false;

	tile_val = g_MurDroite(	(int)PlayerEnt->x + (PlayerEnt->sprite_w - PlayerEnt->sprite_cw)/2,
							(int)PlayerEnt->y + (PlayerEnt->sprite_h - PlayerEnt->sprite_ch)/2,
							PlayerEnt->sprite_cw,
							PlayerEnt->sprite_ch);

	if(tile_val == 0 || tile_val == SMUR_ID)
	{
		PlayerEnt->x += MoveOffset;
		return true;
	}
	else
	{
		g_FixeDroite(&PlayerEnt->x, PlayerEnt->sprite_cw);
	}
	return false;
}

bool player_MoveUp(float MoveOffset, pEnt PlayerEnt)
{
	int	tile_val;

	if(!PlayerEnt)
		return false;

	tile_val = g_MurHaut(	(int)PlayerEnt->x + (PlayerEnt->sprite_w - PlayerEnt->sprite_cw)/2,
							(int)PlayerEnt->y + (PlayerEnt->sprite_h - PlayerEnt->sprite_ch)/2,
							PlayerEnt->sprite_cw,
							PlayerEnt->sprite_ch,
							MUR_ID);

	if(tile_val == 0 || tile_val == SMUR_ID)
	{
		PlayerEnt->y -= MoveOffset;
		return true;
	}
	else
	{
		g_FixeHaut(&PlayerEnt->x, (PlayerEnt->sprite_w - PlayerEnt->sprite_cw)/2);
	}
	return false;
}

bool player_MoveDown(float MoveOffset, pEnt PlayerEnt)
{
	int	tile_val;

	if(!PlayerEnt)
		return false;

	tile_val = g_MurBas(	(int)PlayerEnt->x + (PlayerEnt->sprite_w - PlayerEnt->sprite_cw)/2,
							(int)PlayerEnt->y + (PlayerEnt->sprite_h - PlayerEnt->sprite_ch)/2,
							PlayerEnt->sprite_cw,
							PlayerEnt->sprite_ch,
							MUR_ID);

	if(tile_val == 0 || tile_val == SMUR_ID)
	{
		PlayerEnt->y += MoveOffset;
		return true;
	}
	else
	{
		g_FixeBas(&PlayerEnt->x, PlayerEnt->sprite_cw);
	}
	return false;
}

void player_MakeInvinsible()
{
	gInvinsibleTime = INVISIBLE_T;
	gNinvinsibleStars = 6;
}

void player_DrawCircleStarsPos(int x, int y, float *sinus_x)
{
	float	angle;
	float	star_x;
	float	star_y;
	int		nStars;

	nStars = gNinvinsibleStars;
	if(nStars <= 0)
	{
		gInvinsibleTime = 0;
		return;
	}
	for(angle=0 ; angle<6.28f ; angle+=6.28f/nStars)
	{
		star_x = x + 45.0f * (float)cos((double)(angle+*sinus_x));
		star_y = y + 45.0f * (float)sin((double)(angle+*sinus_x));
		blit(sprites, STAR_X, STAR_Y, (int)star_x, (int)star_y, STAR_FW, STAR_FH, true);
	}
	*sinus_x += (gInvinsibleTime * 0.000001f * gFrameTime_msec);
	if(*sinus_x >= 6.28f/nStars)
		*sinus_x -= 6.28f/nStars;
}

void player_DrawCircleStars()
{
	pEnt	PlayerEnt;
	float	center_x;
	float	center_y;
//	float	angle;
//	float	star_x;
//	float	star_y;
//	int		nStars;

	// hack
	if(ProgramState == PS_CONGRAT)
	{
		gNinvinsibleStars = 6;
		gInvinsibleTime = INVISIBLE_T/2;
	}

	if(gInvinsibleTime <= 0)
		return;
	gInvinsibleTime -= gFrameTime_msec;
	if(gInvinsibleTime < 0)
	{
		gInvinsibleTime = 0;
	}

	PlayerEnt = g_FindSprite(ENT_PLAYER, 1);
	if(!PlayerEnt)
		return;
	center_x = PlayerEnt->x+PLAYER_FW/2;
	center_y = PlayerEnt->y+PLAYER_FH/2;

	player_DrawCircleStarsPos((int)center_x, (int)center_y, &PlayerEnt->sinus_x);

	/*
	nStars = gNinvinsibleStars;
	if(nStars <= 0)
	{
		gInvinsibleTime = 0;
		return;
	}
	for(angle=0 ; angle<6.28f ; angle+=6.28f/nStars)
	{
		star_x = center_x + 45.0f * (float)cos((double)(angle+PlayerEnt->sinus_x));
		star_y = center_y + 45.0f * (float)sin((double)(angle+PlayerEnt->sinus_x));
		blit(sprites, STAR_X, STAR_Y, (int)star_x, (int)star_y, STAR_FW, STAR_FH, true);
	}

	PlayerEnt->sinus_x += (gInvinsibleTime * 0.000001f * gFrameTime_msec);
	if(PlayerEnt->sinus_x >= 6.28f/nStars)
		PlayerEnt->sinus_x -= 6.28f/nStars;
	*/
}

void player_MakeSuperBumpers()
{
	pEnt	Ent;

	for(Ent=gEntityList ; Ent ; Ent=Ent->Next)
	{
		if(Ent->sprite_id != ENT_BUMPER)
			continue;
		if(Ent->x >= 0 && Ent->x <= ScreenX)
		if(Ent->y >= 0 && Ent->y <= ScreenY)
		{
			g_AddParticles(ENT_GREENSTAR, (int)Ent->x+Ent->sprite_w/2, (int)Ent->y+Ent->sprite_h/2);
			Ent->dir = 1;
		}
	}
}
