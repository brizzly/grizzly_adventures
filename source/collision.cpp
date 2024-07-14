
#include "game.h"
#include "world.h"
#include "collision.h"


extern	int		gOffset_X;
extern	int		gOffset_Y;
extern	sBlocGrizzly	*gLevel;
extern	sHeaderGrizzly	gHeader;
extern	int		gBlocDefinition[MAX_TILES];
extern	int		gTileCollisionID;
extern	int		gTilePos;



int g_MurDroite(int x, int y, int w, int h)
{
	int		position;
	int		brique;
	int		nTiles;
	int		y_test;
	int		index;
	int		h2;
	int		tile_val;

//	return 0; //

	h2 = h-1;
	nTiles = (int)(h2 / (TILE_Y+1)) + 1;

	x += (int)gOffset_X;
	y += (int)gOffset_Y;

	x += w;

	for(index=0 ; index<nTiles ; index++)
	{
		y_test = y + index*TILE_Y;
		if(y_test > y + h2)
			y_test = y + h2 - 1;

		position = (x/TILE_X) + (y_test/TILE_Y)*gHeader.level_w;
		if(position < 0 || position >= gHeader.level_w*gHeader.level_h)
			return 0;

		brique = gLevel[position].blocID - 48;
		tile_val = g_IsMur(brique);
		if(tile_val)
			return tile_val;
	}
	return 0;
}

int g_MurGauche(int x, int y, int w, int h)
{
	int		position;
	int		brique;
	int		nTiles;
	int		y_test;
	int		index;
	int		h2;
	int		tile_val;

//	return 0; //

	h2 = h-1;
	nTiles = (int)(h2 / (TILE_Y+1)) + 1;

	x += (int)gOffset_X;
	y += (int)gOffset_Y;

	x -= 1;

	for(index=0 ; index<nTiles ; index++)
	{
		y_test = y + index*TILE_Y;
		if(y_test > y + h2)
			y_test = y + h2 - 1;

		position = (x/TILE_X) + (y_test/TILE_Y)*gHeader.level_w;
		if(position < 0 || position >= gHeader.level_w*gHeader.level_h)
			return 0;

		brique = gLevel[position].blocID - 48;
		tile_val = g_IsMur(brique);
		if(tile_val)
			return tile_val;
	}
	return 0;
}

int g_MurHaut(int x, int y, int w, int h, int tile_id)
{
	int		position;
	int		brique;
	int		nTiles;
	int		x_test;
	int		index;
	int		tile_val;
	int		w2;

	//y += TILE_Y/2;

	w2 = w-1;
	nTiles = (int)(w2 / (TILE_X+1)) + 2;

	x += (int)gOffset_X;
	y += (int)gOffset_Y;

	for(index=0 ; index<nTiles ; index++)
	{
		x_test = x + 3 + index*TILE_X; // hack +3
		if(x_test >= x + w2) // >
			x_test = x + w2 - 1;

		position = (x_test/TILE_X) + (y/TILE_Y)*gHeader.level_w;
		if(position < 0 || position >= gHeader.level_w*gHeader.level_h)
			return 0;

		brique = gLevel[position].blocID - 48;
		
		// global tile collision infos
		gTileCollisionID = brique;
		gTilePos = position;
		
		if(tile_id == MUR_ID)
		{
			tile_val = g_IsMur(brique);
			if(tile_val)
				return tile_val;
		}
		if(brique == tile_id)
			goto ok_murhaut;
	}

	return 0;

ok_murhaut:

	return tile_id;
}

int g_MurBas(int x, int y, int w, int h, int tile_id)
{
	int		position;
	int		brique;
	int		nTiles;
	int		x_test;
	int		index;
	int		w2;
	int		tile_val;

	w2 = w - 1;
	nTiles = (int)(w2 / (TILE_X+1)) + 2;

	x += (int)gOffset_X;
	y += (int)gOffset_Y;

	y += h;

	for(index=0 ; index<nTiles ; index++)
	{
		x_test = x + index*TILE_X;
		if(x_test > x + w2)
			x_test = x + w2 - 1;

		position = (x_test/TILE_X) + (y/TILE_Y)*gHeader.level_w;
		if(position < 0 || position >= gHeader.level_w*gHeader.level_h)
			return 0;

		brique = gLevel[position].blocID - 48;
		
		if(tile_id == MUR_ID)
		{
			tile_val = g_IsMur(brique);
			if(tile_val)
				return tile_val;
		}
		if(brique == tile_id)
			return tile_id;
	}

	return 0;
}

void g_FixeDroite(float *x, int w)
{
	int		offset;

	offset = ( (int)*x + w + (int)gOffset_X ) % TILE_X;
//	if(offset < (TILE_X/2))
	if(offset < 32)
		*x -= offset;
}

void g_FixeGauche(float *x, int sw)
{
	int		offset;

	offset = ( (int)*x + sw + (int)gOffset_X ) % TILE_X;
//	if(offset > (TILE_X/2))
	if(offset > 16)
		*x += (TILE_X - offset);
}

void g_FixeHaut(float *y, int sh)
{
	int		offset;

	offset = ( (int)*y + sh + (int)gOffset_Y ) % TILE_Y;
//	if(offset > (TILE_Y/2))
	if(offset > 32)
		*y += (TILE_Y - offset);
}

BOOL g_FixeBas(float *y, int h)
{
	int		offset;

	offset = ( (int)*y + h + (int)gOffset_Y ) % TILE_Y;
	if(offset < 16)
//	if(offset < (TILE_Y/2))
	{
		*y -= offset;
		return true;
	}
	return false;
}

int g_MordTile(int x, int y)
{
	int		position;

	x += (int)gOffset_X;
	y += (int)gOffset_Y;

	position = (x/TILE_X) + (y/TILE_Y)*gHeader.level_w;
	if(position < 0 || position >= gHeader.level_w*gHeader.level_h)
		return -1;
	return gLevel[position].blocID - 48;
}

BOOL g_MordTile_2(int x, int y, int w, int h, int tx, int ty, int tw, int th)
{
	int		mid1_x;
	int		mid1_y;
	int		mid2_x;
	int		mid2_y;
	int		X;
	int		Y;
	float	dist;
	
	mid1_x = x + w/2;
	mid1_y = y + h/2;

	mid2_x = tx + tw/2;
	mid2_y = ty + th/2;

	X = mid2_x - mid1_x;
	Y = mid2_y - mid1_y;

	dist = (float) sqrt((float)(X*X + Y*Y));
	
	if(dist <= 35) // 10
		return TRUE;

	return FALSE;
}

BOOL g_Collision_2Tiles(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
	if(x1+w1 < x2)
		return FALSE;
	if(x1 > x2+w2)
		return FALSE;

	if(y1+h1 < y2)
		return FALSE;
	if(y1 > y2+h2)
		return FALSE;

	return TRUE;
}

int g_IsMur(int tile_id)
{
	if(tile_id < 0 || tile_id >= MAX_TILES)
		return 0;

	if(gBlocDefinition[tile_id] == MUR_ID)
		return MUR_ID;
	else if(gBlocDefinition[tile_id] == SMUR_ID)
		return SMUR_ID;
	return 0;
}

