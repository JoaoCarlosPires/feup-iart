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
