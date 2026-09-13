#include "PathPlanner.h"
#include <queue>
#include <math.h>

struct NodeComparator {
    bool operator()(const Node* n1, const Node* n2) const {
        return n1->f > n2->f;
    }
};

double PathPlanner::Heuristic(int x1, int y1, int x2, int y2) {
    return (double)abs(x1 - x2) + abs(y1 - y2);
}

std::vector<std::pair<int, int>> PathPlanner::FindPath(Maze* m, int startX, int startY, int targetX, int targetY, int teamId) {
    std::priority_queue<Node*, std::vector<Node*>, NodeComparator> openList;
    std::vector<std::vector<Node*>> nodeGrid(MAZE_SIZE, std::vector<Node*>(MAZE_SIZE, nullptr));

    Node* startNode = new Node(startX, startY);
    startNode->g = 0;
    startNode->h = Heuristic(startX, startY, targetX, targetY);
    startNode->f = startNode->g + startNode->h;

    openList.push(startNode);
    nodeGrid[startX][startY] = startNode;

    std::vector<std::pair<int, int>> finalPath;

    while (!openList.empty()) {
        Node* current = openList.top();
        openList.pop();

        if (current->x == targetX && current->y == targetY) {
            Node* temp = current;
            while (temp != nullptr) {
                finalPath.push_back({ temp->x, temp->y });
                temp = temp->parent;
            }
            break;
        }

        current->closed = true;

        int dx[] = { 0, 0, 1, -1 };
        int dy[] = { 1, -1, 0, 0 };

        for (int i = 0; i < 4; i++) {
            int nx = current->x + dx[i];
            int ny = current->y + dy[i];

            if (nx >= 0 && nx < MAZE_SIZE && ny >= 0 && ny < MAZE_SIZE &&
                m->getCell(nx, ny) != WALL && m->getCell(nx, ny) != ROCK && m->getCell(nx, ny) != TREE) {

                if (nodeGrid[nx][ny] != nullptr && nodeGrid[nx][ny]->closed) continue;

                double danger = m->getDanger(nx, ny, teamId);
                double movementCost = 1.0 + (danger * 10.0);

                double tentative_g = current->g + movementCost;

                if (nodeGrid[nx][ny] == nullptr) {
                    Node* neighbor = new Node(nx, ny);
                    neighbor->parent = current;
                    neighbor->g = tentative_g;
                    neighbor->h = Heuristic(nx, ny, targetX, targetY);
                    neighbor->f = neighbor->g + neighbor->h;
                    nodeGrid[nx][ny] = neighbor;
                    openList.push(neighbor);
                }
                else if (tentative_g < nodeGrid[nx][ny]->g) {
                    nodeGrid[nx][ny]->parent = current;
                    nodeGrid[nx][ny]->g = tentative_g;
                    nodeGrid[nx][ny]->f = nodeGrid[nx][ny]->g + nodeGrid[nx][ny]->h;
                }
            }
        }
    }

    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            if (nodeGrid[i][j] != nullptr) {
                delete nodeGrid[i][j];
            }
        }
    }

    return finalPath;
}