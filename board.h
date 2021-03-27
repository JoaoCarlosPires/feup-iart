#ifndef FEUP_IART_BOARD_H
#define FEUP_IART_BOARD_H

#include "stone.h"
#include "tiles.h"
#include <string>
#include <vector>

using namespace std;


class Board{
    private:
        int x;
        int y;
        vector<vector<Tiles>> tiles;
    public:
        Board(int x, int y);
        void draw();
};

#endif //FEUP_IART_BOARD_H

