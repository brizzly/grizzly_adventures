
#include "sprite.h"
#include "game.h"
#include "player.h"
#include "collision.h"
#include "sprite_process.h"


extern	float	gFrameTime_msec;
extern	int		gSoundVol;
extern	float	gJumpTime;
extern	int		gVie;
extern	int		gClee;
extern	pEnt	gEntityList;
extern	bool	gPanelIsActive;
extern	bool	gIsDiamantGrabed;
extern	int		ProgramState;
extern	bool	gPlayerRun;
extern	bool	RotationDir;


void sprite_RandomizeRotation(pEnt Ent)
{
	int		i;
	float	MoveOffsetX, MoveOffsetY;

	if(!Ent)
		return;

	RotationDir = !RotationDir;
	Ent->RotateDir = RotationDir;
	return;

	gFrameTime_msec = 17;

	if(Ent->sprite_id == ENT_HLIFT && Ent->lift_type == RLIFT_ID)
	{
		for(i=0 ; i<(10+rand()%500)*400 ; i++)
		{
			g_RLiftProcess(Ent);
		}
	}
	else if(Ent->sprite_id == ENT_MONSTER2 && Ent->move_state == MOVE_ROT)
	{
		for(i=0 ; i<(10+rand()%500)*400 ; i++)
		{
			sprite_Rotate(Ent, 0.15f, 0.004f, &MoveOffsetX, &MoveOffsetY);
			Ent->x += MoveOffsetX;
			Ent->y += MoveOffsetY;
		}
	}
}

void sprite_Rotate(pEnt Ent, float amplitude, float speed, float *x_move, float *y_move)
{
	if(!Ent)
		return;

	if(false == Ent->RotateDir)
	{
		// calcul deplacement en X
		Ent->sinus_x -= (speed * gFrameTime_msec); // +=
		if(Ent->sinus_x <= 0) // >= 6.56
		{
			if(Ent->ia_dir == 1) // 0
				Ent->ia_dir = 0; // 1
			else
				Ent->ia_dir = 1; // 0
			Ent->sinus_x += 6.25; // -=
		}

		// calcul deplacement en Y
		Ent->sinus_y -= (speed * gFrameTime_msec); // +=
		if(Ent->sinus_y <= 0) // >= 6.25
		{
			Ent->sinus_y += 6.25; // -=
		}

		*x_move = -amplitude * gFrameTime_msec * (float)cos(Ent->sinus_x); //+
		*y_move = -amplitude * gFrameTime_msec * (float)sin(Ent->sinus_y); //+
	}
	else
	{
		// calcul deplacement en X
		Ent->sinus_x += (speed * gFrameTime_msec);
		if(Ent->sinus_x >= 6.25)
		{
			if(Ent->ia_dir == 0)
				Ent->ia_dir = 1;
			else
				Ent->ia_dir = 0;
			Ent->sinus_x -= 6.25;
		}

		// calcul deplacement en Y
		Ent->sinus_y += (speed * gFrameTime_msec);
		if(Ent->sinus_y >= 6.25)
		{
			Ent->sinus_y -= 6.25;
		}

		*x_move = amplitude * gFrameTime_msec * (float)cos(Ent->sinus_x); //+
		*y_move = amplitude * gFrameTime_msec * (float)sin(Ent->sinus_y); //+
	}
}


