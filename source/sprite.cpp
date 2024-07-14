

#include "game.h"
#include "world.h"
#include "player.h"
#include "collision.h"
#include "directx.h"
#include "sprite_process.h"
#include "sprite.h"


pEnt	gEntityList = NULL;
int		gTileCollisionID;
int		gTilePos;


extern	int		ProgramState;
extern	float	gFrameTime_msec;
extern	sBlocGrizzly	*gLevel;
extern	sHeaderGrizzly	gHeader;
extern	int		gOffset_X;
extern	int		gOffset_Y;
extern	int		gDebugVal;
extern	int		gBlocDefinition[MAX_TILES];
extern	bool	gIsHeroCanJump;
extern	bool	gJumpKeyPressed;
extern	BOOL	gIsDWscroll;
extern	BOOL	gIsUPscroll;
extern	float	gJumpTime;
extern	int		gVie;
extern	int		level_id;
extern	int		gSoundVol;
extern	int		gClee;
extern	float	gInvinsibleTime;
extern	bool	gPanelIsActive;
extern	int		gEpisodeID;
extern	bool	gPlayerRun;
extern	bool	gIsCheckPoint;
extern	int		gCheckPointX;
extern	int		gCheckPointY;
extern	bool	IsGameFullyActive;

extern	KGraphic	*sprites, *monsters;


pEnt g_AddSprite(int sprite_id, int posx, int posy, float vx, float vy, bool ajout_sens)
{
	pEnt	Ent;
	pEnt	CurrEnt;

	// hack
	if(sprite_id == ENT_MONSTER5)
		posy += 11;

	Ent = (pEnt)malloc(sizeof(Ent_t));
	memset(Ent,0,sizeof(Ent_t));

	Ent->sprite_id = sprite_id;
	Ent->x = (float)posx;
	Ent->y = (float)posy;
	Ent->velx = vx;
	Ent->vely = vy;
	Ent->activated = false;
	Ent->hit = false;
	Ent->LiftCollOk = false;
	Ent->OnLift = false;
	Ent->sinus_x = 0;
	Ent->sinus_y = 0;
	Ent->RemoveMe = false;
	Ent->ia_dir = 0;
	Ent->move_state = 0;
	Ent->ItemCatched = false;
	Ent->fatiguetime = 0;
	Ent->IsFatigue = false;
	Ent->Next = NULL;
	if(vy != 0)
		Ent->IsGravity = true;
	else
		Ent->IsGravity = false;

	if(Ent->sprite_id == ENT_MONSTER2)
		Ent->IsGravity = false;
	else if(Ent->sprite_id == ENT_EXITAD)
		Ent->ItemCatched = true;


	if(ajout_sens == false)
	{
		Ent->Next = gEntityList;
		gEntityList = Ent;
	}
	else
	{
		if(gEntityList)
		{
			for(CurrEnt=gEntityList ; CurrEnt ; CurrEnt=CurrEnt->Next)
			{
				if(CurrEnt->Next == NULL)
				{
					CurrEnt->Next = Ent;
					return gEntityList;
				}
			}
		}
		else
		{
			Ent->Next = NULL;
			gEntityList = Ent;
		}
	}
	return Ent;
}

void g_CleanupSpriteList()
{
	pEnt	Ent;

	while(gEntityList)
	{
		Ent = gEntityList->Next;
		free(gEntityList);
		gEntityList = Ent;
	}
}

pEnt g_FindSprite(int sprite_id, int pos)
{
	pEnt	Ent;

	for(Ent=gEntityList ; Ent ; Ent=Ent->Next)
	{
		if(Ent->sprite_id == sprite_id)
		{
			if(pos <= 1)
				return Ent;
			pos--;
		}
	}
	return NULL;
}

void g_RemoveSprite(pEnt SpriteEnt)
{
	pEnt	Ent;
	pEnt	TmpEnt;

	if(!SpriteEnt)
		return;
	if(SpriteEnt == gEntityList)
	{
		Ent = gEntityList->Next;
		free(gEntityList);
		gEntityList = Ent;
		return;
	}

	Ent=gEntityList;
	while(Ent->Next != SpriteEnt)
	{
		Ent=Ent->Next;
		if(!Ent->Next)
			return;
	}

	TmpEnt = SpriteEnt->Next;
	free(SpriteEnt);
	Ent->Next = TmpEnt;
}

