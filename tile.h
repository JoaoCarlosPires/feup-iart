#ifndef FEUP_IART_TILE_H
#define FEUP_IART_TILE_H

#include "stone.h"
#include <vector>
#include <string>
#include <bits/stdc++.h>

using namespace std;


class Tile{
    private:
        stack<Stone> stacks;
        unsigned int posX;
        unsigned int posY;
        bool empty;
    public:
        Tile(unsigned int x, unsigned int y);
        void setStone(Stone &s);
        Stone getTop();
        bool isEmpty();
        void add(int pieceType, int color);
        void add2(Stone stone);
        Stone pop();
};

#endif //FEUP_IART_TILE_H