void g_RLiftProcess(pEnt RLiftProcess)
{
	pEnt	PlayerEnt;
	float	MoveOffsetX;
	float	MoveOffsetY;

	if(!RLiftProcess)
		return;
	if(RLiftProcess->sprite_id != ENT_HLIFT)
		return;
	if(RLiftProcess->lift_type != RLIFT_ID)
		return;

	if(ProgramState == PS_PAUSE)
		return;

	// Les deplacements gauche|droite de l'ascenseur horizontal
	sprite_Rotate(RLiftProcess, -0.10f, -0.001f, &MoveOffsetX, &MoveOffsetY);
	RLiftProcess->x += MoveOffsetX;
	RLiftProcess->y += MoveOffsetY;

	PlayerEnt = g_FindSprite(ENT_PLAYER, 1);
	if(!PlayerEnt)
		return;

	// deplacement du joueur s'il est sur l'ascenseur
	if(PlayerEnt->OnLift)
	if(RLiftProcess->OnLift)
	{
		if(MoveOffsetX < 0)
		{
			player_MoveLeft(-MoveOffsetX, PlayerEnt);
		}
		else
		{
			player_MoveRight(MoveOffsetX, PlayerEnt);
		}	

		if(MoveOffsetY < 0)
		{
			player_MoveUp(-MoveOffsetY, PlayerEnt);
		}
		else
		{
			player_MoveDown(MoveOffsetY, PlayerEnt);
		}
	}
}

void g_HLiftProcess(pEnt HLiftProcess)
{
	int		tile_val;
	pEnt	PlayerEnt;
	float	MoveOffset;

	if(!HLiftProcess)
		return;
	if(HLiftProcess->sprite_id != ENT_HLIFT)
		return;
	if(HLiftProcess->lift_type != HLIFT_ID)
		return;

	if(ProgramState == PS_PAUSE)
		return;

	// Les deplacements gauche|droite de l'ascenseur horizontal
	MoveOffset = 0;
	if(!HLiftProcess->ia_dir)
	{
		MoveOffset = gFrameTime_msec * 0.07f;
		tile_val = g_MurDroite(	(int)HLiftProcess->x + (HLiftProcess->sprite_w - HLiftProcess->sprite_cw)/2,
								(int)HLiftProcess->y + (HLiftProcess->sprite_h - HLiftProcess->sprite_ch)/2,
								HLiftProcess->sprite_cw,
								HLiftProcess->sprite_ch);
		if(tile_val != 0 && tile_val != SMUR_ID)
		{
			HLiftProcess->ia_dir = true;
		}
	}
	else
	{
		MoveOffset = -gFrameTime_msec * 0.07f;
		tile_val = g_MurGauche(	(int)HLiftProcess->x + (HLiftProcess->sprite_w - HLiftProcess->sprite_cw)/2,
								(int)HLiftProcess->y + (HLiftProcess->sprite_h - HLiftProcess->sprite_ch)/2,
								HLiftProcess->sprite_cw,
									HLiftProcess->sprite_ch);
		if(tile_val != 0 && tile_val != SMUR_ID)
		{
			HLiftProcess->ia_dir = false;
		}
	}
	HLiftProcess->x += MoveOffset;


	PlayerEnt = g_FindSprite(ENT_PLAYER, 1);
	if(!PlayerEnt)
		return;

	// deplacement du joueur s'il est sur l'ascenseur
	if(PlayerEnt->OnLift)
	if(HLiftProcess->OnLift)
	{
		if(MoveOffset < 0)
		{
			player_MoveLeft(-MoveOffset, PlayerEnt);
		}
		else
		{
			player_MoveRight(MoveOffset, PlayerEnt);
		}
	}
}

void g_AbeilleProcess(pEnt MonsterEnt)
{
	float	MoveOffsetX;
	float	MoveOffsetY;

	if(!MonsterEnt)
		return;
	if(MonsterEnt->sprite_id != ENT_MONSTER2)
		return;

	if(ProgramState == PS_PAUSE)
		return;

	switch(MonsterEnt->move_state)
	{
		case MOVE_GD:
		MonsterEnt->x += 0.15f * gFrameTime_msec * (float)sin(MonsterEnt->sinus_x);
		MonsterEnt->sinus_x += (0.004f * gFrameTime_msec);
		if(MonsterEnt->sinus_x >= 6.25)
		{
			MonsterEnt->sinus_y -= 6.25;
		}
		break;

		case MOVE_HB:
		MonsterEnt->y += 0.15f * gFrameTime_msec * (float)sin(MonsterEnt->sinus_y);
		MonsterEnt->sinus_y += (0.004f * gFrameTime_msec);
		if(MonsterEnt->sinus_y >= 6.25)
		{
			MonsterEnt->sinus_y -= 6.25;
		}
		break;

		case MOVE_ROT:
		sprite_Rotate(MonsterEnt, 0.15f, 0.004f, &MoveOffsetX, &MoveOffsetY);
		MonsterEnt->x += MoveOffsetX;
		MonsterEnt->y += MoveOffsetY;
		break;
	}
}

