
#ifndef MONSTER_INCLUDED
#define MONSTER_INCLUDED


void monster_CheckPos();
void monster_Update();
void monster_UpdateAbeille(pEnt AbeilleEnt);
void monster_UpdateGhost(pEnt AbeilleEnt);
void monster_UpdateSpider(pEnt SpiderEnt);
void monster_UpdateSanglier(pEnt SanglierEnt);
void monster_UpdateChenille(pEnt ChenilleEnt);
void monster_UpdateMBleu(pEnt MBleuEnt);
void monster_BeingHit(pEnt Entity);


#endif // MONSTER_INCLUDED


