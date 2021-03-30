#include "stone.h"
#include <vector>
#include <string>

using namespace std;



void Stone::setPosition(unsigned int x, unsigned int y) {

}

Stone::Stone(char color) {

}

string Stone::toString() {
    return "";
}

void FlatStone::changeToFlat() {

}

string FlatStone::toString() {
    return this->symbol;
}

string Capstone::toString() {
    return this->symbol;
}