void g_FeuilleProcess(pEnt FeuilleEnt)
{
	if(!FeuilleEnt)
		return;
	if(FeuilleEnt->sprite_id != ENT_FEUILLE)
		return;

	if(ProgramState == PS_PAUSE)
		return;
	
	FeuilleEnt->x += 0.12f * gFrameTime_msec * (float)sin(FeuilleEnt->sinus_x);
	FeuilleEnt->sinus_x += (0.007f * gFrameTime_msec);
	if(FeuilleEnt->sinus_x >= 6.25)
		FeuilleEnt->sinus_x = 0;
}

#define	LIFTCOL_APPROXVAL	8

void g_Collision_HLift(pEnt PlayerEnt, pEnt CopyEnt)
{
	int		diff;

	if(!PlayerEnt)
		return;
	if(!CopyEnt)
		return;
	if(CopyEnt->sprite_id != ENT_HLIFT)
		return;

	if(PlayerEnt->OnLift == false)
	{
		if((int)PlayerEnt->y+PLAYER_FH <= (int)CopyEnt->y + LIFTCOL_APPROXVAL)
		{
			CopyEnt->LiftCollOk = true;
		}
	}

	if(g_Collision_2Tiles(
							(int)PlayerEnt->x,
							(int)PlayerEnt->y,
							PlayerEnt->sprite_w,
							PlayerEnt->sprite_h,
							(int)CopyEnt->x,
							(int)CopyEnt->y,
							CopyEnt->sprite_w,
							CopyEnt->sprite_h))
	{
		// si le joueur tombe, et touche un ascenseur
		if(PlayerEnt->vely >= 0)
		{
			if(CopyEnt->LiftCollOk)
			{
				// si il doit y avoir une correction de la hauteur du joueur sur le sprite d'ascenseur
				diff = (int)PlayerEnt->y+PlayerEnt->sprite_h - (int)CopyEnt->y;
				if(diff > LIFTCOL_APPROXVAL)
					return;		
			
				PlayerEnt->y = 1 + CopyEnt->y - PlayerEnt->sprite_h;
				PlayerEnt->vely = 0;

				CopyEnt->LiftCollOk = false;
				PlayerEnt->OnLift = true;
				CopyEnt->OnLift = true;
				return;
			}
		}
		
	}
	else
	{
		PlayerEnt->OnLift = false;
		CopyEnt->OnLift = false;
	}
}

void g_Collision_Bumper(pEnt PlayerEnt, pEnt CopyEnt)
{
	if(!PlayerEnt)
		return;
	if(!CopyEnt)
		return;
	if(CopyEnt->sprite_id != ENT_BUMPER)
		return;

	if(g_MordTile_2(
						(int)PlayerEnt->x,
						(int)PlayerEnt->y,
						PlayerEnt->sprite_w,
						PlayerEnt->sprite_h,
						(int)CopyEnt->x,
						(int)CopyEnt->y,
						CopyEnt->sprite_w,
						CopyEnt->sprite_h
					))
	{
		if(!CopyEnt->activated)
		{
			CopyEnt->activated = TRUE;
			CopyEnt->nframe = 1;
			PlayerEnt->vely = 0;	// ??
			if(CopyEnt->dir == 1)
			{
				lb_PlaySound(20, gSoundVol);
				g_AddParticles(ENT_GREENSTAR, (int)CopyEnt->x+CopyEnt->sprite_w/2, (int)CopyEnt->y+CopyEnt->sprite_h/2);
				g_AddBumperJump(PlayerEnt, 2*PLAYER_JUMP);
			}
			else
			{
				lb_PlaySound(4, gSoundVol);
				g_AddBumperJump(PlayerEnt, PLAYER_JUMP);
			}
		//	if(CopyEnt->sprite_id == ENT_PLAYER)
		//	{
			gJumpTime = 0;
		//	}
		}
	}
}

