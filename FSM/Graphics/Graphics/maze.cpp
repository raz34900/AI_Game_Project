#include "Maze.h"
#include <stdlib.h> 
#include <time.h>   
#include <cmath>

// Define PI for drawing smooth circles
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Maze::Maze() {
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            grid[i][j] = SPACE;
            dangerForTeamA[i][j] = 0;
            dangerForTeamB[i][j] = 0;
        }
    }
}

void Maze::init() {
    srand((unsigned int)time(NULL));

    // 1. Build 6x6 room structure
    generateRoomsAndCorridors();

    // 2. Place 1 large center lake and 4 small corner lakes
    placeLakes();

    // 3. Place Hospitals and Armories
    placeItems(AMMO, 2);
    placeItems(HEALTH, 2);

    // 4. Place larger 2x2 obstacles (Trees and Rocks) for cover
    placeObstacles(40);

    // 5. Place Shield Houses
    placeShields(2);
}

// Generates the 2x2 Shield Houses
void Maze::placeShields(int count) {
    int placed = 0;
    while (placed < count) {
        int x = rand() % (MAZE_SIZE - 4) + 2;
        int y = rand() % (MAZE_SIZE - 4) + 2;
        // Check if a 2x2 area is completely free
        if (grid[x][y] == SPACE && grid[x + 1][y] == SPACE && grid[x][y + 1] == SPACE && grid[x + 1][y + 1] == SPACE) {
            grid[x][y] = SHIELD; grid[x + 1][y] = SHIELD;
            grid[x][y + 1] = SHIELD; grid[x + 1][y + 1] = SHIELD;
            placed++;
        }
    }
}

void Maze::generateRoomsAndCorridors() {
    // Outer boundary walls
    for (int i = 0; i < MAZE_SIZE; i++) {
        grid[i][0] = WALL; grid[i][MAZE_SIZE - 1] = WALL;
        grid[0][i] = WALL; grid[MAZE_SIZE - 1][i] = WALL;
    }

    // Inner walls for 6x6 grid
    for (int i = 1; i < MAZE_SIZE - 1; i++) {
        if (i % 16 == 0) {
            for (int j = 1; j < MAZE_SIZE - 1; j++) {
                // Leave openings (doors)
                if (!(j % 16 > 5 && j % 16 < 11)) {
                    grid[i][j] = WALL;
                    grid[j][i] = WALL;
                }
            }
        }
    }
}

void Maze::placeLakes() {
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            // Calculate distance for the large center lake
            double distToCenter = sqrt(pow(i - 50.0, 2) + pow(j - 50.0, 2));

            // Calculate distances for the 4 smaller corner lakes
            double distTopLeft = sqrt(pow(i - 24.0, 2) + pow(j - 24.0, 2));
            double distTopRight = sqrt(pow(i - 72.0, 2) + pow(j - 24.0, 2));
            double distBotLeft = sqrt(pow(i - 24.0, 2) + pow(j - 72.0, 2));
            double distBotRight = sqrt(pow(i - 72.0, 2) + pow(j - 72.0, 2));

            // Center lake radius = 14.0 | Small corner lakes radius = 6.0
            if (distToCenter < 14.0 || distTopLeft < 6.0 || distTopRight < 6.0 || distBotLeft < 6.0 || distBotRight < 6.0) {
                grid[i][j] = WATER;
            }
        }
    }
}

void Maze::placeItems(int itemType, int count) {
    int placed = 0;
    while (placed < count) {
        int x = rand() % (MAZE_SIZE - 4) + 2;
        int y = rand() % (MAZE_SIZE - 4) + 2;
        if (grid[x][y] == SPACE) {
            grid[x][y] = itemType;
            placed++;
        }
    }
}

