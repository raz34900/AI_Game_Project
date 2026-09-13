#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <string> 
#include "glut.h"
#include "NPC.h"
#include "Maze.h"

// Global state variables
std::vector<NPC*> npcs;
Maze* maze = nullptr;
bool isGameOver = false;
std::string winnerMsg = "";

void init() {
    srand((unsigned int)time(0));
    glClearColor(0.5, 0.7, 0.5, 0); // Grass background
    glOrtho(0, 100, 0, 100, -1, 1);

    maze = new Maze();
    maze->init();

    // Team Blue (Team A)
    npcs.push_back(new NPC(10, 10, TEAM_A, FIGHTER, maze));
    npcs.push_back(new NPC(15, 10, TEAM_A, FIGHTER, maze));
    npcs.push_back(new NPC(10, 15, TEAM_A, MEDIC, maze));
    npcs.push_back(new NPC(15, 15, TEAM_A, SUPPLY, maze));

    // Team Red (Team B)
    npcs.push_back(new NPC(90, 90, TEAM_B, FIGHTER, maze));
    npcs.push_back(new NPC(85, 90, TEAM_B, FIGHTER, maze));
    npcs.push_back(new NPC(90, 85, TEAM_B, MEDIC, maze));
    npcs.push_back(new NPC(85, 85, TEAM_B, SUPPLY, maze));
}

// Draw the Heads-Up Display (Health and Ammo)
void drawHUD() {
    int blueY = 95, redY = 95;
    for (NPC* npc : npcs) {
        if (npc->getHP() > 0) {
            std::string info = (npc->getRole() == FIGHTER ? "F" : (npc->getRole() == MEDIC ? "M" : "S"));
            info += " HP:" + std::to_string((int)npc->getHP());
            if (npc->getRole() == FIGHTER) info += " Am:" + std::to_string(npc->getAmmo());

            if (npc->getTeam() == TEAM_A) { // Left side
                glColor3d(0, 0, 1); glRasterPos2d(2, blueY); blueY -= 4;
            }
            else { // Right side
                glColor3d(1, 0, 0); glRasterPos2d(78, redY); redY -= 4;
            }
            for (char c : info) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (maze) {
        maze->draw();
        maze->drawAnimations();
    }

    // Draw all active NPCs
    for (NPC* npc : npcs) {
        if (npc->getHP() > 0) npc->show();
    }

    drawHUD();

    // Display Game Over screen
    if (isGameOver) {
        // Wider white box to fit long texts
        glColor3d(1, 1, 1);
        glBegin(GL_POLYGON);
        glVertex2d(10, 45); glVertex2d(10, 55);
        glVertex2d(90, 55); glVertex2d(90, 45);
        glEnd();

        // Auto-center the text based on its length
        glColor3d(0, 0, 0);
        double startX = 50.0 - (winnerMsg.length() * 0.85);
        glRasterPos2d(startX, 49);

        for (char c : winnerMsg) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
    glutSwapBuffers();
}

void timer(int value) {
    if (!isGameOver) {
        int teamAF = 0, teamBF = 0;            // Count alive fighters
        int ammoA = 0, ammoB = 0;              // Count total ammo
        bool supplyA = false, supplyB = false; // Check if supply is alive
        int hpA = 0, hpB = 0;                  // Count total HP for tie-breakers

        std::vector<std::pair<int, int>> posA, posB;

        for (NPC* npc : npcs) {
            if (npc->getHP() > 0) {
                // Collect stats for Team A
                if (npc->getTeam() == TEAM_A) {
                    posA.push_back({ (int)npc->getX(), (int)npc->getY() });
                    hpA += npc->getHP();
                    if (npc->getRole() == FIGHTER) { teamAF++; ammoA += npc->getAmmo(); }
                    if (npc->getRole() == SUPPLY) supplyA = true;
                }
                // Collect stats for Team B
                else {
                    posB.push_back({ (int)npc->getX(), (int)npc->getY() });
                    hpB += npc->getHP();
                    if (npc->getRole() == FIGHTER) { teamBF++; ammoB += npc->getAmmo(); }
                    if (npc->getRole() == SUPPLY) supplyB = true;
                }

                // Run AI Logic
                npc->MakeDecision(npcs);
                npc->DoSomeWork();
            }
        }

        if (maze) maze->updateSafetyMaps(posA, posB);

        // Win Condition 1: All fighters of a team are dead
        if (teamAF == 0) {
            isGameOver = true;
            winnerMsg = "RED WINS! (Blue Fighters Dead)";
        }
        else if (teamBF == 0) {
            isGameOver = true;
            winnerMsg = "BLUE WINS! (Red Fighters Dead)";
        }
        else {
            // Win Condition 2: Stalemate (No ammo & No supply) - Check who has more HP
            bool canShootA = (ammoA > 0 || supplyA);
            bool canShootB = (ammoB > 0 || supplyB);

            if (!canShootA && !canShootB) {
                isGameOver = true;
                if (hpA > hpB) winnerMsg = "BLUE WINS BY HP! (No Ammo Left)";
                else if (hpB > hpA) winnerMsg = "RED WINS BY HP! (No Ammo Left)";
                else winnerMsg = "DRAW! (Equal HP, No Ammo Left)";
            }
            else if (!canShootA && !supplyA) {
                isGameOver = true;
                winnerMsg = "RED WINS! (Blue has no supply)"; // Added missing semicolon
            }
            else if (!canShootB && !supplyB) { // Fixed: added 'if' after 'else'
                isGameOver = true;
                winnerMsg = "BLUE WINS! (Red has no supply)"; // Added missing semicolon
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(30, timer, 0); // Loop at ~33 FPS
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(800, 800);
    glutCreateWindow("AI Tactical Battle Simulator");
    init();
    glutDisplayFunc(display);
    glutTimerFunc(30, timer, 0);
    glutMainLoop();
    return 0;
}