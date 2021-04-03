#ifndef FEUP_IART_TAK_H
#define FEUP_IART_TAK_H

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
        Tak(int mode, int difficultyP1, int difficultyP2, int size);
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
        bool pathHorizontal(vector<vector<Tile>> board);
        bool pathVertical(vector<vector<Tile>> board);
        int semiPathHorizontal(vector<vector<Tile>> board);
        int semiPathVertical(vector<vector<Tile>> board);
        bool isPathBuilt(vector<vector<Tile>> board);
        int lenghtSemiPathBuilt(vector<vector<Tile>> board);
        int visibleWinner();
        bool getPathV(vector<vector<Tile>> tiles, int line, int col);
        bool getPathH(vector<vector<Tile>> tiles, int line, int col);
        int getPathV2(vector<vector<Tile>> tiles, int line, int col);
        int getPathH2(vector<vector<Tile>> tiles, int line, int col);
        bool isPlayable(vector<int> origin, vector<int> dest, int numOfPieces);
        vector<vector<int>> getAllPossibleMoves(vector<vector<Tile>> currentBoard, int player);
        vector<int> minimax(vector<vector<int>> allMoves);
        int evaluator(vector<vector<Tile>> currentBoard);
        int GetMaxMove(vector<vector<Tile>> currentBoard, int alpha, int beta, int diff);
        int GetMinMove(vector<vector<Tile>> currentBoard, int diff);
        vector<vector<Tile>> simulateMakeMove(vector<vector<Tile>> board, vector<int> play);
        int getNumFStones(int player);
        int getNumCStones(int player);
        void infoOrPlay();
};

#endif //FEUP_IART_TAK_H


