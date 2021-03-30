#include "tile.h"

Stone Tile::getTop() {
    return stack.top();
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
            this->stack.push(newF);
            break;
        }
        case 2: {
            FlatStone newF = FlatStone(color, true);
            this->stack.push(newF);
            break;
        }
        case 3: {
            Capstone newC = Capstone(color);
            this->stack.push(newC);
            return;
        }
        default:
            cout << "Error!\n";
            break;
    }
}

void Tile::add2(Stone stone) {
    this->stack.push(stone);
}

Stone Tile::pop() {
    Stone toReturn = this->stack.top();
    this->stack.pop();
    return toReturn;
}