void g_DrawOntopSprites()
{
	pEnt		Ent;
	KGraphic	*pdds;

	for(Ent=gEntityList ; Ent ; Ent=Ent->Next)
	{
		pdds = sprites;
		if(Ent->ItemCatched)
		{
			if(	Ent->sprite_id == ENT_POMME	|| Ent->sprite_id == ENT_BANANE)
			{
				if(Ent->x <= 0 && Ent->y <= 0)
				{
					g_AddCompteur_Pomme(1);
					g_RemoveSprite(Ent);
					Ent = gEntityList;
					continue;
				}
			}
			else if(Ent->sprite_id == ENT_LIFEUP)
			{
				if(Ent->y <= 0)
				{
					gVie++;
					g_RemoveSprite(Ent);
					Ent = gEntityList;
					continue;
				}
			}
			else if(Ent->sprite_id == ENT_KEY)
			{
				if(Ent->velx || Ent->vely)
				if( Ent->y <= 0 || ((fabs(Ent->x - 513) <= 5) && (fabs(Ent->y - 4) <= 5)))
				{
					//Ent->velx = Ent->vely = 0;
					gClee++;
					g_RemoveSprite(Ent);
					Ent = gEntityList;
					continue;
				}
			}
			blit(pdds, Ent->sprite_x,Ent->sprite_y,(int)Ent->x,(int)Ent->y,(int)Ent->sprite_w,(int)Ent->sprite_h,true);
		}
	}
}

// update la position et affiche les sprites
bool g_DrawSprites()
{
	pEnt		Ent;
	KGraphic	*pdds;

	for(Ent=gEntityList ; Ent ; Ent=Ent->Next)
	{
		gFlipY = false;
		pdds = sprites;

		g_AddGravity(Ent);
		if(!g_UpdateMove(Ent))
		{
			Ent->RemoveMe = true;	
		}

		if(Ent->sprite_id == ENT_PLAYER)
		{
			pdds = sprites; //grizzly;

			if(gInvinsibleTime > 0)
			if((int)gInvinsibleTime % 200 > 100)
				continue;

			if(!g_BoundPos(&Ent->x, &Ent->y, Ent->sprite_w, Ent->sprite_h))
			{
				if(ProgramState == PS_ACTIVE)
				{
					g_LostLive();
					return false;
				}
			}
			if(Ent->hit) gFlipY = true;
		}
		else if(Ent->sprite_id == ENT_MONSTER1)
		{
			pdds = monsters;
			if(Ent->hit) gFlipY = true;
		}
		else if(Ent->sprite_id == ENT_MONSTER2)
		{
			pdds = monsters;
			if(Ent->hit) gFlipY = true;
			g_AbeilleProcess(Ent);
		}
		else if(Ent->sprite_id == ENT_MONSTER3)
		{
			pdds = monsters;
			if(Ent->hit) gFlipY = true;
		}
		else if(Ent->sprite_id == ENT_MONSTER4)
		{
			pdds = monsters;
			if(Ent->hit) gFlipY = true;
		}
		else if(Ent->sprite_id == ENT_MONSTER5)
		{
			int		i;
			int		hauteur;
			int		ajust_y;
			int		tile_val;
			int		col;

			// affiche le fils d'arraigné
			pdds = monsters;
			if(Ent->IsGravity == false)
			{
				if(Ent->dir == 0)
					col = 28;
				else
					col = 17;
				ajust_y = 0;
				i = 1;

				tile_val = g_MordTile((int)Ent->x + col, (int)Ent->y);
				if(g_IsMur(tile_val) == 0)
				while(1)
				{
					hauteur = (int)Ent->y-i*TILE_Y;
					i++;

					// arrete le fil sur le premier tile de plafond
					tile_val = g_MordTile((int)Ent->x + col, hauteur);
					if(g_IsMur(tile_val))
					{
						ajust_y = (hauteur+gOffset_Y) / TILE_Y;
						ajust_y++;
						ajust_y *= TILE_Y;
						ajust_y -= (hauteur+gOffset_Y);
						hauteur += ajust_y;
					}
					else
						ajust_y = 0;

					blit(pdds,
							Ent->sprite_x-1,
							Ent->sprite_y,
							(int)Ent->x+col,
							hauteur,
							1,
							TILE_Y-ajust_y,
							false);
					if(hauteur <= 0)
						break;

					if(ajust_y != 0)
						break;
				}
			}
			if(Ent->hit) gFlipY = true;
		}
		else if(Ent->sprite_id == ENT_MONSTER6)
		{
			pdds = monsters;
			if(Ent->hit) gFlipY = true;
		}
		else if(Ent->sprite_id == ENT_FEUILLE)
		{
			if(Ent->vely >= 0)
				Ent->vely *= 0.75f;
			g_FeuilleProcess(Ent);
		}
		else if(Ent->sprite_id == ENT_HLIFT && Ent->lift_type == HLIFT_ID)
		{
			g_HLiftProcess(Ent);
		}
		else if(Ent->sprite_id == ENT_HLIFT && Ent->lift_type == RLIFT_ID)
		{
			g_RLiftProcess(Ent);
		}
		else if(Ent->sprite_id == ENT_EXITAD)
		{
	//		if(Ent->ItemCatched)
				continue;
		}
		else if(Ent->sprite_id == ENT_LIFEUP)
		{
			if(Ent->ItemCatched)
				continue;
		}	
		else if(Ent->sprite_id == ENT_DIAMANT)
		{
		//	Ent->nframe = level_id-1;
		}
		else if(Ent->sprite_id == ENT_POMME ||
			Ent->sprite_id == ENT_KEY ||
			Ent->sprite_id == ENT_BANANE)
		{
			if(Ent->ItemCatched)
			{
				if(Ent->x <= 0 && Ent->y <= 0)
				{
					if(Ent->sprite_id == ENT_POMME)
					{
						g_AddCompteur_Pomme(1);
					}
					else if(Ent->sprite_id == ENT_BANANE)
					{
						g_AddCompteur_Pomme(5);
					}
					g_RemoveSprite(Ent);
					Ent = gEntityList;
					continue;
				}
				continue;
			}
		}
		else if(Ent->sprite_id == ENT_STAR ||
				Ent->sprite_id == ENT_GREENSTAR ||
				Ent->sprite_id == ENT_REDSTAR)
		{
			if(!g_BoundPos(&Ent->x, &Ent->y, Ent->sprite_w, Ent->sprite_h))
			{
				Ent->RemoveMe = true;
			}
		}

		blit(pdds, Ent->sprite_x, Ent->sprite_y, (int)Ent->x, (int)Ent->y, (int)Ent->sprite_w, (int)Ent->sprite_h, true);
	}

	// efface les objets qui sont tombés trop bas hors de la map
	for(Ent=gEntityList ; Ent ; Ent=Ent->Next)
	{
		if(Ent->RemoveMe)
		{
			g_RemoveSprite(Ent);
			Ent=gEntityList;
			continue;
		}
	}

	return true;
}

