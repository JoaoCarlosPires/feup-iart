#include "tile.h"

Stone Tile::getTop() {
    return stacks.top();
}

void Tile::setStone(Stone &s) {

}

int Tile::getSize() {
    return stacks.size();
}

stack<Stone> Tile::getStack() {
    return stacks;
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

stack<Stone> Tile::getStackMove(int numberPieces) {
    stack<Stone> result;
    for(int i = 0; i<numberPieces; i++){
        result.push(stacks.top());
        stacks.pop();
    }
    return result;
}
