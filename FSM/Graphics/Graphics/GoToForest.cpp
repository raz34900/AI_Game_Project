#include "GoToForest.h"

void GoToForest::OnEnter(NPC* pn)
{
    // Do nothing, act as an Idle state
    pn->setIsmoving(false);
}

void GoToForest::Transition(NPC* pn)
{
    // Removed the transition to GoToWarehouse to completely stop the infinite loop ping-pong!
}

void GoToForest::OnExit(NPC* pn)
{
    pn->setIsmoving(false);
}