void g_AnimateSprite()
{
	pEnt	Ent;
	float	FrameTime;
	int		nFrame;
	int		SpriteX;
	int		SpriteY;
	int		FrameW;
	int		FrameH;
	BOOL	LoopAnim;
	int		CollW;
	int		CollH;

	if(ProgramState == PS_PAUSE)
		return;

	for(Ent=gEntityList ; Ent ; Ent=Ent->Next)
	{
		switch(Ent->sprite_id)
		{
			case ENT_PLAYER:
			LoopAnim = true;
			FrameTime = PLAYER_FTIME;
			nFrame = PLAYER_FRAME;
			SpriteX = PLAYER_X;
			SpriteY = PLAYER_Y;
			FrameW = PLAYER_FW;
			FrameH = PLAYER_FH;
			CollW = PLAYER_CW;
			CollH = PLAYER_CH;
			break;

			case ENT_MONSTER1:
			LoopAnim = true;
			FrameTime = MONSTER1_FTIME;
			nFrame = MONSTER1_FRAME;
			SpriteX = MONSTER1_X;
			SpriteY = MONSTER1_Y;
			FrameW = MONSTER1_FW;
			FrameH = MONSTER1_FH;
			CollW = MONSTER1_CW;
			CollH = MONSTER1_CH;
			break;

			case ENT_MONSTER2:
			LoopAnim = true;
			FrameTime = MONSTER2_FTIME;
			nFrame = MONSTER2_FRAME;
			SpriteX = MONSTER2_X;
			SpriteY = MONSTER2_Y;
			FrameW = MONSTER2_FW;
			FrameH = MONSTER2_FH;
			CollW = MONSTER2_CW;
			CollH = MONSTER2_CH;
			break;

			case ENT_MONSTER3:
			LoopAnim = true;
			FrameTime = MONSTER3_FTIME;
			nFrame = MONSTER3_FRAME;
			SpriteX = MONSTER3_X;
			SpriteY = MONSTER3_Y;
			FrameW = MONSTER3_FW;
			FrameH = MONSTER3_FH;
			CollW = MONSTER3_CW;
			CollH = MONSTER3_CH;
			break;

			case ENT_MONSTER4:
			LoopAnim = true;
			FrameTime = MONSTER4_FTIME;
			nFrame = MONSTER4_FRAME;
			SpriteX = MONSTER4_X;
			SpriteY = MONSTER4_Y;
			FrameW = MONSTER4_FW;
			FrameH = MONSTER4_FH;
			CollW = MONSTER4_CW;
			CollH = MONSTER4_CH;
			break;

			case ENT_MONSTER5:
			LoopAnim = true;
			FrameTime = MONSTER5_FTIME;
			nFrame = MONSTER5_FRAME;
			SpriteX = MONSTER5_X;
			SpriteY = MONSTER5_Y;
			FrameW = MONSTER5_FW;
			FrameH = MONSTER5_FH;
			CollW = MONSTER5_CW;
			CollH = MONSTER5_CH;
			break;

			case ENT_MONSTER6:
			LoopAnim = true;
			FrameTime = MONSTER6_FTIME;
			nFrame = MONSTER6_FRAME;
			SpriteX = MONSTER6_X;
			SpriteY = MONSTER6_Y;
			FrameW = MONSTER6_FW;
			FrameH = MONSTER6_FH;
			CollW = MONSTER6_CW;
			CollH = MONSTER6_CH;
			break;

			case ENT_HLIFT:
			LoopAnim = false;
			FrameTime = 10;
			nFrame = 1;
			SpriteX = HLIFT_X;
			SpriteY = HLIFT_Y;
			FrameW = HLIFT_FW;
			FrameH = HLIFT_FH;
			CollW = HLIFT_CW;
			CollH = HLIFT_CH;
			break;

			case ENT_LIFEUP:
			LoopAnim = false;
			FrameTime = 10;
			nFrame = 1;
			SpriteX = LIFEUP_X;
			SpriteY = LIFEUP_Y;
			FrameW = LIFEUP_FW;
			FrameH = LIFEUP_FH;
			CollW = LIFEUP_CW;
			CollH = LIFEUP_CH;
			break;

			case ENT_POMME:
			LoopAnim = true;
			FrameTime = POMME_FTIME;
			nFrame = POMME_FRAME;
			SpriteX = POMME_X;
			SpriteY = POMME_Y;
			FrameW = POMME_FW;
			FrameH = POMME_FH;
			CollW = POMME_CW;
			CollH = POMME_CH;
			break;

			case ENT_POMMEV:
			LoopAnim = true;
			FrameTime = POMMEV_FTIME;
			nFrame = POMMEV_FRAME;
			SpriteX = POMMEV_X;
			SpriteY = POMMEV_Y;
			FrameW = POMMEV_FW;
			FrameH = POMMEV_FH;
			CollW = POMMEV_CW;
			CollH = POMMEV_CH;
			break;

			case ENT_POMMEJ:
			LoopAnim = true;
			FrameTime = POMMEJ_FTIME;
			nFrame = POMMEJ_FRAME;
			SpriteX = POMMEJ_X;
			SpriteY = POMMEJ_Y;
			FrameW = POMMEJ_FW;
			FrameH = POMMEJ_FH;
			CollW = POMMEJ_CW;
			CollH = POMMEJ_CH;
			break;

			case ENT_BANANE:
			LoopAnim = false;
			FrameTime = 10;
			nFrame = 1;
			SpriteX = BANANE_X;
			SpriteY = BANANE_Y;
			FrameW = BANANE_FW;
			FrameH = BANANE_FH;
			CollW = BANANE_CW;
			CollH = BANANE_CH;
			break;

			case ENT_KEY:
			LoopAnim = false;
			FrameTime = KEY_FTIME;
			nFrame = 1;
			SpriteX = KEY_X;
			SpriteY = KEY_Y;
			FrameW = KEY_FW;
			FrameH = KEY_FH;
			CollW = KEY_CW;
			CollH = KEY_CH;
			break;

			case ENT_DIAMANT:
			LoopAnim = false;
			FrameTime = DIAMANT_FTIME;
			nFrame = Ent->nframe; //1;
			SpriteX = DIAMANT_X;
			SpriteY = DIAMANT_Y;
			FrameW = DIAMANT_FW;
			FrameH = DIAMANT_FH;
			CollW = DIAMANT_CW;
			CollH = DIAMANT_CH;
			break;

			case ENT_STAR:
			LoopAnim = false;
			FrameTime = STAR_FTIME;
			nFrame = 1;
			SpriteX = STAR_X;
			SpriteY = STAR_Y;
			FrameW = STAR_FW;
			FrameH = STAR_FH;
			CollW = STAR_CW;
			CollH = STAR_CH;
			break;

			case ENT_REDSTAR:
			LoopAnim = false;
			FrameTime = REDSTAR_FTIME;
			nFrame = 1;
			SpriteX = REDSTAR_X;
			SpriteY = REDSTAR_Y;
			FrameW = REDSTAR_FW;
			FrameH = REDSTAR_FH;
			CollW = REDSTAR_CW;
			CollH = REDSTAR_CH;
			break;

			case ENT_GREENSTAR:
			LoopAnim = false;
			FrameTime = GREENSTAR_FTIME;
			nFrame = 1;
			SpriteX = GREENSTAR_X;
			SpriteY = GREENSTAR_Y;
			FrameW = GREENSTAR_FW;
			FrameH = GREENSTAR_FH;
			CollW = GREENSTAR_CW;
			CollH = GREENSTAR_CH;
			break;

			case ENT_EXITAD:
			LoopAnim = false;
			FrameTime = EXITAD_FTIME;
			nFrame = 1;
			SpriteX = EXITAD_X;
			SpriteY = EXITAD_Y;
			FrameW = EXITAD_FW;
			FrameH = EXITAD_FH;
			CollW = EXITAD_CW;
			CollH = EXITAD_CH;
			break;

			case ENT_FEUILLE:
			LoopAnim = false;
			FrameTime = 10;
			nFrame = 1;
			SpriteX = FEUILLE_X;
			SpriteY = FEUILLE_Y;
			FrameW = FEUILLE_FW;
			FrameH = FEUILLE_FH;
			CollW = FEUILLE_CW;
			CollH = FEUILLE_CH;
			break;

			case ENT_POTMIEL:
			LoopAnim = true;
			FrameTime = POTMIEL_FTIME;
			nFrame = POTMIEL_FRAME;
			SpriteX = POTMIEL_X;
			SpriteY = POTMIEL_Y;
			FrameW = POTMIEL_FW;
			FrameH = POTMIEL_FH;
			CollW = POTMIEL_CW;
			CollH = POTMIEL_CH;
			break;

			case ENT_SMOKE:
			LoopAnim = true;
			FrameTime = SMOKE_FTIME;
			nFrame = SMOKE_FRAME;
			SpriteX = SMOKE_X;
			SpriteY = SMOKE_Y;
			FrameW = SMOKE_FW;
			FrameH = SMOKE_FH;
			CollW = SMOKE_CW;
			CollH = SMOKE_CH;
			break;

			case ENT_PANG:
			LoopAnim = false;
			FrameTime = PANG_FTIME;
			nFrame = PANG_FRAME;
			SpriteX = PANG_X;
			SpriteY = PANG_Y;
			FrameW = PANG_FW;
			FrameH = PANG_FH;
			CollW = PANG_CW;
			CollH = PANG_CH;
			break;

			case ENT_COFFRE:
			LoopAnim = false;
			FrameTime = 20;
			nFrame = 1;
			SpriteX = COFFRE_X;
			SpriteY = COFFRE_Y;
			FrameW = COFFRE_FW;
			FrameH = COFFRE_FH;
			CollW = COFFRE_CW;
			CollH = COFFRE_CH;
			break;

			case ENT_BUMPER:
			LoopAnim = false;
			if(!Ent->activated)
				nFrame = 1;
			else
				nFrame = BUMPER_FRAME;
			FrameTime = BUMPER_FTIME;
			SpriteX = BUMPER_X;
			SpriteY = BUMPER_Y;
			FrameW = BUMPER_FW;
			FrameH = BUMPER_FH;
			CollW = BUMPER_CW;
			CollH = BUMPER_CH;
			break;
		}

		if(Ent->sprite_id == ENT_PLAYER)
		{
			// anim de  respiration reduite
			if(Ent->dir == 2 || Ent->dir == 3)
			{
				FrameTime = PLAYER_FTIME * 2.5f;
			}
			
			// anim de  jump reduite
			if(Ent->dir == 4 || Ent->dir == 5)
			{
				FrameTime = PLAYER_FTIME * 4.0f;
			}

			// anim de course accélérée
			if(gPlayerRun)
			{
				FrameTime = PLAYER_FTIME / 1.5f;
			}
		}
		else if(Ent->sprite_id == ENT_DIAMANT)
		{
			Ent->nframe = level_id-1;
			goto noanim;
		}
		else if(Ent->sprite_id == ENT_MONSTER3)
		{
			// anim de  respiration reduite
			if(Ent->dir != 0 && Ent->dir != 1)
				FrameTime = MONSTER3_FTIME * 2.5f;
		}

		Ent->frametime += gFrameTime_msec;
		if(Ent->frametime > FrameTime)
		{
			Ent->frametime = 0;
			Ent->nframe++;
			if(LoopAnim)
			{
				if(Ent->nframe >= nFrame)
					Ent->nframe = 0;
			}
			else
			{
				if(Ent->nframe >= nFrame)
				{
					Ent->nframe = nFrame-1;

					// on desactive le bumper apres son activation suite a un saut
					if(Ent->sprite_id == ENT_BUMPER)
					{
						Ent->activated = FALSE;
				//		Ent->dir = 0;	// les superbumper redevienne normaux ici
					}
				}
			}
		}

noanim:

		Ent->sprite_x = SpriteX + Ent->nframe*FrameW;
		Ent->sprite_y = SpriteY + Ent->dir*FrameH;
		Ent->sprite_w = FrameW;
		Ent->sprite_h = FrameH;
		Ent->sprite_cw = CollW;
		Ent->sprite_ch = CollH;

		if(Ent->sprite_id == ENT_PLAYER)
		{
			if(Ent->dir != 0)
			{
				Ent->sprite_x =  SpriteX + (nFrame - Ent->nframe - 1)*FrameW;
			}
		}

		if(Ent->sprite_id == ENT_MONSTER3)
		if(Ent->dir == 0)
		{
			Ent->sprite_x =  SpriteX + (nFrame - Ent->nframe - 1)*FrameW;
		}
	}
}

