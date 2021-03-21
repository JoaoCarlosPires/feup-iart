#include "stone.h"
#include <vector>
#include <string>
#include <bits/stdc++.h>

using namespace std;


class Tiles{
    private:
        stack<Stone> stack{};
        unsigned int posX;
        unsigned int posY;
    public:
        Tiles(unsigned int x, unsigned int y);
        void setStone(Stone &s);
};