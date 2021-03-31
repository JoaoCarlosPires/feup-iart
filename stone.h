#ifndef FEUP_IART_STONE_H
#define FEUP_IART_STONE_H

#include <string>

using namespace std;

class Stone{
    protected:
        int color;
        unsigned int posX;
        unsigned int posY;
    public:
        Stone(int color);
        void setPosition(unsigned int x, unsigned int y);
        virtual string toString();
        virtual int getColor();
};

class Capstone: public Stone{
    private:
        string symbol = "C";
    public:
        Capstone(int color);
        string toString() override ;
        int getColor() override;
};

class FlatStone: public Stone{
    private:
        string symbol = "F";
        bool wall;
    public:
        FlatStone(int color, bool wall);
        void changeToFlat();
        string toString() override ;
        int getColor() override;
};

#endif //FEUP_IART_STONE_H
