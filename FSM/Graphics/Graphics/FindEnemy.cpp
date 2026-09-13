#include "FindEnemy.h"

void FindEnemy::OnEnter(NPC* pn) { pn->setIsmoving(true); }
void FindEnemy::Transition(NPC* pn) { OnExit(pn); }
void FindEnemy::OnExit(NPC* pn) { pn->setIsmoving(false); }