void Maze::placeObstacles(int count) {
    int placed = 0;
    while (placed < count) {
        int x = rand() % (MAZE_SIZE - 4) + 2;
        int y = rand() % (MAZE_SIZE - 4) + 2;
        // Check if a 2x2 area is completely free
        if (grid[x][y] == SPACE && grid[x + 1][y] == SPACE && grid[x][y + 1] == SPACE && grid[x + 1][y + 1] == SPACE) {
            int obsType = (rand() % 2 == 0) ? TREE : ROCK;
            grid[x][y] = obsType; grid[x + 1][y] = obsType;
            grid[x][y + 1] = obsType; grid[x + 1][y + 1] = obsType;
            placed++;
        }
    }
}

int Maze::getCell(int x, int y) const {
    if (x >= 0 && x < MAZE_SIZE && y >= 0 && y < MAZE_SIZE) return grid[x][y];
    return WALL;
}

void Maze::setCell(int x, int y, int value) {
    if (x >= 0 && x < MAZE_SIZE && y >= 0 && y < MAZE_SIZE) grid[x][y] = value;
}

// --- MINE & EXPLOSION LOGIC ---
void Maze::placeMine(double x, double y, int teamID) {
    activeMines.push_back({ x, y, teamID, true });
}

bool Maze::checkAndTriggerMine(double x, double y, int teamID) {
    for (auto& m : activeMines) {
        // Trigger if active and belongs to the ENEMY team
        if (m.active && m.teamID != teamID) {
            double dist = sqrt(pow(m.x - x, 2) + pow(m.y - y, 2));
            if (dist < 1.2) { // Detection radius
                m.active = false;
                addExplosion(m.x, m.y);
                return true;
            }
        }
    }
    return false;
}

void Maze::addExplosion(double x, double y) {
    // Generate 25 green shrapnel pixels flying in random directions
    for (int i = 0; i < 25; i++) {
        double vx = (rand() % 100 - 50) / 70.0;
        double vy = (rand() % 100 - 50) / 70.0;
        shrapnel.push_back({ x, y, vx, vy, 15 }); // Live for 15 frames
    }
}
// ------------------------------

