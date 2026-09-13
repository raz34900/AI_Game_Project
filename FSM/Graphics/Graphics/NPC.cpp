#include <math.h>
#include <stdlib.h>
#include <algorithm> 
#include "NPC.h"
#include "glut.h"
#include "PathPlanner.h"

#include "GoToForest.h"
#include "GoToWarehouse.h"
#include "FindMedic.h"
#include "FindEnemy.h"
#include "FindSupply.h"
#include "Flee.h"

NPC::NPC(double startX, double startY, Team t, Role r, Maze* m)
    : x(startX), y(startY), team(t), role(r), myMaze(m), isMoving(false), attackCooldown(0), pathRefreshTimer(0)
{
    if (role == FIGHTER) {
        hp = 100;
        ammo = 50;
        supplyPayload = 0;
        mines = 1; // Each fighter gets 1 mine (Total 2 per team)
    }
    else {
        hp = 100;
        ammo = 0;
        supplyPayload = 1;
        mines = 0;
    }
    aggression = (double)rand() / RAND_MAX;
    shieldTimer = 0; // Initialize shield timer

    pCurrentState = new GoToForest();
    pCurrentState->OnEnter(this);
}

NPC::~NPC() {
    if (pCurrentState != nullptr) delete pCurrentState;
}

void NPC::show() {
    if (hp <= 0) return;

    // Body becomes Camouflage Green if shield is active!
    if (shieldTimer > 0) glColor3d(0.3, 0.5, 0.3); // Camo Green
    else if (team == TEAM_A) glColor3d(0, 0, 1);
    else glColor3d(1, 0, 0);

    glBegin(GL_POLYGON);
    glVertex2d(x - 1, y - 1); glVertex2d(x - 1, y + 1);
    glVertex2d(x + 1, y + 1); glVertex2d(x + 1, y - 1);
    glEnd();

    // Head color
    if (role == MEDIC) glColor3d(1, 1, 1);
    else if (role == SUPPLY) glColor3d(1, 1, 0);
    else glColor3d(0.8, 0.6, 0.4);

    glBegin(GL_POLYGON);
    glVertex2d(x - 0.5, y + 1); glVertex2d(x - 0.5, y + 2);
    glVertex2d(x + 0.5, y + 2); glVertex2d(x + 0.5, y + 1);
    glEnd();

    glLineWidth(1.0); // Ensure thin lines
    glColor3d(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2d(x - 0.5, y + 1); glVertex2d(x - 0.5, y + 2);
    glVertex2d(x + 0.5, y + 2); glVertex2d(x + 0.5, y + 1);
    glEnd();

    // Letter
    glColor3d(0, 0, 0);
    glRasterPos2d(x - 0.3, y - 0.3);
    char label = (role == FIGHTER ? 'F' : (role == MEDIC ? 'M' : 'S'));
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, label);

    // ==========================================
    // UI: HEALTH BAR
    // ==========================================

    // 1. Draw dark background for health to make it pop
    glColor3d(0.2, 0.2, 0.2);
    glBegin(GL_POLYGON);
    glVertex2d(x - 1.5, y + 2.5); glVertex2d(x + 1.5, y + 2.5);
    glVertex2d(x + 1.5, y + 2.9); glVertex2d(x - 1.5, y + 2.9);
    glEnd();

    // 2. Draw Green Health
    glColor3d(0.0, 0.8, 0.0);
    double hpRatio = (hp / 100.0);
    glBegin(GL_POLYGON);
    glVertex2d(x - 1.5, y + 2.5);
    glVertex2d(x - 1.5 + hpRatio * 3.0, y + 2.5);
    glVertex2d(x - 1.5 + hpRatio * 3.0, y + 2.9);
    glVertex2d(x - 1.5, y + 2.9);
    glEnd();

    // 3. Very thin black outline
    glLineWidth(1.0);
    glColor3d(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2d(x - 1.5, y + 2.5); glVertex2d(x + 1.5, y + 2.5);
    glVertex2d(x + 1.5, y + 2.9); glVertex2d(x - 1.5, y + 2.9);
    glEnd();

    // 'H' Label
    glRasterPos2d(x - 2.5, y + 2.5);
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'H');

    // ==========================================
    // UI: AMMO BAR (Fighters Only)
    // ==========================================
    if (role == FIGHTER) {

        // 1. Draw dark background for ammo
        glColor3d(0.2, 0.2, 0.2);
        glBegin(GL_POLYGON);
        glVertex2d(x - 1.5, y + 3.2); glVertex2d(x + 1.5, y + 3.2);
        glVertex2d(x + 1.5, y + 3.6); glVertex2d(x - 1.5, y + 3.6);
        glEnd();

        // 2. Draw Orange Ammo
        glColor3d(1.0, 0.5, 0.0);
        double ammoRatio = ((double)ammo / 50.0);
        if (ammoRatio > 1.0) ammoRatio = 1.0; // Extra safety guard for visuals

        glBegin(GL_POLYGON);
        glVertex2d(x - 1.5, y + 3.2);
        glVertex2d(x - 1.5 + ammoRatio * 3.0, y + 3.2);
        glVertex2d(x - 1.5 + ammoRatio * 3.0, y + 3.6);
        glVertex2d(x - 1.5, y + 3.6);
        glEnd();

        // 3. Very thin black outline
        glLineWidth(1.0);
        glColor3d(0, 0, 0);
        glBegin(GL_LINE_LOOP);
        glVertex2d(x - 1.5, y + 3.2); glVertex2d(x + 1.5, y + 3.2);
        glVertex2d(x + 1.5, y + 3.6); glVertex2d(x - 1.5, y + 3.6);
        glEnd();

        // 'S' Label
        glRasterPos2d(x - 2.5, y + 3.2);
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'S');
    }
}

