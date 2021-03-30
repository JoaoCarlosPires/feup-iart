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
        int numFStonesP1; // no. of flat/standing stones for P1
        int numFStonesP2; // no. of flat/standing stones for P2
        int numCStonesP1; // no. of capstones for P1
        int numCStonesP2; // no. of capstones for P2
        Board board = Board(0,0);
    public:
        Tak(int mode, int difficultyP1, int difficultyP2);
        void gameCycle();
        bool endOfGame();
        void getWinner();
        void drawHeader();
        void drawBoard();
        vector<int> getPlay();
        bool canPlay(vector<int> play);
        void makeMove(vector<int> play);
        void changePlayer();
        int getMode();
        string getDifficulty(string diff);
        vector<int> getOriginTile();
        vector<int> getDestTile();
        int getPieceToMove();
        bool isAdjacent(vector<int> origin, vector<int> dest);
};

#endif //FEUP_IART_TAK_H


