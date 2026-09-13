#include "FindSupply.h"

void FindSupply::OnEnter(NPC* pn) { pn->setIsmoving(true); }
void FindSupply::Transition(NPC* pn) { OnExit(pn); }
void FindSupply::OnExit(NPC* pn) { pn->setIsmoving(false); }