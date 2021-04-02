#include "board.h"

Board::Board(int x, int y) {
    this->x = x;
    this->y = y;

    for (int l = 0; l < this->y; l++) {
        vector<Tile> line;
        for (int c = 0; c < this->x; c++) {
            Tile tile = Tile(c, l);
            line.push_back(tile);
        }
        this->tiles.push_back(line);
    }

}

void Board::draw(int player) {
    vector<vector<Tile>> tiles = getTiles();
    for (int k = 1; k <= this->x; k++) {
        cout << "\t" << k;
    }
    cout << "\n";
    for (int l = 0; l < this->y; l++) {
        cout << l+1 << "\t";
        for (int c = 0; c < this->x; c++) {
            if (tiles[l][c].isEmpty()) cout << "\t";
            else {
                Stone* myStone;
                myStone = tiles[l][c].getTop();
                cout << myStone->getSymbol() << player << "\t";
            }
        }
        cout << "\n";
    }
}

vector<vector<Tile>> Board::getTiles() {
    return this->tiles;
}

int Board::getSize() {
    return this->x;
}

void Board::update(vector<vector<Tile>> board) {
    this->tiles = board;
};

bool Board::allTilesFull() {
    for(int i=0; i<x; i++){
        for(int j=0; j<y; j++){
            if(tiles[j][i].isEmpty()){
                return false;
            }
        }
    }
    return true;
}