bool g_BoundPos(float *x, float *y, int w, int h)
{
	if(*x > ScreenX - w)
	{
		*x = (float)(ScreenX - w);
	}
	else if(*x < 0)
	{
		*x = 0;
	}

	if(*y > gHeader.level_h*TILE_Y - gOffset_Y)
	{
		*y = (float)(gHeader.level_h*TILE_Y - gOffset_Y);
		return false;
	}
	else if(*y < -h)
	{
		*y = (float)-h;
	}
	return true;
}

void g_AddGravity(pEnt Ent)
{
	int		val_tile;
	float	grav_value;

	if(!Ent)
		return;
	if(false == IsGameFullyActive)
		return;

//	if(ProgramState == PS_CONGRAT ||
//		ProgramState == PS_WINLEVEL)
//		return;
	if(ProgramState == PS_PAUSE)
		return;

	if(Ent->sprite_id == ENT_BANANE)
		if(!Ent->IsGravity)
			return;

	if(	Ent->sprite_id == ENT_POMME		||
		Ent->sprite_id == ENT_KEY		||
		Ent->sprite_id == ENT_DIAMANT	||
		Ent->sprite_id == ENT_HLIFT		||
		Ent->sprite_id == ENT_EXITAD	||
		Ent->sprite_id == ENT_SMOKE		||
		Ent->sprite_id == ENT_PANG		||
		Ent->sprite_id == ENT_LIFEUP	||
		Ent->sprite_id == ENT_BUMPER)
		return;

	if(Ent->OnLift)
	{
		if(Ent->sprite_id == ENT_PLAYER)
		{
			gIsHeroCanJump = true;
		}
		if(Ent->vely >= 0)
			return;
	}

	// pas de gravitee sur les abeilles
	if(Ent->sprite_id == ENT_MONSTER2)
	if(!Ent->IsGravity)
		return;

	if(Ent->sprite_id == ENT_MONSTER4)
	if(!Ent->IsGravity)
		return;

	if(Ent->sprite_id == ENT_MONSTER5)
	if(!Ent->IsGravity)
		return;

//	if(Ent->sprite_id == ENT_MONSTER6)
//	if(!Ent->IsGravity)
//		return;


	val_tile = g_MurBas(	(int)Ent->x + (Ent->sprite_w - Ent->sprite_cw)/2,
							(int)Ent->y + (Ent->sprite_h - Ent->sprite_ch)/2,
							Ent->sprite_cw,
							Ent->sprite_ch, MUR_ID);

	// hack
	if(Ent->sprite_id == ENT_MONSTER2 ||
		Ent->sprite_id == ENT_MONSTER4 ||
		Ent->sprite_id == ENT_MONSTER5)
	{
		if(Ent->hit)
		{
			val_tile = 0;
		}
	}
	else if(Ent->sprite_id == ENT_STAR ||
		Ent->sprite_id == ENT_GREENSTAR ||
		Ent->sprite_id == ENT_REDSTAR)
	{
		val_tile = 0;
	}

	if(val_tile)
	{
		if(Ent->sprite_id == ENT_PLAYER)
		{
			if(!gJumpKeyPressed)
			{
				gIsHeroCanJump = true;
				gJumpTime = 0;
			}
		}
		return;
	}
	else
	{
		if(Ent->sprite_id == ENT_PLAYER)
		{
			gIsHeroCanJump = false;
		}
	}

	if(Ent->sprite_id == ENT_PLAYER)
	{
		grav_value = gJumpTime * 0.0003f;
		grav_value = (0.01f + grav_value) * gFrameTime_msec;
		Ent->vely += grav_value;
	}
	else
	{
		Ent->vely += 0.01f * gFrameTime_msec;
	}
}

