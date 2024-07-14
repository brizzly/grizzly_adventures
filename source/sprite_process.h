
#ifndef SPRITEPROCESS_INC__
#define SPRITEPROCESS_INC__


void sprite_RandomizeRotation(pEnt Ent);
void sprite_Rotate(pEnt Ent, float amplitude, float speed, float *x_move, float *y_move);

void g_AbeilleProcess(pEnt MonsterEnt);
void g_RLiftProcess(pEnt RLiftProcess);
void g_HLiftProcess(pEnt HLiftProcess);
void g_FeuilleProcess(pEnt FeuilleEnt);
void g_Collision_HLift(pEnt PlayerEnt, pEnt CopyEnt);
void g_Collision_Bumper(pEnt PlayerEnt, pEnt CopyEnt);
void g_GetVelocityDir(float *velx, float *vely, float posx, float posy, float destx, float desty);
bool g_Collision_Pomme(pEnt PlayerEnt, pEnt CopyEnt);
bool g_Collision_PommeV(pEnt PlayerEnt, pEnt CopyEnt);
bool g_Collision_PommeJ(pEnt PlayerEnt, pEnt CopyEnt);
bool g_Collision_Banane(pEnt PlayerEnt, pEnt CopyEnt);
bool g_Collision_Vie(pEnt PlayerEnt, pEnt CopyEnt);
bool g_Collision_Key(pEnt PlayerEnt, pEnt CopyEnt);
bool g_Collision_Diamant(pEnt PlayerEnt, pEnt CopyEnt);
void g_UpdatePlayer_ItemCollision(pEnt PlayerEnt);
void g_PangProcess();
void g_SmokeProcess();


#endif // SPRITEPROCESS_INC__
