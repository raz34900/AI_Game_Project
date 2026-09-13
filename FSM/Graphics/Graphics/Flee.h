#pragma once
#include "State.h"
#include "NPC.h"

class Flee : public State
{
public:
    void OnEnter(NPC* pn) override;
    void Transition(NPC* pn) override;
    void OnExit(NPC* pn) override;
};