void Maze::draw() {
    // Draw grid elements
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            if (grid[i][j] == WALL) {
                glColor3d(0.3, 0.3, 0.3);
                glBegin(GL_POLYGON);
                glVertex2d(i, j); glVertex2d(i, j + 1);
                glVertex2d(i + 1, j + 1); glVertex2d(i + 1, j);
                glEnd();
            }
            else if (grid[i][j] == WATER) {
                glColor3d(0.2, 0.5, 0.8);
                glBegin(GL_POLYGON);
                glVertex2d(i, j); glVertex2d(i, j + 1);
                glVertex2d(i + 1, j + 1); glVertex2d(i + 1, j);
                glEnd();
            }
            else if (grid[i][j] == TREE) {
                glColor3d(0.0, 0.4, 0.1);
                glBegin(GL_POLYGON);
                glVertex2d(i - 0.2, j - 0.2); glVertex2d(i + 1.2, j - 0.2);
                glVertex2d(i + 1.2, j + 1.2); glVertex2d(i - 0.2, j + 1.2);
                glEnd();
            }
            else if (grid[i][j] == ROCK) {
                glColor3d(0.5, 0.5, 0.5);
                glBegin(GL_POLYGON);
                glVertex2d(i, j - 0.2); glVertex2d(i + 1.2, j + 0.2);
                glVertex2d(i + 1, j + 1); glVertex2d(i - 0.2, j + 0.8);
                glEnd();
            }
            else if (grid[i][j] == HEALTH) {
                // Hospital Building
                glColor3d(0.9, 0.9, 0.9);
                glBegin(GL_POLYGON);
                glVertex2d(i - 1, j - 1); glVertex2d(i + 2, j - 1);
                glVertex2d(i + 2, j + 2); glVertex2d(i - 1, j + 2);
                glEnd();
                // Red Cross
                glColor3d(0.8, 0, 0);
                glBegin(GL_POLYGON);
                glVertex2d(i + 0.2, j - 0.5); glVertex2d(i + 0.8, j - 0.5);
                glVertex2d(i + 0.8, j + 1.5); glVertex2d(i + 0.2, j + 1.5);
                glEnd();
                glBegin(GL_POLYGON);
                glVertex2d(i - 0.5, j + 0.2); glVertex2d(i + 1.5, j + 0.2);
                glVertex2d(i + 1.5, j + 0.8); glVertex2d(i - 0.5, j + 0.8);
                glEnd();
            }
            else if (grid[i][j] == AMMO) {
                // Armory Building
                glColor3d(0.0, 0.0, 0.0);
                glBegin(GL_POLYGON);
                glVertex2d(i - 1, j - 1); glVertex2d(i + 2, j - 1);
                glVertex2d(i + 2, j + 2); glVertex2d(i - 1, j + 2);
                glEnd();
                // Orange accent
                glColor3d(1.0, 0.5, 0.0);
                glBegin(GL_POLYGON);
                glVertex2d(i + 0.2, j + 0.2); glVertex2d(i + 0.8, j + 0.2);
                glVertex2d(i + 0.8, j + 0.8); glVertex2d(i + 0.2, j + 0.8);
                glEnd();
            }
            // NEW VISUALS: Draw ONE large silver circle for the 2x2 SHIELD block
            else if (grid[i][j] == SHIELD) {
                // Only draw the graphic if this is the bottom-left cell of the 2x2 block
                bool isBottomLeft = true;
                if (i > 0 && grid[i - 1][j] == SHIELD) isBottomLeft = false;
                if (j > 0 && grid[i][j - 1] == SHIELD) isBottomLeft = false;

                if (isBottomLeft) {
                    double cx = i + 1.0; // Center of the 2x2 area
                    double cy = j + 1.0;
                    double radius = 1.4; // Large size like the hospital

                    // 1. Large Silver Base Circle
                    glColor3d(0.75, 0.75, 0.75); // Silver
                    glBegin(GL_POLYGON);
                    for (int k = 0; k < 32; k++) { // 32 segments for smoothness
                        double angle = 2.0 * M_PI * k / 32.0;
                        glVertex2d(cx + radius * cos(angle), cy + radius * sin(angle));
                    }
                    glEnd();

                    // 2. Thin Black Inner Circle
                    glColor3d(0, 0, 0); // Black
                    glLineWidth(2.0);
                    glBegin(GL_LINE_LOOP);
                    for (int k = 0; k < 32; k++) {
                        double angle = 2.0 * M_PI * k / 32.0;
                        glVertex2d(cx + (radius * 0.7) * cos(angle), cy + (radius * 0.7) * sin(angle));
                    }
                    glEnd();
                }
            }
        }
    }

    // Draw active Landmines
    for (const auto& m : activeMines) {
        if (m.active) {
            // Dark grey plate
            glColor3d(0.2, 0.2, 0.2);
            glBegin(GL_POLYGON);
            glVertex2d(m.x - 0.8, m.y - 0.8); glVertex2d(m.x + 0.8, m.y - 0.8);
            glVertex2d(m.x + 0.8, m.y + 0.8); glVertex2d(m.x - 0.8, m.y + 0.8);
            glEnd();
            // Red blinking light
            glColor3d(1.0, 0.0, 0.0);
            glPointSize(6.0);
            glBegin(GL_POINTS);
            glVertex2d(m.x, m.y);
            glEnd();
        }
    }
}

void Maze::addAnimation(double sx, double sy, double tx, double ty, int type) {
    animations.push_back({ sx, sy, tx, ty, type, 10 });
}

void Maze::addBlood(double bx, double by) {
    bloodSplatters.push_back({ bx + ((rand() % 10 - 5) / 10.0), by + ((rand() % 10 - 5) / 10.0), 150 });
}

