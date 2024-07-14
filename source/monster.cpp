
#include "game.h"
#include "sprite.h"
#include "world.h"
#include "sprite_process.h"
#include "collision.h"
#include "monster.h"



extern	float	gFrameTime_msec;
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
extern	pEnt	gEntityList;
extern	float	gEffet_EarthQuake;
extern	float	gBzzTime;
extern	int		gOffset_X;
extern	int		gOffset_Y;
extern	sHeaderGrizzly	gHeader;
extern	int		gNinvinsibleStars;
extern	int		ProgramState;
extern	int		gEpisodeID;



// si un monstre sort de la map on l'élimine du jeu
void monster_CheckPos()
{
	pEnt	Ent;
	int		nb_monsters;

	nb_monsters = 0;
	for(Ent=gEntityList ; Ent ; Ent=Ent->Next)
	{
		if(	Ent->sprite_id == ENT_MONSTER1 ||
			Ent->sprite_id == ENT_MONSTER2 ||
			Ent->sprite_id == ENT_MONSTER3)
		{
			if((int)Ent->x + Ent->sprite_w + gOffset_X < 0)
			{
				Ent->RemoveMe = true;
			}
			if(false == Ent->hit)
			if((int)Ent->y + Ent->sprite_h + gOffset_Y < 0)
			{
				Ent->RemoveMe = true;
			}
			if((int)Ent->x + gOffset_X > gHeader.level_w*TILE_X)
			{
				Ent->RemoveMe = true;
			}
			if((int)Ent->y + gOffset_Y > gHeader.level_h*TILE_Y)
			{
				Ent->RemoveMe = true;
			}
			nb_monsters++;
		}
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

	//g_DrawString(10,10,"nb_monsters: %d", nb_monsters);
}

void monster_UpdateAbeille(pEnt AbeilleEnt)
{
	pEnt	PlayerEnt;
	int		valx;
	int		valy;

	if(!AbeilleEnt)
		return;
	PlayerEnt = g_FindSprite(ENT_PLAYER,1);
	if(!PlayerEnt)
		return;

	// test si l'abeille mord le joueur
	valx = abs(((int)PlayerEnt->x+PlayerEnt->sprite_w)/2 - ((int)AbeilleEnt->x+AbeilleEnt->sprite_w)/2);
	valy = abs(((int)PlayerEnt->y+PlayerEnt->sprite_h)/2 - ((int)AbeilleEnt->y+AbeilleEnt->sprite_h)/2);
	if(valx < 10 && valy < 20)
	{
		// player kill the monster
		if(gInvinsibleTime > 0)
		{
			gNinvinsibleStars--;
			lb_PlaySound(10, gSoundVol);
			AbeilleEnt->hit = true;
			AbeilleEnt->IsGravity = true;
			g_AddBumperJump(AbeilleEnt, PLAYER_JUMP*0.80f);
		}
		else
		{
			g_AddParticles(ENT_REDSTAR, (int)PlayerEnt->x, (int)PlayerEnt->y);
			lb_PlaySound(2, gSoundVol);
			PlayerEnt->vely = 0;
			g_AddBumperJump(PlayerEnt, PLAYER_JUMP);
			PlayerEnt->vely /= 2;
			g_LostLive();
		}
		return;
	}
	if(valx < 150 && valy < 150)
	{
		if(gBzzTime <= 0)
		{
			gBzzTime = BZZ_TIME;
			lb_PlaySound(2, gSoundVol);	//Bzz Bzz
		}
		else
		{
			gBzzTime -= gFrameTime_msec;
		}
	}

	// l'abeille se tourne dans la direction du joueur
	if(AbeilleEnt->x+AbeilleEnt->sprite_w/2 > PlayerEnt->x+PlayerEnt->sprite_w/2)
	{
		AbeilleEnt->dir = 0;
	}
	else
	{
		AbeilleEnt->dir = 1;
	}
}

void monster_UpdateGhost(pEnt GhostEnt)
{
	pEnt	PlayerEnt;
	float	velx;
	float	vely;
	int		valx;
	int		valy;

	if(!GhostEnt)
		return;
	PlayerEnt = g_FindSprite(ENT_PLAYER, 1);
	if(!PlayerEnt)
		return;

	if(PlayerEnt->x <= GhostEnt->x)
	{
		GhostEnt->dir = 1;
	}
	else
	{
		GhostEnt->dir = 0;
	}

	// seulement si le joueur regarde pas le fantome, celui si se rapproche
	if( ((PlayerEnt->dir == 1 || PlayerEnt->dir == 3 || PlayerEnt->dir == 5) && GhostEnt->dir == 1) ||
		((PlayerEnt->dir == 0 || PlayerEnt->dir == 2 || PlayerEnt->dir == 4) && GhostEnt->dir == 0) )
	{
		g_GetVelocityDir(&velx, &vely, GhostEnt->x, GhostEnt->y, PlayerEnt->x, PlayerEnt->y);

		if(velx > 0)
		{
			GhostEnt->velx += 0.01f;
			if(GhostEnt->velx > 1.3f)
				GhostEnt->velx = 1.3f;
		}
		else if(velx < 0)
		{
			GhostEnt->velx -= 0.01f;
			if(GhostEnt->velx < -1.3f)
				GhostEnt->velx = -1.3f;
		}

		if(vely > 0)
		{
			GhostEnt->vely += 0.008f;
			if(GhostEnt->vely > 0.8f)
				GhostEnt->vely = 0.8f;
		}
		else if(vely < 0)
		{
			GhostEnt->vely -= 0.008f;
			if(GhostEnt->vely < -0.8f)
				GhostEnt->vely = -0.8f;
		}
	}
	else
	{
		GhostEnt->velx = 0;
		GhostEnt->vely = 0;
	}

	// test si le fantome mord le joueur
	valx = abs(((int)PlayerEnt->x+PlayerEnt->sprite_w)/2 - ((int)GhostEnt->x+GhostEnt->sprite_w)/2);
	valy = abs(((int)PlayerEnt->y+PlayerEnt->sprite_h)/2 - ((int)GhostEnt->y+GhostEnt->sprite_h)/2);
	if(valx < 10 && valy < 20)
	{
		// player kill the monster
		if(gInvinsibleTime > 0)
		{
			gNinvinsibleStars--;
			monster_BeingHit(GhostEnt);
			GhostEnt->IsGravity = true;
			g_AddBumperJump(GhostEnt, PLAYER_JUMP*0.80f);
		}
		else
		{
			g_AddParticles(ENT_REDSTAR, (int)PlayerEnt->x, (int)PlayerEnt->y);
			lb_PlaySound(2, gSoundVol);
			PlayerEnt->vely = 0;
			g_AddBumperJump(PlayerEnt, PLAYER_JUMP);
			PlayerEnt->vely /= 2;
			g_LostLive();
		}
	}
}

void monster_UpdateSpider(pEnt SpiderEnt)
{
	pEnt	PlayerEnt;
	int		valx;
	int		valy;
	int		tile_val;
	float	offset_y;

	if(!SpiderEnt)
		return;

	// deplacements haut bas
	offset_y = 0.20f * gFrameTime_msec * (float)sin(SpiderEnt->sinus_y);
	tile_val = g_MurBas(	(int)SpiderEnt->x + (SpiderEnt->sprite_w - SpiderEnt->sprite_cw)/2,
							(int)SpiderEnt->y + (SpiderEnt->sprite_h - SpiderEnt->sprite_ch)/2,
							SpiderEnt->sprite_cw,
							SpiderEnt->sprite_ch, MUR_ID);
//	if((tile_val == 0 || offset_y <= 0) && (int)SpiderEnt->x + gOffset_X <= gHeader.level_w*TILE_X)
	if(tile_val == 0 || offset_y <= 0)
	{
		// hack
		if(gEpisodeID == 3)
			SpiderEnt->y += offset_y * 0.5f;
		else
			SpiderEnt->y += offset_y;
		SpiderEnt->sinus_y += (0.002f * gFrameTime_msec);
		if(SpiderEnt->sinus_y >= 6.25)
		{
			SpiderEnt->sinus_y -= 6.25;
		}
	}
	else
	{
		SpiderEnt->sinus_y += 3.14f;
	}

	PlayerEnt = g_FindSprite(ENT_PLAYER, 1);
	if(!PlayerEnt)
		return;

	// test si la spider mord le joueur
	valx = abs(((int)PlayerEnt->x+PlayerEnt->sprite_w)/2 - ((int)SpiderEnt->x+SpiderEnt->sprite_w)/2);
	valy = abs(((int)PlayerEnt->y+PlayerEnt->sprite_h)/2 - ((int)SpiderEnt->y+SpiderEnt->sprite_h)/2);
	if(valx < 10 && valy < 20)
	{
		// player kill the monster
		if(gInvinsibleTime > 0)
		{
			gNinvinsibleStars--;
			monster_BeingHit(SpiderEnt);
			SpiderEnt->IsGravity = true;
			g_AddBumperJump(SpiderEnt, PLAYER_JUMP*0.80f);
		}
		else
		{
			g_AddParticles(ENT_REDSTAR, (int)PlayerEnt->x, (int)PlayerEnt->y);
			lb_PlaySound(2, gSoundVol);
			PlayerEnt->vely = 0;
			g_AddBumperJump(PlayerEnt, PLAYER_JUMP);
			PlayerEnt->vely /= 2;
			g_LostLive();
		}
	}

	// l'arraignée se tourne dans la direction du joueur
	if(SpiderEnt->x+SpiderEnt->sprite_w/2 > PlayerEnt->x+PlayerEnt->sprite_w/2)
	{
		SpiderEnt->dir = 0;
	}
	else
	{
		SpiderEnt->dir = 1;
	}
}

void monster_UpdateSanglier(pEnt SanglierEnt)
{
	pEnt	PlayerEnt;
	int		valx;
	int		valy;
	int		tile_val;

	if(!SanglierEnt)
		return;
	PlayerEnt = g_FindSprite(ENT_PLAYER, 1);
	if(!PlayerEnt)
		return;

	// test si le monstre mord le joueur
	valx = abs((int)PlayerEnt->x - (int)SanglierEnt->x);
	valy = abs((int)PlayerEnt->y - (int)SanglierEnt->y);
	if(valx < 28 && valy < 40)
	{
		// player kill the monster
		if(gInvinsibleTime > 0)
		{
			gNinvinsibleStars--;
			lb_PlaySound(13, gSoundVol);
			monster_BeingHit(SanglierEnt);
			g_AddBumperJump(SanglierEnt, PLAYER_JUMP*0.80f);
		}
	//	else if(PlayerEnt->y + PlayerEnt->sprite_ch < SanglierEnt->y + SanglierEnt->sprite_ch*0.80f) //67
		else if(	PlayerEnt->y + (PlayerEnt->sprite_h+PlayerEnt->sprite_ch)/2 >=
						SanglierEnt->y + (SanglierEnt->sprite_h-SanglierEnt->sprite_ch)/2
				&&	PlayerEnt->y + (PlayerEnt->sprite_h+PlayerEnt->sprite_ch)/2 <=
						SanglierEnt->y + SanglierEnt->sprite_h
				&&	PlayerEnt->vely > 0)
		{
			g_AddParticles(ENT_STAR, (int)PlayerEnt->x+PLAYER_FW/2, (int)PlayerEnt->y+PLAYER_FH);
			monster_BeingHit(SanglierEnt);
			g_AddBumperJump(SanglierEnt, PLAYER_JUMP*0.80f);
			PlayerEnt->vely = 0;
			g_AddBumperJump(PlayerEnt, PLAYER_JUMP*0.67f);
		}
		else
		{
			g_AddParticles(ENT_REDSTAR, (int)PlayerEnt->x, (int)PlayerEnt->y);
			PlayerEnt->vely = 0;
			g_AddBumperJump(PlayerEnt, PLAYER_JUMP);
			g_LostLive();
		}
		return;
	}

	if(SanglierEnt->dir == 0)
	{
		SanglierEnt->x += 1;

		// test si ya un mur a droite
		tile_val = g_MurDroite(	(int)SanglierEnt->x + (SanglierEnt->sprite_w - SanglierEnt->sprite_cw)/2,
								(int)SanglierEnt->y + (SanglierEnt->sprite_h - SanglierEnt->sprite_ch)/2,
								SanglierEnt->sprite_cw,
								SanglierEnt->sprite_ch);

		if(tile_val == 0 || tile_val == SMUR_ID)
		{			
		}
		else
		{
			SanglierEnt->dir = 1;
			g_FixeDroite(&SanglierEnt->x, (SanglierEnt->sprite_w - SanglierEnt->sprite_cw)/2);
		}

		// test si ya le vide a droite
		tile_val = g_MurDroite(	(int)SanglierEnt->x + (SanglierEnt->sprite_w - SanglierEnt->sprite_cw)/2,
								TILE_Y+(int)SanglierEnt->y + (SanglierEnt->sprite_h - SanglierEnt->sprite_ch)/2,
								SanglierEnt->sprite_cw,
								SanglierEnt->sprite_ch);

		if(tile_val == 0 && SanglierEnt->vely == 0)
		{
			SanglierEnt->dir = 1;
		}
	}
	else
	{
		SanglierEnt->x -= 1;

		// test si ya un mur a gauche
		tile_val = g_MurGauche(	(int)SanglierEnt->x + (SanglierEnt->sprite_w - SanglierEnt->sprite_cw)/2,
								(int)SanglierEnt->y + (SanglierEnt->sprite_h - SanglierEnt->sprite_ch)/2,
								SanglierEnt->sprite_cw,
								SanglierEnt->sprite_ch);

		if(tile_val == 0 || tile_val == SMUR_ID)
		{
					
		}
		else
		{
			SanglierEnt->dir = 0;
			g_FixeGauche(&SanglierEnt->x, (SanglierEnt->sprite_w - SanglierEnt->sprite_cw)/2);
		}

		// test si ya le vide a gauche
		tile_val = g_MurGauche(	(int)SanglierEnt->x + (SanglierEnt->sprite_w - SanglierEnt->sprite_cw)/2,
								TILE_Y+(int)SanglierEnt->y + (SanglierEnt->sprite_h - SanglierEnt->sprite_ch)/2,
								SanglierEnt->sprite_cw,
								SanglierEnt->sprite_ch);

		if(tile_val == 0 && SanglierEnt->vely == 0)
		{
			SanglierEnt->dir = 0;
		}
	}

	// fumee
	SanglierEnt->fatiguetime -= gFrameTime_msec;
	if(SanglierEnt->fatiguetime < -309)
	{
		SanglierEnt->fatiguetime = 0;
		g_AddSprite(ENT_SMOKE, (int) SanglierEnt->x-10+SanglierEnt->sprite_w/2, (int) SanglierEnt->y-10+SanglierEnt->sprite_h, 0, 0, true);
	}
}

void monster_UpdateMBleu(pEnt MBleuEnt)
{
	pEnt	PlayerEnt;
	int		valx;
	int		valy;
	int		tile_val;
	float	OldXpos;

	if(!MBleuEnt)
		return;
	PlayerEnt = g_FindSprite(ENT_PLAYER, 1);
	if(!PlayerEnt)
		return;

	// test si le monstre mord le joueur
	valx = abs((int)PlayerEnt->x - (int)MBleuEnt->x);
	valy = abs((int)PlayerEnt->y - (int)MBleuEnt->y);
	if(valx < 20 && valy < 40)
//	if(valx < MBleuEnt->sprite_cw && valy < MBleuEnt->sprite_ch)
	{
		// player kill the monster
		if(gInvinsibleTime > 0)
		{
			gNinvinsibleStars--;
			monster_BeingHit(MBleuEnt);
			g_AddBumperJump(MBleuEnt, PLAYER_JUMP*0.80f);
		}
	//	else if(PlayerEnt->y + PlayerEnt->sprite_ch < MBleuEnt->y + MBleuEnt->sprite_ch*0.80f) //40
		else if(	PlayerEnt->y + (PlayerEnt->sprite_h+PlayerEnt->sprite_ch)/2 >=
						MBleuEnt->y + (MBleuEnt->sprite_h-MBleuEnt->sprite_ch)/2
				&&	PlayerEnt->y + (PlayerEnt->sprite_h+PlayerEnt->sprite_ch)/2 <=
						MBleuEnt->y + MBleuEnt->sprite_h
				&&	PlayerEnt->vely > 0)
		{
			g_AddParticles(ENT_STAR, (int)PlayerEnt->x+PLAYER_FW/2, (int)PlayerEnt->y+PLAYER_FH);
			monster_BeingHit(MBleuEnt);
			g_AddBumperJump(MBleuEnt, PLAYER_JUMP*0.80f);
			PlayerEnt->vely = 0;
			g_AddBumperJump(PlayerEnt, PLAYER_JUMP*0.67f);
		}
		else
		{
			g_AddParticles(ENT_REDSTAR, (int)PlayerEnt->x, (int)PlayerEnt->y);
			PlayerEnt->vely = 0;
			g_AddBumperJump(PlayerEnt, PLAYER_JUMP);
			g_LostLive();
		}
		return;
	}
	
	// deplacements du monstre
	OldXpos = MBleuEnt->x;
	if(MBleuEnt->dir != 0)
	{
		MBleuEnt->x += 1;
		tile_val = g_MurDroite(	(int)MBleuEnt->x + (MBleuEnt->sprite_w - MBleuEnt->sprite_cw)/2,
								(int)MBleuEnt->y + (MBleuEnt->sprite_h - MBleuEnt->sprite_ch)/2,
								MBleuEnt->sprite_cw,
								MBleuEnt->sprite_ch);

		if(tile_val == 0 || tile_val == SMUR_ID)
		{
					
		}
		else
		{
			MBleuEnt->dir = 0;
			g_FixeDroite(&MBleuEnt->x, (MBleuEnt->sprite_w - MBleuEnt->sprite_cw)/2);
		}

		// test si ya le vide a droite
		tile_val = g_MurDroite(	(int)MBleuEnt->x + (MBleuEnt->sprite_w - MBleuEnt->sprite_cw)/2,
								TILE_Y+(int)MBleuEnt->y + (MBleuEnt->sprite_h - MBleuEnt->sprite_ch)/2,
								MBleuEnt->sprite_cw,
								MBleuEnt->sprite_ch);

		if(tile_val == 0 && MBleuEnt->vely == 0)
		{
			MBleuEnt->dir = 0;
		}
	}
	else
	{
		MBleuEnt->x -= 1;
		tile_val = g_MurGauche(	(int)MBleuEnt->x + (MBleuEnt->sprite_w - MBleuEnt->sprite_cw)/2,
								(int)MBleuEnt->y + (MBleuEnt->sprite_h - MBleuEnt->sprite_ch)/2,
								MBleuEnt->sprite_cw,
								MBleuEnt->sprite_ch);

		if(tile_val == 0 || tile_val == SMUR_ID)
		{
					
		}
		else
		{
			MBleuEnt->dir = 1;
			g_FixeGauche(&MBleuEnt->x, (MBleuEnt->sprite_w - MBleuEnt->sprite_cw)/2);
		}

		// test si ya le vide a gauche
		tile_val = g_MurGauche(	(int)MBleuEnt->x + (MBleuEnt->sprite_w - MBleuEnt->sprite_cw)/2,
								TILE_Y+(int)MBleuEnt->y + (MBleuEnt->sprite_h - MBleuEnt->sprite_ch)/2,
								MBleuEnt->sprite_cw,
								MBleuEnt->sprite_ch);

		if(tile_val == 0 && MBleuEnt->vely == 0)
		{
			MBleuEnt->dir = 1;
		}
	}

	// gestion des frames de fatigue
	if(MBleuEnt->IsFatigue == false)
	{
		if(MBleuEnt->dir != 0 && MBleuEnt->dir != 1)
			MBleuEnt->dir -= 2;
		MBleuEnt->fatiguetime += gFrameTime_msec;
		if(MBleuEnt->fatiguetime >= MONS_FATIGUE_T)
		{
			MBleuEnt->fatiguetime = MONS_FATIGUE_T - 2000 + (rand()%20) * 100;
			MBleuEnt->IsFatigue = true;
		}
	}
	else
	{
		MBleuEnt->x = OldXpos;

		if(MBleuEnt->dir == 0 || MBleuEnt->dir == 1)
			MBleuEnt->dir += 2;
		MBleuEnt->fatiguetime -= gFrameTime_msec;
		if(MBleuEnt->fatiguetime <= 0)
		{
			MBleuEnt->fatiguetime = 0;
			MBleuEnt->IsFatigue = false;
		}
	}
}

void monster_UpdateChenille(pEnt ChenilleEnt)
{
	pEnt	PlayerEnt;
	int		valx;
	int		valy;
	int		tile_val;
	float	OldXpos;

	if(!ChenilleEnt)
		return;
	PlayerEnt = g_FindSprite(ENT_PLAYER, 1);
	if(!PlayerEnt)
		return;

	// test si le monstre mord le joueur
	valx = abs((int)PlayerEnt->x - (int)ChenilleEnt->x);
	valy = abs((int)PlayerEnt->y - (int)ChenilleEnt->y);
	if(valx < 26 && valy < 40)
//	if(valx < (ChenilleEnt->sprite_w/2)+10 && valy < (ChenilleEnt->sprite_h/2)+10)
	{
		// player kill the monster
		if(gInvinsibleTime > 0)
		{
			gNinvinsibleStars--;
			monster_BeingHit(ChenilleEnt);
			g_AddBumperJump(ChenilleEnt, PLAYER_JUMP*0.85f);
		}
		else if(	PlayerEnt->y + (PlayerEnt->sprite_h+PlayerEnt->sprite_ch)/2 >=
						ChenilleEnt->y + (ChenilleEnt->sprite_h-ChenilleEnt->sprite_ch)/2
				&&	PlayerEnt->y + (PlayerEnt->sprite_h+PlayerEnt->sprite_ch)/2 <=
						ChenilleEnt->y + ChenilleEnt->sprite_h
				&&	PlayerEnt->vely > 0)
		{
			g_AddParticles(ENT_STAR, (int)PlayerEnt->x+PLAYER_FW/2, (int)PlayerEnt->y+PLAYER_FH);
			monster_BeingHit(ChenilleEnt);
			g_AddBumperJump(ChenilleEnt, PLAYER_JUMP*0.80f);
			PlayerEnt->vely = 0;
			g_AddBumperJump(PlayerEnt, PLAYER_JUMP*0.67f);
		}
		else //if(	PlayerEnt->vely <= 0	
			//	&&	valx < 26 && valy < 35)
		{
			g_AddParticles(ENT_REDSTAR, (int)PlayerEnt->x, (int)PlayerEnt->y);
			PlayerEnt->vely = 0;
			g_AddBumperJump(PlayerEnt, PLAYER_JUMP);
			g_LostLive();
		}
		return;
	}

	// deplacements du monstre
	OldXpos = ChenilleEnt->x;
	
	// hack: la chenille avance seulement selon certaines frames d'anim
	if(ChenilleEnt->nframe == 1 || ChenilleEnt->nframe == 2)
		goto no_move_chenille;
	
	if(ChenilleEnt->dir != 0)
	{
		ChenilleEnt->x += 0.5f;
		tile_val = g_MurDroite(	(int)ChenilleEnt->x + (ChenilleEnt->sprite_w - ChenilleEnt->sprite_cw)/2,
								(int)ChenilleEnt->y + (ChenilleEnt->sprite_h - ChenilleEnt->sprite_ch)/2,
								ChenilleEnt->sprite_cw,
								ChenilleEnt->sprite_ch);

		if(tile_val == 0 || tile_val == SMUR_ID)
		{
					
		}
		else
		{
			ChenilleEnt->dir = 0;
			g_FixeDroite(&ChenilleEnt->x, (ChenilleEnt->sprite_w - ChenilleEnt->sprite_cw)/2);
		}

		// test si ya le vide a droite
		tile_val = g_MurDroite(	(int)ChenilleEnt->x + (ChenilleEnt->sprite_w - ChenilleEnt->sprite_cw)/2,
								TILE_Y+(int)ChenilleEnt->y + (ChenilleEnt->sprite_h - ChenilleEnt->sprite_ch)/2,
								ChenilleEnt->sprite_cw,
								ChenilleEnt->sprite_ch);

		if(tile_val == 0 && ChenilleEnt->vely == 0)
		{
			ChenilleEnt->dir = 0;
		}
	}
	else
	{
		ChenilleEnt->x -= 0.5f;
		tile_val = g_MurGauche(	(int)ChenilleEnt->x + (ChenilleEnt->sprite_w - ChenilleEnt->sprite_cw)/2,
								(int)ChenilleEnt->y + (ChenilleEnt->sprite_h - ChenilleEnt->sprite_ch)/2,
								ChenilleEnt->sprite_cw,
								ChenilleEnt->sprite_ch);

		if(tile_val == 0 || tile_val == SMUR_ID)
		{
					
		}
		else
		{
			ChenilleEnt->dir = 1;
			g_FixeGauche(&ChenilleEnt->x, (ChenilleEnt->sprite_w - ChenilleEnt->sprite_cw)/2);
		}

		// test si ya le vide a gauche
		tile_val = g_MurGauche(	(int)ChenilleEnt->x + (ChenilleEnt->sprite_w - ChenilleEnt->sprite_cw)/2,
								TILE_Y+(int)ChenilleEnt->y + (ChenilleEnt->sprite_h - ChenilleEnt->sprite_ch)/2,
								ChenilleEnt->sprite_cw,
								ChenilleEnt->sprite_ch);

		if(tile_val == 0 && ChenilleEnt->vely == 0)
		{
			ChenilleEnt->dir = 1;
		}
	}

no_move_chenille:

	// gestion des frames de fatigue
	if(ProgramState == PS_ACTIVE)
	{
//		if(ChenilleEnt->IsFatigue == false)
		{
		//	if(ChenilleEnt->dir != 0 && ChenilleEnt->dir != 1)
		//		ChenilleEnt->dir -= 2;
			ChenilleEnt->fatiguetime += gFrameTime_msec;
			if(ChenilleEnt->fatiguetime >= MONS_FATIGUE_T)
			{
				ChenilleEnt->fatiguetime = 0; //MONS_FATIGUE_T;
			//	ChenilleEnt->IsFatigue = true;
				g_AddBumperJump(ChenilleEnt, PLAYER_JUMP * 0.20f);
			}
		}
		/*
		else
		{
			ChenilleEnt->x = OldXpos;

		//	if(ChenilleEnt->dir == 0 || ChenilleEnt->dir == 1)
		//		ChenilleEnt->dir += 2;
			ChenilleEnt->fatiguetime -= gFrameTime_msec;
			if(ChenilleEnt->fatiguetime <= 0)
			{
				ChenilleEnt->fatiguetime = 0;
				ChenilleEnt->IsFatigue = false;
			}
		}
		*/
	}
}

void monster_Update()
{
	pEnt	Ent;
	pEnt	CopyEnt;

	if(ProgramState == PS_PAUSE)
		return;

	monster_CheckPos();

	for(Ent=gEntityList ; Ent ; )
	{
		CopyEnt = Ent;
		Ent=Ent->Next;

		if(	CopyEnt->sprite_id != ENT_MONSTER1 &&
			CopyEnt->sprite_id != ENT_MONSTER2 &&
			CopyEnt->sprite_id != ENT_MONSTER3 &&
			CopyEnt->sprite_id != ENT_MONSTER4 &&
			CopyEnt->sprite_id != ENT_MONSTER5 &&
			CopyEnt->sprite_id != ENT_MONSTER6 )
			continue;

		if(CopyEnt->hit)
			continue;

		// test si un effet POW est activé
		if(gEffet_EarthQuake)
		{
			if(CopyEnt->x >= 0 && CopyEnt->x <= ScreenX)
			if(CopyEnt->y >= 0 && CopyEnt->y <= ScreenY)
			{
				monster_BeingHit(CopyEnt);
				CopyEnt->IsGravity = true;
				g_AddBumperJump(CopyEnt, PLAYER_JUMP*0.80f);
				continue;
			}
		}

		switch(CopyEnt->sprite_id)
		{
			case ENT_MONSTER1:
			monster_UpdateSanglier(CopyEnt);
			break;

			case ENT_MONSTER2:
			monster_UpdateAbeille(CopyEnt);
			break;

			case ENT_MONSTER3:
			monster_UpdateMBleu(CopyEnt);
			break;

			case ENT_MONSTER4:
			monster_UpdateGhost(CopyEnt);
			break;

			case ENT_MONSTER5:
			monster_UpdateSpider(CopyEnt);
			break;

			case ENT_MONSTER6:
			monster_UpdateChenille(CopyEnt);
			break;
		}
	}
}

void monster_BeingHit(pEnt Entity)
{
	if(!Entity)
		return;
	Entity->hit = true;
	switch(Entity->sprite_id)
	{
		case ENT_MONSTER1:
		lb_PlaySound(13, gSoundVol);
		break;

		case ENT_MONSTER2:
		lb_PlaySound(10, gSoundVol);
		break;

		case ENT_MONSTER3:
		lb_PlaySound(0, gSoundVol); 
		break;

		case ENT_MONSTER4:
		lb_PlaySound(14, gSoundVol);
		break;

		case ENT_MONSTER5:
		lb_PlaySound(0, gSoundVol);
		break;

		case ENT_MONSTER6:
		lb_PlaySound(11, gSoundVol);
		break;
	}

	g_AddSprite(ENT_PANG, (int) Entity->x+Entity->sprite_w/2 - PANG_FW/2, (int) Entity->y+Entity->sprite_h/2 - PANG_FH/2 -10, 0, 0, true);
}