void g_AddBumperJump(pEnt Ent, float JumpValue)
{
//	int	tileid;

	if(!Ent)
		return;

	if(ProgramState == PS_ACTIVE)
	{
		if(Ent->vely < 0)
			return;
	}

	Ent->vely = -JumpValue;
}

void g_AddVelEntity(pEnt Ent)
{
	float	MoveSpeed_X;
	float	MoveSpeed_Y;

	if(!Ent)
		return;

	MoveSpeed_X = (Ent->velx * gFrameTime_msec * 0.08f);
	MoveSpeed_Y = (Ent->vely * gFrameTime_msec * 0.08f);

#define MAX_SPEED	9 // 12

	// limitation de vitesse du sprite
	if(Ent->sprite_id == ENT_PLAYER)
	{
		if(MoveSpeed_X > MAX_SPEED)
			MoveSpeed_X = MAX_SPEED;
		else if(MoveSpeed_X < -MAX_SPEED)
			MoveSpeed_X = -MAX_SPEED;

		if(MoveSpeed_Y > MAX_SPEED)
			MoveSpeed_Y = MAX_SPEED;
		else if(MoveSpeed_Y < -MAX_SPEED)
			MoveSpeed_Y = -MAX_SPEED;
	}

	Ent->x += MoveSpeed_X;
	Ent->y += MoveSpeed_Y;
}