void Maze::drawAnimations() {
    // 1. Draw Blood Splatters (Bottom layer)
    for (auto it = bloodSplatters.begin(); it != bloodSplatters.end();) {
        glColor3d(0.7, 0.0, 0.0);
        glBegin(GL_POLYGON);
        glVertex2d(it->x - 0.3, it->y - 0.3); glVertex2d(it->x + 0.3, it->y - 0.3);
        glVertex2d(it->x + 0.3, it->y + 0.3); glVertex2d(it->x - 0.3, it->y + 0.3);
        glEnd();

        it->life--;
        if (it->life <= 0) it = bloodSplatters.erase(it);
        else ++it;
    }

    // 2. Draw Shrapnel (Explosion pixels)
    for (auto it = shrapnel.begin(); it != shrapnel.end();) {
        glColor3d(0.0, 1.0, 0.0); // Bright Green pixels
        glPointSize(4.0);
        glBegin(GL_POINTS);
        glVertex2d(it->x, it->y);
        glEnd();

        // Move particle
        it->x += it->vx;
        it->y += it->vy;
        it->life--;

        if (it->life <= 0) it = shrapnel.erase(it);
        else ++it;
    }

    // 3. Draw Projectiles (Bullets and Grenades)
    for (auto it = animations.begin(); it != animations.end();) {
        double progress = 1.0 - (it->timer / 10.0);
        double bx = it->sx + (it->tx - it->sx) * progress;
        double by = it->sy + (it->ty - it->sy) * progress;

        if (it->type == 0) { // Bullet
            glColor3d(0, 0, 0);
            glBegin(GL_POLYGON);
            glVertex2d(bx - 0.5, by - 0.5); glVertex2d(bx + 0.5, by - 0.5);
            glVertex2d(bx + 0.5, by + 0.5); glVertex2d(bx - 0.5, by + 0.5);
            glEnd();
        }
        else { // Grenade
            glColor3d(0, 1, 0);
            glBegin(GL_POLYGON);
            glVertex2d(bx - 1, by - 0.5); glVertex2d(bx, by - 0.5);
            glVertex2d(bx, by + 0.5); glVertex2d(bx - 1, by + 0.5);
            glEnd();
            glBegin(GL_POLYGON);
            glVertex2d(bx + 0.5, by - 0.5); glVertex2d(bx + 1.5, by - 0.5);
            glVertex2d(bx + 1.5, by + 0.5); glVertex2d(bx + 0.5, by + 0.5);
            glEnd();
        }

        it->timer--;
        if (it->timer <= 0) {
            // Trigger explosion pixels when grenade reaches target
            if (it->type == 1) {
                addExplosion(it->tx, it->ty);
            }
            it = animations.erase(it);
        }
        else ++it;
    }
}

void Maze::updateSafetyMaps(const std::vector<std::pair<int, int>>& teamA_pos, const std::vector<std::pair<int, int>>& teamB_pos) {
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            dangerForTeamA[i][j] = 0;
            dangerForTeamB[i][j] = 0;
        }
    }

    auto projectDanger = [&](const std::vector<std::pair<int, int>>& positions, double targetMap[MAZE_SIZE][MAZE_SIZE]) {
        for (const auto& pos : positions) {
            int nx = pos.first; int ny = pos.second;
            for (int dx = -10; dx <= 10; dx++) {
                for (int dy = -10; dy <= 10; dy++) {
                    if (nx + dx >= 0 && nx + dx < MAZE_SIZE && ny + dy >= 0 && ny + dy < MAZE_SIZE) {
                        double dist = sqrt(dx * dx + dy * dy);
                        // Water does not block danger projection, but solid obstacles do
                        if (dist < 10 && grid[nx + dx][ny + dy] != WALL && grid[nx + dx][ny + dy] != TREE && grid[nx + dx][ny + dy] != ROCK) {
                            targetMap[nx + dx][ny + dy] += (10.0 - dist) * 5.0;
                        }
                    }
                }
            }
        }
        };

    projectDanger(teamA_pos, dangerForTeamB);
    projectDanger(teamB_pos, dangerForTeamA);
}

double Maze::getDanger(int x, int y, int teamID) const {
    if (x < 0 || x >= MAZE_SIZE || y < 0 || y >= MAZE_SIZE) return 0;
    return (teamID == 0) ? dangerForTeamA[x][y] : dangerForTeamB[x][y];
}