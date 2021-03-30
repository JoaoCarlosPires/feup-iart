#ifndef FEUP_IART_STONE_H
#define FEUP_IART_STONE_H

#include <string>

using namespace std;

class Stone{
    protected:
        char colour;
        unsigned int posX;
        unsigned int posY;
    public:
        Stone(char color);
        void setPosition(unsigned int x, unsigned int y);
        virtual string toString();
};

class Capstone: public Stone{
    private:
        string symbol = "C";
    public:
        string toString() override ;

};

class FlatStone: public Stone{
    private:
        string symbol = "F";
        bool wall;
    public:
        void changeToFlat();
        string toString() override ;
};

#endif //FEUP_IART_STONE_H
