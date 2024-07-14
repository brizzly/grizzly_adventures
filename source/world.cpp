

#include "game.h"
#include "sprite.h"
#include "directx.h"
#include "world.h"

extern	KGraphic *background, *monsters, *paralax1;

float	gEffet_EarthQuake;


bool	gIsDWscroll = false;
bool	gIsUPscroll = false;
bool	gIsLFscroll = false;
bool	gIsRGscroll = false;
int		gScrollY_BG = 0;



extern	float	scroll;
extern	float	scroll2;
extern	float	scroll3;
extern	float	scroll_Y;
extern	float	scroll2_Y;
extern	float	scroll3_Y;
extern	int		gOffset_X;
extern	int		gOffset_Y;
extern	float	gOffset_Xf;
extern	float	gOffset_Yf;
extern	pEnt	gEntityList;
extern	sHeaderGrizzly	gHeader;
extern	float	gFrameTime_msec;
extern	bool	gEndLevelHit;
extern	bool	gPlayerRun;
extern	int		gEpisodeID;
extern	int		ProgramState;
extern	bool	gPanelIsActive;
extern	short	gTransitionDir;


void w_DrawToper2(int x_offset, int y_offset, int theme)
{
	int	topper_y[3] = {513, 614, 715};

	if(y_offset > 100)
		return;

	x_offset = x_offset % ScreenX;

	// affiche le bitmap du haut
	blit(monsters, x_offset, 0+topper_y[theme-1], 0, -y_offset, (int)(ScreenX-x_offset), 100, true);
	blit(monsters, 0, 0+topper_y[theme-1], (int)(ScreenX-x_offset), -y_offset, x_offset, 100, true);

}

void w_DrawToper(int x_offset, int y_offset)
{
	w_DrawToper2(x_offset, y_offset, gEpisodeID);
}

void w_Draw2Paralax()
{
	int		m_scroll;
	int		h_scroll;
	int		p_scroll;
	int		m2_scroll;
	int		h2_scroll;
	int		p2_scroll;

	m_scroll = 0;
	h_scroll = (int)scroll;
	p_scroll = (int)scroll2;

	m2_scroll = 0;
	h2_scroll = (int)scroll_Y;
	p2_scroll = (int)scroll2_Y;

	if(gEpisodeID == 1 || gEpisodeID == 2)
	if(gTransitionDir == 1 || gEndLevelHit)
	{
		gScrollY_BG = (int)((float)h2_scroll / 1.5f) + 20;
		if(gScrollY_BG < 0)
			gScrollY_BG = 0;
		if(gScrollY_BG > 40)
			gScrollY_BG = 40;
	}

	if(gEpisodeID == 3)
	if(gTransitionDir == 1 || gEndLevelHit)
	{
		p2_scroll %= ScreenY;
	}

	// affiche la paralaxe 1
//	blit(paralax2, h_scroll, 0, 0, h2_scroll, (int)(ScreenX-h_scroll), ScreenY, true);
//	blit(paralax2, 0, 0,(int)(ScreenX-h_scroll), h2_scroll, h_scroll, ScreenY, true);
	blit(paralax1, h_scroll, 0+480, 0, h2_scroll, (int)(ScreenX-h_scroll), ScreenY, true);
	blit(paralax1, 0, 0+480,(int)(ScreenX-h_scroll), h2_scroll, h_scroll, ScreenY, true);

	// affiche la paralaxe 2
	blit(paralax1, p_scroll, 0, 0, p2_scroll, (int)(ScreenX-p_scroll), ScreenY-p2_scroll, true);
	blit(paralax1, 0, 0, (int)(ScreenX-p_scroll), p2_scroll, p_scroll, ScreenY-p2_scroll, true);

	if(gEpisodeID == 3)
	if(gTransitionDir == 1 || gEndLevelHit)
	{
		// hack : pour boucler la paralaxe du world 3
		blit(paralax1, p_scroll, ScreenY-p2_scroll, 0, 0, (int)(ScreenX-p_scroll), p2_scroll, true);
		blit(paralax1, 0, ScreenY-p2_scroll, (int)(ScreenX-p_scroll), 0, p_scroll, p2_scroll, true);
	}
}

void w_DrawBackground()
{
	if((gEpisodeID == 1 || gEpisodeID == 2) && (gTransitionDir == 1 || gEndLevelHit))
	{
		blit(background, 0, 40-gScrollY_BG, 0, 0, ScreenX, ScreenY, false);
	}
	else
	{
		blit(background, 0, 0, 0, 0, ScreenX, ScreenY, false);
	}
}

void w_DrawWorld()
{
	w_DrawBackground();
	w_Draw2Paralax();
}


// ==============================================
//
//		SCROLLINGS
//
// ==============================================

void g_Scroll_Left(float scroll_step)
{
	pEnt	Ent;
	float	offset;
	int		i_offset;

	if(gPlayerRun) scroll_step *= 1.5f;

	if(gOffset_X > 0)
	{
		offset = scroll_step;
		gOffset_Xf -= offset;
		if(gOffset_Xf >= 0)
			return;
		i_offset = (int)(-gOffset_Xf+1.0f);
		gOffset_X -= i_offset;
		gOffset_Xf += (float)i_offset;

		if(gOffset_X > 0)
		{
			scroll -= offset * 0.3f;
			if(scroll < 0)
				scroll += ScreenX;

			scroll2 -= offset * 0.5f;
			if(scroll2 < 0)
				scroll2 += ScreenX;

			scroll3 -= offset * 0.7f;
			if(scroll3 < 0)
				scroll3 += ScreenX;
		}
		else
		{
			i_offset += gOffset_X;
			gOffset_X = 0;
		}

		for(Ent=gEntityList ; Ent ; Ent=Ent->Next)
		{
			if(Ent->sprite_id == ENT_POMME ||
				Ent->sprite_id == ENT_KEY ||
				Ent->sprite_id == ENT_LIFEUP ||
				Ent->sprite_id == ENT_BANANE)
			if(Ent->ItemCatched)
				continue;
			Ent->x += i_offset;
		}
	}
}

