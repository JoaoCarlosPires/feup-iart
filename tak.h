#ifndef FEUP_IART_TAK_H
#define FEUP_IART_TAK_H

#include "player.h"
#include "board.h"

using namespace std;

class Tak{
    private:
        int mode;
        int difficultyP1;
        int difficultyP2;
        int currentPlayer;
        Board board = Board(0,0);
    public:
        Tak(int mode, int difficultyP1, int difficultyP2);
        void gameCycle();
        bool endOfGame();
        void getWinner();
        void drawHeader();
        void drawBoard();
        vector<int> getPlay();
        bool canPlay();
        void makeMove(vector<int> play);
        void changePlayer();
        int getMode();
        string getDifficulty(string diff);
};

#endif //FEUP_IART_TAK_H


