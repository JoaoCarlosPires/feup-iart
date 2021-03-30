#include "board.h"

Board::Board(int x, int y) {
    this->x = x;
    this->y = y;

    for (int c = 0; c < this->x; c++) {
        vector<Tile> line;
        for (int l = 0; l < this->y; l++) {
            Tile tile = Tile(c, l);
            line.push_back(tile);
        }
        this->tiles.push_back(line);
    }

}

void Board::draw() {
    vector<vector<Tile>> tiles = getTiles();
    for (int c = 1; c <= this->x; c++) {
        cout << "\t" << c;
    }
    cout << "\n";
    for (int l = 0; l < this->y; l++) {
        cout << l+1 << "\t";
        for (int c = 0; c < this->x; c++) {
            if (tiles[c][l].isEmpty()) cout << "\t";
            else cout << tiles[c][l].getTop().toString() << "\t";
        }
        cout << "\n";
    }
}

vector<vector<Tile>> Board::getTiles() {
    return this->tiles;
}

int Board::getSize() {
    return this->x;
};

