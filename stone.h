#ifndef FEUP_IART_STONE_H
#define FEUP_IART_STONE_H

#include <string>

using namespace std;

class Stone{
    protected:
        int color;
        string symbol;
        unsigned int posX;
        unsigned int posY;
    public:
        Stone(int color, string symbol);
        void setPosition(unsigned int x, unsigned int y);
        virtual int getColor();
        virtual string getSymbol();
        virtual bool isWall();
};

class Capstone: public Stone{
    private:
        string symbol;
    public:
        Capstone(int color);
        string getSymbol() override ;
        int getColor() override;
};

class FlatStone: public Stone{
    private:
        string symbol;
        bool wall;
    public:
        FlatStone(int color, bool wall);
        void changeToFlat();
        string getSymbol() override ;
        int getColor() override;
        bool isWall() override;
};

#endif //FEUP_IART_STONE_H
