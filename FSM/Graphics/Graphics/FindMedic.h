#pragma once
#include "State.h"
#include "NPC.h"
#include <vector>

class FindMedic : public State
{
public:
    void OnEnter(NPC* pn) override;
    void Transition(NPC* pn) override;
    void OnExit(NPC* pn) override;
};