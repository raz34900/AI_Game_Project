#pragma once
#include "State.h"
#include "NPC.h"

class FindEnemy : public State
{
public:
    void OnEnter(NPC* pn) override;
    void Transition(NPC* pn) override;
    void OnExit(NPC* pn) override;
};