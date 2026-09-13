#include "Flee.h"

void Flee::OnEnter(NPC* pn) { pn->setIsmoving(true); }
void Flee::Transition(NPC* pn) { OnExit(pn); }
void Flee::OnExit(NPC* pn) { pn->setIsmoving(false); }