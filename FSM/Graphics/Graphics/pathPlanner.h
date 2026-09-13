#ifndef PATHPLANNER_H
#define PATHPLANNER_H

#include <vector>
#include "Maze.h"

struct Node {
    int x, y;
    double g, h, f;
    Node* parent;
    bool closed;

    Node(int x, int y) : x(x), y(y), g(0), h(0), f(0), parent(nullptr), closed(false) {}
};

class PathPlanner {
public:
    static std::vector<std::pair<int, int>> FindPath(Maze* m, int startX, int startY, int targetX, int targetY, int teamId);
    static double Heuristic(int x1, int y1, int x2, int y2);
};

#endif