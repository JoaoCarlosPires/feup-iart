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

bool Stone::isWall() {
    return false;
}

int Stone::getColor() {
    return this->color;
}

void FlatStone::changeToFlat() {

}

bool FlatStone::isWall() {
    return wall;
}

string FlatStone::toString() {
    return this->symbol;
}

FlatStone::FlatStone(int color, bool wall) : Stone(color) {
    this->wall = wall;
}

int FlatStone::getColor() {
    return this->color;
}

string Capstone::toString() {
    return this->symbol;
}

Capstone::Capstone(int color) : Stone(color) {

}

int Capstone::getColor() {
    return this->color;
}