bool NPC::calculatePathAndSetTarget(int tx, int ty) {
    targetX = tx; targetY = ty;
    path = PathPlanner::FindPath(myMaze, (int)x, (int)y, tx, ty, (int)team);
    if (!path.empty()) { isMoving = true; return true; }
    isMoving = false; return false;
}

void NPC::setCurrentState(State* ps) {
    if (pCurrentState != nullptr) delete pCurrentState;
    pCurrentState = ps;
}

void NPC::DoSomeWork() {
    if (attackCooldown > 0) attackCooldown--;
    if (shieldTimer > 0) shieldTimer--; // Count down shield timer

    if (isMoving && !path.empty()) {
        int nextX = path.back().first; int nextY = path.back().second;
        double dx = nextX - x; double dy = nextY - y;
        double dist = sqrt(dx * dx + dy * dy);

        double currentSpeed = SPEED;
        if (myMaze->getCell((int)x, (int)y) == WATER) {
            currentSpeed = SPEED * 0.4;
        }

        if (dist > currentSpeed) {
            x += currentSpeed * (dx / dist); y += currentSpeed * (dy / dist);
        }
        else {
            x = nextX; y = nextY;
            path.pop_back();
            if (path.empty()) {
                isMoving = false;
                pCurrentState->Transition(this);
            }
        }

        // CHECK MINE PROXIMITY EVERY STEP
        if (myMaze->checkAndTriggerMine(x, y, team)) {
            takeDamage(20);
            myMaze->addBlood(x, y);
        }

        // Check if Fighter stepped on SHIELD House
        if (role == FIGHTER && myMaze->getCell((int)x, (int)y) == SHIELD) {
            shieldTimer = 330; // ~10 seconds at 33 FPS

            // Delete the 2x2 building block from the map
            for (int dx = -2; dx <= 2; dx++) {
                for (int dy = -2; dy <= 2; dy++) {
                    if (myMaze->getCell((int)x + dx, (int)y + dy) == SHIELD) {
                        myMaze->setCell((int)x + dx, (int)y + dy, SPACE);
                    }
                }
            }
        }
    }
}

bool NPC::hasLineOfSight(int targetX, int targetY) {
    double dx = targetX - x; double dy = targetY - y;
    double steps = std::max(abs(dx), abs(dy));
    if (steps == 0) return true;

    double xInc = dx / steps; double yInc = dy / steps;
    double cx = x; double cy = y;

    for (int i = 0; i < steps; i++) {
        int cellType = myMaze->getCell((int)cx, (int)cy);
        if (cellType == WALL || cellType == TREE || cellType == ROCK) return false;
        cx += xInc; cy += yInc;
    }
    return true;
}

