#ifndef FEUP_IART_BOARD_H
#define FEUP_IART_BOARD_H

#include "stone.h"
#include "tile.h"
#include <string>
#include <vector>

using namespace std;


class Board{
    private:
        int x;
        int y;
        vector<vector<Tile>> tiles;
    public:
        Board(int x, int y);
        void draw();
        vector<vector<Tile>> getTiles();
        int getSize();
        void update(vector<vector<Tile>> board);
};

#endif //FEUP_IART_BOARD_H