bool g_UpdateMove(pEnt Ent)
{
	int		tile_val;

	if(false == IsGameFullyActive)
		return true;
	// hack
	if(ProgramState == PS_LOADING)
		return true;

	if(ProgramState == PS_PAUSE)
		return true;

	// la sortie
	if(ProgramState == PS_ACTIVE)
	{
		if(Ent->sprite_id == ENT_PLAYER)
		{
			if(g_MordTile((int)Ent->x+PLAYER_FW/2, (int)Ent->y+PLAYER_FH/2) == EXIT_ID)
			{
				Ent->vely = 0;
				g_EndLevelHit();
				return true;
			}
		}
	}
	
	// tout objet qui sort de la map vers le bas est supprimé
	if(Ent->sprite_id != ENT_PLAYER)
	{
		if((int)Ent->y + gOffset_Y > gHeader.level_h*TILE_Y)
		{
			return false;
		}
	}

	// si c'est une etoile ou une pancarte,.. ya pas de collision avec le decors
	if(	Ent->sprite_id == ENT_STAR ||
		Ent->sprite_id == ENT_REDSTAR ||
		Ent->sprite_id == ENT_GREENSTAR ||
		Ent->sprite_id == ENT_SMOKE ||
		Ent->sprite_id == ENT_PANG ||
		Ent->sprite_id == ENT_MONSTER4 ||
		Ent->sprite_id == ENT_MONSTER5 ||
		Ent->sprite_id == ENT_EXITAD)
	{
		g_AddVelEntity(Ent);
		return true;
	}


	if(Ent->velx == 0 && Ent->vely == 0)
		return true;

	g_AddVelEntity(Ent);

	// si c'est une banane attrapé ya pas de collision avec le decors
	if(Ent->ItemCatched)
	if(Ent->sprite_id == ENT_BANANE ||
		Ent->sprite_id == ENT_KEY ||
		Ent->sprite_id == ENT_LIFEUP ||
		Ent->sprite_id == ENT_POMME)
	return true;



	if(ProgramState == PS_ACTIVE || ProgramState == PS_CONGRAT || ProgramState == PS_WINLEVEL || ProgramState == PS_GAMEOVER)
	{
		if(Ent->vely >= 0)
		{
			// les pics
			if(Ent->sprite_id == ENT_PLAYER)
			{
				int pic_id;

				if(gEpisodeID == 2)
					pic_id = PIC_ID - 10;
				else if(gEpisodeID == 3)
					pic_id = PIC_ID + 5;
				else
					pic_id = PIC_ID;

#ifdef DEMO_VERSION
	//pic_id = PIC_ID - 10;
#endif

				tile_val = g_MurBas(	(int)Ent->x + (Ent->sprite_w - Ent->sprite_cw)/2,
										(int)Ent->y-TILE_Y + (Ent->sprite_h - Ent->sprite_ch)/2,
										Ent->sprite_cw,
										Ent->sprite_ch, pic_id);

				if(tile_val)
				{
					if(gInvinsibleTime <= 0)
					{
						g_AddParticles(ENT_REDSTAR, (int)Ent->x, (int)Ent->y);
						g_AddBumperJump(Ent, PLAYER_JUMP);
						g_LostLive();
						return true;
					}
				}
			}

			if(Ent->hit == false)
			{
				tile_val = g_MurBas(	(int)Ent->x + (Ent->sprite_w - Ent->sprite_cw)/2,
										(int)Ent->y + (Ent->sprite_h - Ent->sprite_ch)/2,
										Ent->sprite_cw,
										Ent->sprite_ch, MUR_ID);

				if(tile_val)
				{
					if(Ent->sprite_id == ENT_PLAYER)
					{
					}
					else if(Ent->sprite_id == ENT_FEUILLE)
					{
						//g_RemoveSprite(Ent);
						return false;
					}
					else if(Ent->sprite_id == ENT_BANANE ||
						Ent->sprite_id == ENT_POMMEJ ||
						Ent->sprite_id == ENT_POMMEV ||
						Ent->sprite_id == ENT_POMME)
					{
						Ent->velx = 0;
					}

					if(g_FixeBas(&Ent->y, Ent->sprite_ch))
						Ent->vely = 0;
				}
			}
		}
		else
		{
			//player_MoveLeft(0, Ent);
			//player_MoveRight(0, Ent);
			tile_val = g_MurHaut(	(int)Ent->x + (Ent->sprite_w - Ent->sprite_cw)/2,
									(int)Ent->y + (Ent->sprite_h - Ent->sprite_ch)/2,
									Ent->sprite_cw,
									Ent->sprite_h, MUR_ID);

			if(Ent->sprite_id == ENT_PLAYER)
			{
				if( gTileCollisionID == SPECIAL_ID )
				{
					lb_PlaySound(3,gSoundVol);
					g_AddParticles(ENT_STAR, (int)Ent->x + PLAYER_FW/2, (int)Ent->y);
					gLevel[gTilePos].blocID  = VOID_ID + 48;
					g_AddSpecialBlocItem(0, (int)Ent->x, (int)Ent->y);
				}
				else if( gTileCollisionID == PJAUNE_ID )
				{
					lb_PlaySound(3,gSoundVol);
				//	g_AddParticles(ENT_STAR, (int)Ent->x + PLAYER_FW/2, (int)Ent->y);
					gLevel[gTilePos].blocID  = VOID_ID + 48;
					g_AddSpecialBlocItem(1, (int)Ent->x, (int)Ent->y);
				}
				else if( gTileCollisionID == PVERTE_ID )
				{
					lb_PlaySound(3,gSoundVol);
				//	g_AddParticles(ENT_STAR, (int)Ent->x + PLAYER_FW/2, (int)Ent->y);
					gLevel[gTilePos].blocID  = VOID_ID + 48;
					g_AddSpecialBlocItem(2, (int)Ent->x, (int)Ent->y);
				}
				else if( gTileCollisionID == POW_ID )
				{
					lb_PlaySound(3,gSoundVol);
					lb_PlaySound(19,gSoundVol);
					w_Effect1();
					g_AddParticles(ENT_STAR, (int)Ent->x + PLAYER_FW/2, (int)Ent->y);
					g_AddParticles(ENT_STAR, (int)Ent->x + PLAYER_FW/2, (int)Ent->y);
					gLevel[gTilePos].blocID  = VOID_ID + 48;
				}
				else if( gTileCollisionID == CHECKPOINT_ID )
				{
					// recupere les infos du point de passage
					gIsCheckPoint = true;
				//	gCheckPointX = (((int)Ent->x % TILE_X) * TILE_X) + gOffset_X - (PLAYER_FW-TILE_X)/2;
				//	gCheckPointY = (((int)Ent->y % TILE_Y) * TILE_Y) + gOffset_Y - (PLAYER_FH-TILE_Y)/2;
					gCheckPointX = (int)Ent->x + gOffset_X - (PLAYER_FW-TILE_X)/2;
					gCheckPointY = (int)Ent->y + gOffset_Y - (PLAYER_FH-TILE_Y)/2;

					lb_PlaySound(21,gSoundVol); // 3
					g_AddParticles(ENT_STAR, (int)Ent->x + PLAYER_FW/2, (int)Ent->y);
					g_AddParticles(ENT_STAR, (int)Ent->x + PLAYER_FW/2, (int)Ent->y);
					g_AddParticles(ENT_STAR, (int)Ent->x + PLAYER_FW/2, (int)Ent->y);
					gLevel[gTilePos].blocID  = VOID_ID + 48;
				}
			}

			if(tile_val == MUR_ID)
			{
				Ent->vely = 0.01f; // 0 : hack
				g_FixeHaut(&Ent->y, (Ent->sprite_h - Ent->sprite_ch)/2 );
			}
		}
	}
	return true;
}