void NPC::Attack(NPC* target, const std::vector<NPC*>& all_npcs) {
    if (attackCooldown > 0 || ammo <= 0 || target->getHP() <= 0) return;

    bool useGrenade = (rand() % 100 < 20) && (ammo >= 5);

    if (useGrenade) {
        useAmmo(5);
        attackCooldown = 50;
        myMaze->addAnimation(x, y, target->getX(), target->getY(), 1);

        for (NPC* other : all_npcs) {
            if (other->getTeam() != this->team && other->getHP() > 0) {
                double distToGrenade = sqrt(pow(target->getX() - other->getX(), 2) + pow(target->getY() - other->getY(), 2));
                if (distToGrenade <= 5.0) {
                    other->takeDamage(8);
                    myMaze->addBlood(other->getX(), other->getY());
                }
            }
        }
    }
    else {
        useAmmo(1);
        attackCooldown = 15;
        target->takeDamage(2);
        myMaze->addBlood(target->getX(), target->getY());
        myMaze->addAnimation(x, y, target->getX(), target->getY(), 0);
    }
}

void NPC::MakeDecision(const std::vector<NPC*>& all_npcs) {
    if (hp <= 0) return;
    if (pathRefreshTimer > 0) pathRefreshTimer--;

    double critical_hp = 50.0 + (1.0 - aggression) * 30.0;
    double critical_ammo = 15.0 + (1.0 - aggression) * 10.0;

    // --- FIGHTER LOGIC ---
    if (role == FIGHTER) {

        // STRATEGIC MINE PLACEMENT
        if (mines > 0 && isMoving && (rand() % 40 == 0)) {
            int cx = (int)x, cy = (int)y;
            bool isChokePoint = (myMaze->getCell(cx - 1, cy) == WALL && myMaze->getCell(cx + 1, cy) == WALL) ||
                (myMaze->getCell(cx, cy - 1) == WALL && myMaze->getCell(cx, cy + 1) == WALL);

            bool isNearItem = false;
            for (int dx = -2; dx <= 2; dx++) {
                for (int dy = -2; dy <= 2; dy++) {
                    int c = myMaze->getCell(cx + dx, cy + dy);
                    if (c == HEALTH || c == AMMO || c == SHIELD) isNearItem = true;
                }
            }

            if (isChokePoint || isNearItem || (rand() % 200 == 0)) {
                myMaze->placeMine(x, y, team);
                mines--;
            }
        }

        if (hp < critical_hp) {
            NPC* myMedic = nullptr;
            for (NPC* other : all_npcs) {
                if (other->getTeam() == this->team && other->getRole() == MEDIC && other->getHP() > 0) {
                    myMedic = other; break;
                }
            }
            if (myMedic) {
                if (dynamic_cast<FindMedic*>(pCurrentState) == nullptr) setCurrentState(new FindMedic());
                if (pathRefreshTimer <= 0) {
                    calculatePathAndSetTarget((int)myMedic->getX(), (int)myMedic->getY());
                    pathRefreshTimer = 15;
                }
                return;
            }
        }

        if (ammo < critical_ammo) {
            NPC* mySupply = nullptr;
            for (NPC* other : all_npcs) {
                if (other->getTeam() == this->team && other->getRole() == SUPPLY && other->getHP() > 0) {
                    mySupply = other; break;
                }
            }
            if (mySupply) {
                if (dynamic_cast<FindSupply*>(pCurrentState) == nullptr) setCurrentState(new FindSupply());
                if (pathRefreshTimer <= 0) {
                    calculatePathAndSetTarget((int)mySupply->getX(), (int)mySupply->getY());
                    pathRefreshTimer = 15;
                }
                return;
            }
        }

        // ACTIVE A* SEARCH FOR SHIELD HOUSE
        bool headingToShield = false;
        if (shieldTimer <= 0 && pathRefreshTimer <= 0) {
            int bestSX = -1, bestSY = -1;
            double bestSDist = 9999;
            for (int i = 0; i < MAZE_SIZE; i++) {
                for (int j = 0; j < MAZE_SIZE; j++) {
                    if (myMaze->getCell(i, j) == SHIELD) {
                        double d = sqrt(pow(x - i, 2) + pow(y - j, 2));
                        if (d < bestSDist) { bestSDist = d; bestSX = i; bestSY = j; }
                    }
                }
            }
            if (bestSX != -1 && bestSDist < 30.0) {
                calculatePathAndSetTarget(bestSX, bestSY);
                pathRefreshTimer = 30;
                headingToShield = true;
            }
        }

        if (dynamic_cast<FindEnemy*>(pCurrentState) == nullptr) setCurrentState(new FindEnemy());

        double min_dist = 999999;
        NPC* target_enemy = nullptr;
        for (NPC* other : all_npcs) {
            if (other->getTeam() != this->team && other->getHP() > 0) {
                double dist = sqrt(pow(x - other->getX(), 2) + pow(y - other->getY(), 2));
                if (dist < min_dist) { min_dist = dist; target_enemy = other; }
            }
        }

        if (target_enemy != nullptr) {
            if (min_dist < 15.0 && hasLineOfSight(target_enemy->getX(), target_enemy->getY())) {
                Attack(target_enemy, all_npcs);
                path.clear(); isMoving = false;
            }
            else if (!headingToShield && pathRefreshTimer <= 0) {
                calculatePathAndSetTarget((int)target_enemy->getX(), (int)target_enemy->getY());
                pathRefreshTimer = 20;
            }
        }
    }

    // --- SUPPORT LOGIC (Medic & Supply) ---
    if (role == MEDIC || role == SUPPLY) {
        if (supplyPayload <= 0) {
            int buildingType = (role == MEDIC) ? HEALTH : AMMO;

            if (role == MEDIC && dynamic_cast<GoToForest*>(pCurrentState) == nullptr) setCurrentState(new GoToForest());
            if (role == SUPPLY && dynamic_cast<GoToWarehouse*>(pCurrentState) == nullptr) setCurrentState(new GoToWarehouse());

            int bestX = -1, bestY = -1;
            double bestDist = 9999;
            for (int i = 0; i < MAZE_SIZE; i++) {
                for (int j = 0; j < MAZE_SIZE; j++) {
                    if (myMaze->getCell(i, j) == buildingType) {
                        double d = sqrt(pow(x - i, 2) + pow(y - j, 2));
                        if (d < bestDist) { bestDist = d; bestX = i; bestY = j; }
                    }
                }
            }

            if (bestX != -1) {
                if (bestDist < 3.0) {
                    supplyPayload = 1;
                    path.clear(); isMoving = false;
                }
                else if (pathRefreshTimer <= 0) {
                    calculatePathAndSetTarget(bestX, bestY);
                    pathRefreshTimer = 30;
                }
            }
            return;
        }

        bool someoneNeedsHelp = false;
        NPC* targetFighter = nullptr;
        double min_dist_to_fighter = 9999;

        for (NPC* other : all_npcs) {
            if (other->getTeam() == team && other->getRole() == FIGHTER && other->getHP() > 0) {
                if ((role == MEDIC && other->getHP() < 100) || (role == SUPPLY && other->getAmmo() < 50)) {
                    double d = sqrt(pow(x - other->getX(), 2) + pow(y - other->getY(), 2));
                    if (d < min_dist_to_fighter) {
                        min_dist_to_fighter = d;
                        targetFighter = other;
                        someoneNeedsHelp = true;
                    }
                }
            }
        }

        if (someoneNeedsHelp) {
            if (min_dist_to_fighter <= 4.0 && attackCooldown <= 0) {
                if (role == MEDIC) targetFighter->heal(100);
                if (role == SUPPLY) targetFighter->reload(50);

                supplyPayload = 0;
                attackCooldown = 50;
                path.clear(); isMoving = false;
            }
            else if (pathRefreshTimer <= 0) {
                calculatePathAndSetTarget((int)targetFighter->getX(), (int)targetFighter->getY());
                pathRefreshTimer = 20;
            }
        }
        else {
            if (pathRefreshTimer <= 0) {
                for (NPC* other : all_npcs) {
                    if (other->getTeam() == team && other->getRole() == FIGHTER && other->getHP() > 0) {
                        double distToFighter = sqrt(pow(x - other->getX(), 2) + pow(y - other->getY(), 2));
                        if (distToFighter > 5.0) calculatePathAndSetTarget((int)other->getX(), (int)other->getY());
                        else { path.clear(); isMoving = false; }
                        pathRefreshTimer = 30;
                        break;
                    }
                }
            }
        }
    }
}

void NPC::takeDamage(int amount) {
    if (shieldTimer > 0) return; // Invincible!
    hp -= amount;
    if (hp < 0) hp = 0;
}
void NPC::useAmmo(int amount) { ammo -= amount; if (ammo < 0) ammo = 0; }
void NPC::heal(int amount) { hp += amount; if (hp > 100) hp = 100; }

// FIXED: Added safety cap to ammo so it never exceeds 50 and breaks the visual bar
void NPC::reload(int amount) {
    ammo += amount;
    if (ammo > 50) ammo = 50;
}