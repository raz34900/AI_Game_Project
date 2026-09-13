#pragma once

class Node
{
public:
    int x, y;
    double g, h, f;
    Node* parent;

    Node(int x, int y, Node* parent, double g, double h)
    {
        this->x = x;
        this->y = y;
        this->parent = parent;
        this->g = g;
        this->h = h;
        this->f = g + h; // Total cost
    }
};