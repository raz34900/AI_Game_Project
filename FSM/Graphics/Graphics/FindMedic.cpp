#include "FindMedic.h"

void FindMedic::OnEnter(NPC* pn)
{
    // Implementation will be handled dynamically in MakeDecision for finding the specific medic target
    pn->setIsmoving(true);
}

void FindMedic::Transition(NPC* pn)
{
    OnExit(pn);
    // Decision to switch state will be handled by MakeDecision in NPC class
}

void FindMedic::OnExit(NPC* pn)
{
    pn->setIsmoving(false);
}