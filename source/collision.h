
#ifndef COLLISION_INC__
#define COLLISION_INC__



int		g_MurDroite(int x, int y, int w, int h);
int		g_MurGauche(int x, int y, int w, int h);
int		g_MurHaut(int x, int y, int w, int h, int tile_id);
int		g_MurBas(int x, int y, int w, int h, int tile_id);
void	g_FixeDroite(float *x, int w);
void	g_FixeGauche(float *x, int sw);
void	g_FixeHaut(float *y, int sh);
BOOL	g_FixeBas(float *y, int h);
int		g_MordTile(int x, int y);
BOOL	g_MordTile_2(int x, int y, int w, int h, int tx, int ty, int tw, int th);
BOOL	g_Collision_2Tiles(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
int		g_IsMur(int tile_id);


#endif // COLLISION_INC__