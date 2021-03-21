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
};

class Capstone: public Stone{
    private:

    public:

};

class FlatStone: public Stone{
    private:
        bool wall;
    public:
};