void g_Scroll_Right(float scroll_step)
{
	pEnt	Ent;
	float	offset;
	int		i_offset;
	int		scroll_xmax = TILE_X*gHeader.level_w - ScreenX;

	if(gPlayerRun) scroll_step *= 1.5f;

	if(gOffset_X < scroll_xmax)
	{
		offset = scroll_step;
		gOffset_Xf += offset;
		if(gOffset_Xf < 1.0f)
			return;
		i_offset = (int)gOffset_Xf;
		gOffset_X += i_offset;
		gOffset_Xf -= (float)i_offset;

		if(gOffset_X < scroll_xmax)
		{
			scroll += scroll_step * 0.3f;
			if(scroll > ScreenX)
				scroll -= ScreenX;

			scroll2 += scroll_step * 0.5f;
			if(scroll2 > ScreenX)
				scroll2 -= ScreenX;

			scroll3 += scroll_step * 0.7f;
			if(scroll3 > ScreenX)
				scroll3 -= ScreenX;
		}
		else
		{
			i_offset -= (gOffset_X - scroll_xmax);
			gOffset_X = scroll_xmax;
		}

		for(Ent=gEntityList ; Ent ; Ent=Ent->Next)
		{
			if(Ent->sprite_id == ENT_POMME ||
				Ent->sprite_id == ENT_KEY ||
				Ent->sprite_id == ENT_LIFEUP ||
				Ent->sprite_id == ENT_BANANE)
			if(Ent->ItemCatched)
				continue;
			Ent->x -= i_offset;
		}
	}
}

void g_Scroll_Up(float scroll_step)
{
	pEnt	Ent;
	int		i_offset;

	if(gOffset_Y > 0)
	{
		gOffset_Yf -= scroll_step;
		if(gOffset_Yf >= 0)
			return;
		i_offset = (int)(-gOffset_Yf+1.0f);
		gOffset_Y -= i_offset;
		gOffset_Yf += (float)i_offset;

		if(gOffset_Y > 0)
		{
			scroll_Y += 0.3f * i_offset; //gFrameTime_msec * SCROLL_SPEED * 0.3f;
			scroll2_Y += 0.6f * i_offset; //gFrameTime_msec * SCROLL_SPEED * 0.5f;
//			scroll3_Y += gFrameTime_msec * SCROLL_SPEED * 0.7f;

		//	scroll -= gFrameTime_msec * SCROLL_SPEED * 0.5f;
		//	if(scroll < 0)
		//		scroll += ScreenX;
		}
		else
		{
			i_offset += gOffset_Y;
			gOffset_Y = 0;
		}

		for(Ent=gEntityList ; Ent ; Ent=Ent->Next)
		{
			if(Ent->sprite_id == ENT_POMME ||
				Ent->sprite_id == ENT_KEY ||
				Ent->sprite_id == ENT_LIFEUP ||
				Ent->sprite_id == ENT_BANANE)
			if(Ent->ItemCatched)
				continue;
			Ent->y += i_offset;
		}
	}
	else
		gIsUPscroll = false;
}

void g_Scroll_Down(float scroll_step)
{
	pEnt	Ent;
	int		i_offset;
	int		scroll_ymax = TILE_Y*gHeader.level_h - ScreenY;

	if(gOffset_Y < scroll_ymax)
	{
		gOffset_Yf += scroll_step;
		if(gOffset_Yf < 1.0f)
			return;
		i_offset = (int)gOffset_Yf;
		gOffset_Y += i_offset;
		gOffset_Yf -= (float)i_offset;

		if(gOffset_Y < scroll_ymax)
		{
			scroll_Y -= 0.3f * i_offset; //gFrameTime_msec * SCROLL_SPEED * 0.3f;
			if(scroll_Y < 0)
				scroll_Y = 0;

			scroll2_Y -= 0.6f * i_offset; //gFrameTime_msec * SCROLL_SPEED * 0.5f;
			if(scroll2_Y < 0)
				scroll2_Y = 0;

//			scroll3_Y -= gFrameTime_msec * SCROLL_SPEED * 0.7f;
//			if(scroll3_Y < 0)
//				scroll3_Y = 0;

		//	scroll += gFrameTime_msec * SCROLL_SPEED * 0.5f;
		//	if(scroll > ScreenX)
		//		scroll -= ScreenX;
		}
		else
		{
			i_offset -= (gOffset_Y - scroll_ymax);
			gOffset_Y = scroll_ymax;
		}

		for(Ent=gEntityList ; Ent ; Ent=Ent->Next)
		{
			if(Ent->sprite_id == ENT_POMME ||
				Ent->sprite_id == ENT_KEY ||
				Ent->sprite_id == ENT_LIFEUP ||
				Ent->sprite_id == ENT_BANANE)
			if(Ent->ItemCatched)
				continue;
			Ent->y -= i_offset;
		}
	}
	else
		gIsDWscroll = false;
}


void w_Effect1()
{
	 gEffet_EarthQuake = EARTHQUAKE_T;
}

void w_Effect2()
{
} 