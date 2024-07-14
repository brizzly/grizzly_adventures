
#ifndef PLAYER_INC__
#define PLAYER_INC__


#include "sprite.h"


void player_Update(DWORD input);
bool player_MoveLeft(float MoveOffset, pEnt PlayerEnt);
bool player_MoveRight(float MoveOffset, pEnt PlayerEnt);
bool player_MoveUp(float MoveOffset, pEnt PlayerEnt);
bool player_MoveDown(float MoveOffset, pEnt PlayerEnt);
void player_DrawCircleStarsPos(int x, int y, float *sinus_x);
void player_DrawCircleStars();
void player_MakeInvinsible();
float player_CadreY(pEnt PlayerEnt, float ScrollSpeed);
bool player_AutoCadreX(pEnt PlayerEnt, DWORD input);
bool player_AutoCadreY(pEnt PlayerEnt);
void player_MakeSuperBumpers();



#endif // PLAYER_INC__


