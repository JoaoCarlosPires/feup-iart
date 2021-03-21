#include "stone.h"
#include "tiles.h"
#include <string>
#include <vector>

using namespace std;


class Board{
    private:
        int dimX;
        int dimY;
        vector<vector<Tiles>> tiles;
    public:
        Board(int x=5, int y=5);
        void draw();
};

