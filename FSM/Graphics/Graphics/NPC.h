#pragma once
#include "State.h"
#include "Maze.h"
#include <vector>
#include <utility>

const double SPEED = 0.35;

enum Team { TEAM_A, TEAM_B };
enum Role { FIGHTER, MEDIC, SUPPLY };

class NPC
{
private:
    double x, y, targetX, targetY;
    bool isMoving;
    State* pCurrentState;
    Maze* myMaze;
    int shieldTimer; // NEW: Timer for the camouflage shield

    Team team;
    Role role;
    int hp;
    int ammo;
    double aggression;

    int supplyPayload;
    int mines; // Number of landmines this character holds

    int attackCooldown;
    int pathRefreshTimer;
    std::vector<std::pair<int, int>> path;

public:
    NPC(double startX, double startY, Team t, Role r, Maze* m);
    ~NPC();

    void show();
    void setIsmoving(bool value) { isMoving = value; }
    bool calculatePathAndSetTarget(int tx, int ty);
    void setCurrentState(State* ps);
    void DoSomeWork();
    void MakeDecision(const std::vector<NPC*>& all_npcs);

    bool hasLineOfSight(int targetX, int targetY);
    void Attack(NPC* target, const std::vector<NPC*>& all_npcs);

    double getX() const { return x; }
    double getY() const { return y; }
    int getHP() const { return hp; }
    int getAmmo() const { return ammo; }
    Team getTeam() const { return team; }
    Role getRole() const { return role; }
    double getAggression() const { return aggression; }
    State* getCurrentState() const { return pCurrentState; }

    void takeDamage(int amount);
    void useAmmo(int amount);
    void heal(int amount);
    void reload(int amount);
};