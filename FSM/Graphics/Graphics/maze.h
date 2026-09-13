#pragma once
#include "glut.h"
#include <vector>
#include <utility>

const int MAZE_SIZE = 100;

const int SPACE = 0;
const int WALL = 1;
const int AMMO = 2;
const int HEALTH = 3;
const int TREE = 4;
const int ROCK = 5;
const int WATER = 6;
const int SHIELD = 7; // NEW: Shield House constant

struct AttackAnim {
    double sx, sy, tx, ty;
    int type;
    int timer;
};

// Shrapnel particles for grenade and mine explosions
struct ExplosionParticle {
    double x, y, vx, vy;
    int life;
};

// Landmines
struct Mine {
    double x, y;
    int teamID;
    bool active;
};

struct Blood {
    double x, y;
    int life;
};

class Maze
{
private:
    int grid[MAZE_SIZE][MAZE_SIZE];
    double dangerForTeamA[MAZE_SIZE][MAZE_SIZE];
    double dangerForTeamB[MAZE_SIZE][MAZE_SIZE];

    std::vector<AttackAnim> animations;
    std::vector<ExplosionParticle> shrapnel; // Store explosion pixels
    std::vector<Mine> activeMines;           // Store placed mines
    std::vector<Blood> bloodSplatters;

    void generateRoomsAndCorridors();
    void placeLakes(); // Changed to handle multiple lakes
    void placeItems(int itemType, int count);
    void placeObstacles(int count);
    void placeShields(int count); // NEW: Function to place Shield Houses

public:
    Maze();
    void init();
    void draw();

    int getCell(int x, int y) const;
    void setCell(int x, int y, int value);

    void updateSafetyMaps(const std::vector<std::pair<int, int>>& teamA_pos, const std::vector<std::pair<int, int>>& teamB_pos);
    double getDanger(int x, int y, int teamID) const;

    void addAnimation(double sx, double sy, double tx, double ty, int type);
    void addExplosion(double x, double y); // Trigger green pixel burst
    void placeMine(double x, double y, int teamID); // Drop a mine
    bool checkAndTriggerMine(double x, double y, int teamID); // Check if stepped on

    void addBlood(double x, double y);
    void drawAnimations();
};