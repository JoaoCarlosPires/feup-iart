#include "tile.h"

Stone Tile::getTop() {
    return stacks.top();
}

void Tile::setStone(Stone &s) {

}

Tile::Tile(unsigned int x, unsigned int y) {
    this->posX = x;
    this->posY = y;
    this->empty = true;
}

bool Tile::isEmpty() {
    return this->empty;
}

void Tile::add(int pieceType, int color) {
    switch (pieceType) {
        case 1: {
            FlatStone newF = FlatStone(color, false);
            this->stacks.push(newF);
            break;
        }
        case 2: {
            FlatStone newF = FlatStone(color, true);
            this->stacks.push(newF);
            break;
        }
        case 3: {
            Capstone newC = Capstone(color);
            this->stacks.push(newC);
            return;
        }
        default:
            cout << "Error!\n";
            break;
    }
}

void Tile::add2(Stone stone) {
    this->stacks.push(stone);
}

Stone Tile::pop() {
    Stone toReturn = this->stacks.top();
    this->stacks.pop();
    return toReturn;
}
