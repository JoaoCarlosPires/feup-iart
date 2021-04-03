#include "stone.h"
#include <vector>
#include <string>

using namespace std;



void Stone::setPosition(unsigned int x, unsigned int y) {
    this->posX = x;
    this->posY = y;
}

Stone::Stone(int color, string symbol, bool wall) {
    this->color = color;
    this->symbol = symbol;
    this->wall = wall;
}

bool Stone::isWall() {
    return this->wall;
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

FlatStone::FlatStone(int color, bool wall) : Stone(color, wall?"S":"F", wall) {
}

bool FlatStone::isWall() {
    return this->wall;
}

bool Capstone::isWall() {
    return this->wall;
}

int FlatStone::getColor() {
    return this->color;
}

string FlatStone::getSymbol() {
    return this->symbol;
}

Capstone::Capstone(int color) : Stone(color, "C", false) {
}

int Capstone::getColor() {
    return this->color;
}

string Capstone::getSymbol() {
    return this->symbol;
}