void g_GetVelocityDir(float *velx, float *vely, float posx, float posy, float destx, float desty)
{
	*velx = (destx - posx) * 0.016f;
	*vely = (desty - posy) * 0.016f;
}

bool g_Collision_Pomme(pEnt PlayerEnt, pEnt CopyEnt)
{
	if(!PlayerEnt)
		return false;
	if(!CopyEnt)
		return false;
	if(CopyEnt->sprite_id != ENT_POMME)
		return false;

	if(CopyEnt->ItemCatched == false)
	if(g_MordTile_2(
						(int)PlayerEnt->x,
						(int)PlayerEnt->y,
						PlayerEnt->sprite_w,
						PlayerEnt->sprite_h,
						(int)CopyEnt->x,
						(int)CopyEnt->y,
						CopyEnt->sprite_w,
						CopyEnt->sprite_h)
					)
	{
		g_ShowItemcounter(0);
		lb_PlaySound(1,gSoundVol);
		g_GetVelocityDir(&CopyEnt->velx, &CopyEnt->vely, CopyEnt->x, CopyEnt->y, 0, 0);
		CopyEnt->ItemCatched = true;
		return true;
	}
	return false;
}

bool g_Collision_PommeJ(pEnt PlayerEnt, pEnt CopyEnt)
{
	if(!PlayerEnt)
		return false;
	if(!CopyEnt)
		return false;
	if(CopyEnt->sprite_id != ENT_POMMEJ)
		return false;

//	if(CopyEnt->ItemCatched == false)
	if(CopyEnt->vely >= 0)
	if(g_MordTile_2(
						(int)PlayerEnt->x,
						(int)PlayerEnt->y,
						PlayerEnt->sprite_w,
						PlayerEnt->sprite_h,
						(int)CopyEnt->x,
						(int)CopyEnt->y,
						CopyEnt->sprite_w,
						CopyEnt->sprite_h)
					)
	{
		lb_PlaySound(17,gSoundVol); // 1
		g_RemoveSprite(CopyEnt);
		player_MakeInvinsible();
		return true;
	}
	return false;
}

bool g_Collision_PommeV(pEnt PlayerEnt, pEnt CopyEnt)
{
	if(!PlayerEnt)
		return false;
	if(!CopyEnt)
		return false;
	if(CopyEnt->sprite_id != ENT_POMMEV)
		return false;

//	if(CopyEnt->ItemCatched == false)
	if(CopyEnt->vely >= 0)
	if(g_MordTile_2(
						(int)PlayerEnt->x,
						(int)PlayerEnt->y,
						PlayerEnt->sprite_w,
						PlayerEnt->sprite_h,
						(int)CopyEnt->x,
						(int)CopyEnt->y,
						CopyEnt->sprite_w,
						CopyEnt->sprite_h)
					)
	{
	//	g_ShowItemcounter(0);
		lb_PlaySound(18,gSoundVol); // 1
	//	g_GetVelocityDir(&CopyEnt->velx, &CopyEnt->vely, CopyEnt->x, CopyEnt->y, 0, 0);
	//	CopyEnt->ItemCatched = true;
		player_MakeSuperBumpers();
		g_RemoveSprite(CopyEnt);
		return true;
	}
	return false;
}

