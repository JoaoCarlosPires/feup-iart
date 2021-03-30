#include "stone.h"
#include <vector>
#include <string>

using namespace std;



void Stone::setPosition(unsigned int x, unsigned int y) {

}

Stone::Stone(int color) {
    this->color = color;
}

string Stone::toString() {
    return "";
}

void FlatStone::changeToFlat() {

}

string FlatStone::toString() {
    return this->symbol;
}

FlatStone::FlatStone(int color, bool wall) : Stone(color) {
    this->wall = wall;
}

string Capstone::toString() {
    return this->symbol;
}

Capstone::Capstone(int color) : Stone(color) {

}
