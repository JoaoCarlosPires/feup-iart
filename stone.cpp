#include "stone.h"
#include <vector>
#include <string>

using namespace std;



void Stone::setPosition(unsigned int x, unsigned int y) {
    this->posX = x;
    this->posY = y;
}

Stone::Stone(int color, string symbol) {
    this->color = color;
    this->symbol = symbol;
}

bool Stone::isWall() {
    return false;
}

int Stone::getColor() {
    return this->color;
}

string Stone::getSymbol() {
    return this->symbol;
}

void FlatStone::changeToFlat() {
    this->symbol = "F";
    this->wall = 0;
}

FlatStone::FlatStone(int color, bool wall) : Stone(color, wall?"S":"F") {
    this->wall = wall;
}

bool FlatStone::isWall() {
    return wall;
}

int FlatStone::getColor() {
    return this->color;
}

string FlatStone::getSymbol() {
    return this->symbol;
}

Capstone::Capstone(int color) : Stone(color, "C") {
}

int Capstone::getColor() {
    return this->color;
}

string Capstone::getSymbol() {
    return this->symbol;
}