bool g_Collision_Banane(pEnt PlayerEnt, pEnt CopyEnt)
{
	if(!PlayerEnt)
		return false;
	if(!CopyEnt)
		return false;
	if(CopyEnt->sprite_id != ENT_BANANE)
		return false;

	if(CopyEnt->vely < 0)
		return false;

	if(CopyEnt->ItemCatched == false)
	if(g_MordTile_2(
						(int)PlayerEnt->x,
						(int)PlayerEnt->y,
						PlayerEnt->sprite_w,
						PlayerEnt->sprite_h,
						(int)CopyEnt->x,
						(int)CopyEnt->y,
						CopyEnt->sprite_w,
						CopyEnt->sprite_h)
					)
	{
		g_ShowItemcounter(0);
		lb_PlaySound(16,gSoundVol); // 1
		g_GetVelocityDir(&CopyEnt->velx, &CopyEnt->vely, CopyEnt->x, CopyEnt->y, 0, 0);
		CopyEnt->ItemCatched = true;
		CopyEnt->IsGravity = false;
		return true;
	}
	return false;
}

bool g_Collision_Key(pEnt PlayerEnt, pEnt CopyEnt)
{
	if(!PlayerEnt)
		return false;
	if(!CopyEnt)
		return false;
	if(CopyEnt->sprite_id != ENT_KEY)
		return false;

	if(CopyEnt->ItemCatched == false)
	if(g_MordTile_2(
						(int)PlayerEnt->x,
						(int)PlayerEnt->y,
						PlayerEnt->sprite_w,
						PlayerEnt->sprite_h,
						(int)CopyEnt->x,
						(int)CopyEnt->y,
						CopyEnt->sprite_w,
						CopyEnt->sprite_h)
					)
	{
		g_ShowItemcounter(2);
		lb_PlaySound(12,gSoundVol); // 1
		g_GetVelocityDir(&CopyEnt->velx, &CopyEnt->vely, CopyEnt->x, CopyEnt->y, 304, 0);
		CopyEnt->ItemCatched = true;
		CopyEnt->IsGravity = false;
		return true;
	}
	return false;
}

bool g_Collision_Vie(pEnt PlayerEnt, pEnt CopyEnt)
{
	if(!PlayerEnt)
		return false;
	if(!CopyEnt)
		return false;
	if(CopyEnt->sprite_id != ENT_LIFEUP)
		return false;

	if(CopyEnt->ItemCatched == false)
	if(g_MordTile_2(
						(int)PlayerEnt->x,
						(int)PlayerEnt->y,
						PlayerEnt->sprite_w,
						PlayerEnt->sprite_h,
						(int)CopyEnt->x,
						(int)CopyEnt->y,
						CopyEnt->sprite_w,
						CopyEnt->sprite_h)
					)
	{
		g_ShowItemcounter(1);
		lb_PlaySound(9,gSoundVol);
		g_GetVelocityDir(&CopyEnt->velx, &CopyEnt->vely, CopyEnt->x, CopyEnt->y, 513, 4);
		CopyEnt->ItemCatched = true;
		CopyEnt->IsGravity = false;
		return true;
	}
	return false;
}

bool g_Collision_Diamant(pEnt PlayerEnt, pEnt CopyEnt)
{
	if(!PlayerEnt)
		return false;
	if(!CopyEnt)
		return false;
	if(CopyEnt->sprite_id != ENT_DIAMANT)
		return false;

	if(CopyEnt->ItemCatched == false)
	if(g_MordTile_2(
						(int)PlayerEnt->x,
						(int)PlayerEnt->y,
						PlayerEnt->sprite_w,
						PlayerEnt->sprite_h,
						(int)CopyEnt->x,
						(int)CopyEnt->y,
						CopyEnt->sprite_w,
						CopyEnt->sprite_h)
					)
	{
		lb_PlaySound(9,gSoundVol);
		g_RemoveSprite(CopyEnt);
		gIsDiamantGrabed = true;
		g_AddParticles(ENT_STAR, (int)CopyEnt->x + DIAMANT_FW/2, (int)CopyEnt->y);
		return true;
	}
	return false;
}

