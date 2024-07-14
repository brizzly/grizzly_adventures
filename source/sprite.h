#ifndef SPRITE_INC__
#define SPRITE_INC__


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
//#include "directx.h"


typedef struct entype *pEnt;
typedef struct entype
{
	int		sprite_id;
	int		dir;
	float	x;
	float	y;
	float	velx;
	float	vely;
	float	sinus_x;
	float	sinus_y;
	int		sprite_x;
	int		sprite_y;
	int		sprite_w;
	int		sprite_h;
	int		sprite_cw;
	int		sprite_ch;
	int		nframe;
	float	frametime;
	float	fatiguetime;
	bool	RotateDir;
	bool	IsFatigue;
	bool	ItemCatched;
	bool	IsGravity;
	short	lift_type;
	bool	RemoveMe;
	bool	LiftCollOk;		//
	bool	OnLift;			// l'entitee est sur un ascenseur
	bool	activated;		// for bumper anim
	bool	hit;			// for player or monster that has been hit by something
	short	ia_dir;
	short	move_state;
	struct	entype *Next;

} Ent_t;

enum
{
	ENT_PLAYER,
	ENT_MONSTER1,
	ENT_MONSTER2,
	ENT_MONSTER3,
	ENT_MONSTER4,	// ghost
	ENT_MONSTER5,	// spider
	ENT_MONSTER6,	// chenille
	ENT_POMME,
	ENT_POMMEJ,
	ENT_POMMEV,
	ENT_BANANE,
	ENT_FEUILLE,
	ENT_BUMPER,
	ENT_COFFRE,
	ENT_POTMIEL,
	ENT_HLIFT,
	ENT_LIFEUP,
	ENT_STAR,
	ENT_REDSTAR,
	ENT_GREENSTAR,
	ENT_EXITAD,
	ENT_SMOKE,
	ENT_KEY,
	ENT_DIAMANT,
	ENT_PANG
};

enum
{
	MOVE_GD,
	MOVE_HB,
	MOVE_ROT
};


void	g_CleanupSpriteList();
pEnt	g_AddSprite(int sprite_id, int posx, int posy, float vx, float vy, bool ajout_sens);
void	g_RemoveSprite(pEnt SpriteEnt);
pEnt	g_FindSprite(int sprite_id, int pos);
void	g_AddBumperJump(pEnt Ent, float JumpValue);
bool	g_DrawSprites();
void	g_AnimateSprite();
void	g_AddGravity(pEnt Ent);
bool	g_UpdateMove(pEnt Ent);
bool	g_BoundPos(float *x, float *y, int w, int h);
void	g_DrawOntopSprites();
void	g_AddVelEntity(pEnt Ent);






#endif SPRITE_INC__

