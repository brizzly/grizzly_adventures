#ifndef WORLD_INC__
#define WORLD_INC__


#define	SCROLL_SPEED	0.20f
#define	SCROLL_SLOWSPD	0.060f //0.02f

void g_Scroll_Left(float scroll_step);
void g_Scroll_Right(float scroll_step);
void g_Scroll_Up(float scroll_step);
void g_Scroll_Down(float scroll_step);
void w_DrawToper2(int x_offset, int y_offset, int theme);
void w_DrawToper(int x_offset, int y_offset);
void w_Draw2Paralax();
void w_DrawBackground();
void w_DrawWorld();
void w_Effect1();
void w_Effect2();




#endif WORLD_INC__