void g_PangProcess()
{
	pEnt	Ent;
	pEnt	PangEnt;

	PangEnt = NULL;
	for(Ent=gEntityList ; Ent ; Ent=Ent->Next)
	{
		if(Ent->sprite_id == ENT_PANG)
		{
			if(Ent->nframe >= PANG_FRAME-1)
			{
				Ent->fatiguetime -= gFrameTime_msec;
				if(Ent->fatiguetime < -90)
				{
					g_RemoveSprite(Ent);
					Ent=gEntityList;
					continue;
				}
			}
		}
	}
}

void g_SmokeProcess()
{
	pEnt	Ent;
	pEnt	PlayerEnt;
	static	float	smoke_t = 0;

	PlayerEnt = NULL;
	for(Ent=gEntityList ; Ent ; Ent=Ent->Next)
	{
		if(Ent->sprite_id == ENT_SMOKE)
		{
			if(Ent->nframe >= SMOKE_FRAME-1)
			{
				g_RemoveSprite(Ent);
				Ent=gEntityList;
				continue;
			}
		}
		else if(Ent->sprite_id == ENT_PLAYER)
		{
			PlayerEnt = Ent;
		}
	}

	smoke_t -= gFrameTime_msec;
	if(smoke_t < 0)
		smoke_t = 0;

	if(PlayerEnt && gPlayerRun)
	if(PlayerEnt->dir == 0 || PlayerEnt->dir == 1)
	{
		if(smoke_t == 0)
		{
			smoke_t = 200.0f;
			g_AddSprite(ENT_SMOKE, (int) PlayerEnt->x-10+PlayerEnt->sprite_w/2, (int) PlayerEnt->y-10+PlayerEnt->sprite_h, 0, 0, true);
		}
	}
}

void g_UpdatePlayer_ItemCollision(pEnt PlayerEnt)
{
	pEnt	Ent;
	pEnt	CopyEnt;

	for(Ent=gEntityList ; Ent ; )
	{
		CopyEnt = Ent;
		Ent=Ent->Next;

		// on rencontre un bumper
		if(CopyEnt->sprite_id == ENT_BUMPER)
		{
			g_Collision_Bumper(PlayerEnt, CopyEnt);
		}

		// un ascenseur horizontal
		if(CopyEnt->sprite_id == ENT_HLIFT)
		{
			// si le joueur est deja sur un ascenseur pas la peine de gerer les collisions sur un autre asc.
			if(PlayerEnt->OnLift)
			if(CopyEnt->OnLift == false)
				continue;
			g_Collision_HLift(PlayerEnt, CopyEnt);
		}

		// les pommes
		if(CopyEnt->sprite_id == ENT_POMME)
		{
			if(g_Collision_Pomme(PlayerEnt, CopyEnt))
			{
				Ent=gEntityList;
			}
		}

		// les pommes jaune
		if(CopyEnt->sprite_id == ENT_POMMEJ)
		{
			if(g_Collision_PommeJ(PlayerEnt, CopyEnt))
			{
				Ent=gEntityList;
			}
		}		
		
		// les pommes verte
		if(CopyEnt->sprite_id == ENT_POMMEV)
		{
			if(g_Collision_PommeV(PlayerEnt, CopyEnt))
			{
				Ent=gEntityList;
			}
		}

		// les bananes
		if(CopyEnt->sprite_id == ENT_BANANE)
		{
			if(g_Collision_Banane(PlayerEnt, CopyEnt))
			{
				Ent=gEntityList;
			}
		}

		// les vies
		if(CopyEnt->sprite_id == ENT_LIFEUP)
		{
			if(g_Collision_Vie(PlayerEnt, CopyEnt))
			{
				Ent=gEntityList;
			}
		}

		// les clees
		if(CopyEnt->sprite_id == ENT_KEY)
		{
			if(g_Collision_Key(PlayerEnt, CopyEnt))
			{
				Ent=gEntityList;
			}
		}

		// les diamants
		if(CopyEnt->sprite_id == ENT_DIAMANT)
		{
			if(g_Collision_Diamant(PlayerEnt, CopyEnt))
			{
				Ent=gEntityList;
			}
		}

	}
}

