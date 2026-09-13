#include "GoToWarehouse.h"

void GoToWarehouse::OnEnter(NPC* pn)
{
    // Do nothing, act as an Idle state
    pn->setIsmoving(false);
}

void GoToWarehouse::Transition(NPC* pn)
{
    // Removed the transition to GoToForest to completely stop the infinite loop ping-pong!
}

void GoToWarehouse::OnExit(NPC* pn)
{
    pn->setIsmoving(false);
}