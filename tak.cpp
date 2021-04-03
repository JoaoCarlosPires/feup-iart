#include "tak.h"
#include <string>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

using namespace std::chrono;

int numFStonesP1;
int numFStonesP2;
int numCStonesP1;
int numCStonesP2;

Tak::Tak(int mode, int difficultyP1, int difficultyP2, int size) {
    this->mode = mode;
    this->difficultyP1 = difficultyP1;
    this->difficultyP2 = difficultyP2;
    this->currentPlayer = 2;

    switch (size) {
        case 3:
            numFStonesP1 = 10;
            numFStonesP2 = 10;
            numCStonesP1 = 0;
            numCStonesP2 = 0;
            break;
        case 4:
            numFStonesP1 = 15;
            numFStonesP2 = 15;
            numCStonesP1 = 0;
            numCStonesP2 = 0;
            break;
        case 5:
            numFStonesP1 = 21;
            numFStonesP2 = 21;
            numCStonesP1 = 1;
            numCStonesP2 = 1;
            break;
        case 6:
            numFStonesP1 = 30;
            numFStonesP2 = 30;
            numCStonesP1 = 1;
            numCStonesP2 = 1;
            break;
        case 7:
            numFStonesP1 = 40;
            numFStonesP2 = 40;
            numCStonesP1 = 2;
            numCStonesP2 = 2;
            break;
        case 8:
            numFStonesP1 = 50;
            numFStonesP2 = 50;
            numCStonesP1 = 2;
            numCStonesP2 = 2;
            break;
        default:
            break;
    }

    this->board = Board(size, size);
}

void Tak::gameCycle() {
    if (endOfGame()) {
        cout << "\n\nFinal Board\n\n";
        drawBoard();
        getWinner();
        return ;
    }

    changePlayer();

    drawHeader();
    drawBoard();

    vector<int> play;

    // Mode Player vs Computer (Computer Turn) or Computer vs Computer
    if ((this->mode == 2 && this->currentPlayer == 2) || (this->mode == 3)) {
        auto start = high_resolution_clock::now();

        vector<vector<int>> allMoves = getAllPossibleMoves(this->board.getTiles(), this->currentPlayer);
        play = minimax(allMoves);

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Time taken to get a play: " << duration.count() << " microseconds" << endl;

        makeMove(play);
        gameCycle();
    } else { // Mode Player vs Player or Player vs Computer (Player Turn)
        infoOrPlay();
    }

}

void Tak::infoOrPlay() {
    cout << "\nPlease select one option below:\n";
    cout << "1. Play\n";
    cout << "2. View tile content (stack)\n";
    cout << "3. Hint\n";

    int option = 0;
    while (option != 1 && option != 2 && option != 3) {
        cout << "\nInput: ";
        cin >> option;
    }

    if (option == 1) {
        vector<int> play = getPlay();

        while (!canPlay(play)) {
            play = getPlay();
        }

        makeMove(play);
        gameCycle();
    } else if (option == 2) {
        cout << "\nMORE INFO TILE";
        cout << "\nPlease choose line. Insert a value between 1 and " << this->board.getSize();

        int line = 0;
        while (line < 1 || line > this->board.getSize()) {
            cout << "\nInput: ";
            cin >> line;
        }
        line--;

        cout << "\nPlease choose column. Insert a value between 1 and " << this->board.getSize();

        int col = 0;
        while (col < 1 || col > this->board.getSize()) {
            cout << "\nInput: ";
            cin >> col;
        }
        col--;

        if (!this->board.getTiles()[line][col].isEmpty()) {
            cout << "TOP -> BOTTOM\n";
            stack<Stone> tile = this->board.getTiles()[line][col].getStack();
            while (!tile.empty()) {
                cout << tile.top().getSymbol() << tile.top().getColor() << " - ";
                tile.pop();
            }
            cout << "\n";
        } else {
            cout << "EMPTY TILE\n";
        }

        infoOrPlay();
    } else {
        vector<vector<int>> allMoves = getAllPossibleMoves(this->board.getTiles(), this->currentPlayer);
        vector<int> play = minimax(allMoves);

        switch (play.size()) {
            case 3: {
                string pieceToMove;
                switch (play[0]) {
                    case 1:
                        pieceToMove = "Flat Stone";
                        break;
                    case 2:
                        pieceToMove = "Standing Stone";
                        break;
                    case 3:
                        pieceToMove = "Capstone";
                        break;
                    default:
                        cout << "ERROR!\n";
                        break;
                }
                cout << "\nMove " << pieceToMove << " to position " << play[1]+1 << ", " << play[2]+1 << "\n";
                break;
            }
            case 4: {
                cout << "\nMove top piece from " << play[0]+1 << ", " << play[1]+1 << " to position " << play[2]+1 << ", " << play[3]+1 << "\n";
                break;
            }
            default:
                cout << "Error0\n";
                break;
        }

        infoOrPlay();
    }
}

int Tak::evaluator(vector<vector<Tile>> currentBoard) {
    int score = 0;
    int diff = this->currentPlayer == 1 ? this->difficultyP1:this->difficultyP2;

    if (diff == 1) { // Difficulty Easy : random evaluation of play
        score = rand() % 100;
    } else {
        if (isPathBuilt(currentBoard)) {
            score = 100;
        } else {
            score = rand() % 100;

            // Adding to score the number of visible pieces in the middle capable of building a path
            for (int i = 1; i < currentBoard.size()-1; i++)
                for (int j = 1; j < currentBoard.size()-1; j++)
                    if (!currentBoard[i][j].isEmpty())
                        if (this->currentPlayer == currentBoard[i][j].getTop()->getColor() && currentBoard[i][j].getTop()->getSymbol() != "S")
                            score += 1;

            // Adds the lenght of paths already built for both directions
            score += lenghtSemiPathBuilt(currentBoard);
        }
    }

    return score;
}

vector<int> Tak::minimax(vector<vector<int>> allMoves) {
    vector<int> bestMove, currentMove;
    int bestScore = 0, currentScore;
    int diff = this->currentPlayer==1?this->difficultyP1:this->difficultyP2;

    for (int i = 0; i < allMoves.size(); i++) {
        currentMove = allMoves[i];
        currentScore = GetMinMove(simulateMakeMove(this->board.getTiles(), currentMove), diff);
        if (bestScore < currentScore) {
            bestScore = currentScore;
            bestMove = currentMove;
        }
    }

    return bestMove;
}

int Tak::GetMaxMove(vector<vector<Tile>> currentBoard, int alpha, int beta, int diff) {

    int bestScore = 0;
    int currentScore;

    vector<vector<int>> allMoves;
    allMoves.clear();
    allMoves = getAllPossibleMoves(currentBoard, this->currentPlayer);

    if (diff <= 2) {
        for (int i = 0; i < allMoves.size(); i++) {
            currentScore = evaluator(simulateMakeMove(currentBoard, allMoves[i]));
            if (bestScore < currentScore) {
                bestScore = currentScore;
            }
        }
    } else {
        vector<int> currentMove;
        for (int i = 1; i < allMoves.size(); i++) {
            currentMove = allMoves[i];
            currentScore = GetMaxMove(simulateMakeMove(currentBoard, currentMove), alpha, beta, diff-1);
            bestScore = bestScore>currentScore?bestScore:currentScore;
            alpha = beta>bestScore?beta:bestScore;
            if (beta <= alpha)
                break;
        }
    }

    return bestScore;
}

int Tak::GetMinMove(vector<vector<Tile>> currentBoard, int diff) {
    int alpha, beta;
    int currentScore;
    vector<int> currentMove;

    int nextPlayer = (this->currentPlayer==1)?2:1;
    vector<vector<int>> allMoves = getAllPossibleMoves(currentBoard, nextPlayer);

    currentMove = allMoves[0];
    int bestScore = GetMaxMove(simulateMakeMove(currentBoard, currentMove), alpha, beta, diff);
    for (int i = 1; i < allMoves.size(); i++) {
        currentMove = allMoves[i];
        currentScore = GetMaxMove(simulateMakeMove(currentBoard, currentMove), alpha, beta, diff);
        bestScore = bestScore<currentScore?bestScore:currentScore;
        beta = beta<bestScore?beta:bestScore;
        if (beta <= alpha)
            break;
    }

    return bestScore;
}

vector<vector<int>> Tak::getAllPossibleMoves(vector<vector<Tile>> currentBoard, int player) {
    vector<vector<int>> possibleMoves;

    vector<int> aux;

    for (int line = 0; line < currentBoard.size(); line++) {
        for (int col = 0; col < currentBoard.size(); col++) {
            // If the tile is empty, get all possible moves to there
            if (currentBoard[line][col].isEmpty()) {
               aux = {1, line, col};
               possibleMoves.push_back(aux);
               aux = {2, line, col};
               possibleMoves.push_back(aux);
               if (currentBoard.size() >= 5) {
                   aux = {3, line, col};
                   possibleMoves.push_back(aux);
               }
            } else if (currentBoard[line][col].getTop()->getColor() == player) {

                if (line == 0) {
                    if (!currentBoard[line+1][col].isEmpty()) {
                        aux = {line, col, line+1, col};
                        possibleMoves.push_back(aux);
                    }
                } else if (line == currentBoard.size()-1) {
                    if (!currentBoard[line-1][col].isEmpty()) {
                        aux = {line, col, line-1, col};
                        possibleMoves.push_back(aux);
                    }
                } else {
                    if (!currentBoard[line+1][col].isEmpty()) {
                        aux = {line, col, line+1, col};
                        possibleMoves.push_back(aux);
                    }
                    if (!currentBoard[line-1][col].isEmpty()) {
                        aux = {line, col, line-1, col};
                        possibleMoves.push_back(aux);
                    }
                }

                if (col == 0) {
                    if (!currentBoard[line][col+1].isEmpty()) {
                        aux = {line, col, line, col+1};
                        possibleMoves.push_back(aux);
                    }
                } else if (col == currentBoard.size()-1) {
                    if (!currentBoard[line][col-1].isEmpty()) {
                        aux = {line, col, line, col-1};
                        possibleMoves.push_back(aux);
                    }
                } else {
                    if (!currentBoard[line][col+1].isEmpty()) {
                        aux = {line, col, line, col+1};
                        possibleMoves.push_back(aux);
                    }
                    if (!currentBoard[line][col-1].isEmpty()) {
                        aux = {line, col, line, col-1};
                        possibleMoves.push_back(aux);
                    }
                }

            }

        }
    }

    return possibleMoves;
}

vector<vector<Tile>> Tak::simulateMakeMove(vector<vector<Tile>> board, vector<int> play) {
    if (play.size() == 4) {
        Stone toMove = board[play[0]][play[1]].pop();
        board[play[2]][play[3]].add2(toMove);
    } else if (play.size() == 3) {
        switch (play[0]) {
            case 1:
                this->currentPlayer==1?numFStonesP1--:numFStonesP2--;
                break;
            case 2:
                this->currentPlayer==1?numFStonesP1--:numFStonesP2--;
                break;
            case 3:
                this->currentPlayer==1?numCStonesP1--:numCStonesP2--;
                break;
            default:
                cout << "Error1!\n";
        }
        board[play[1]][play[2]].add(play[0], this->currentPlayer);
    } else {
        cout << "Error2!\n";
    }

    return board;
}

void Tak::makeMove(vector<int> play) {
    if (play.size() == 4) {
        vector<vector<Tile>> board = this->board.getTiles();
        Stone toMove = board[play[0]][play[1]].pop();
        board[play[2]][play[3]].add2(toMove);
        this->board.update(board);
    } else if (play.size() == 3) {
        vector<vector<Tile>> board = this->board.getTiles();
        switch (play[0]) {
            case 1:
                this->currentPlayer==1?numFStonesP1--:numFStonesP2--;
                break;
            case 2:
                this->currentPlayer==1?numFStonesP1--:numFStonesP2--;
                break;
            case 3:
                this->currentPlayer==1?numCStonesP1--:numCStonesP2--;
                break;
            default:
                cout << "Error4!\n";
        }
        board[play[1]][play[2]].add(play[0], this->currentPlayer);
        this->board.update(board);
    } else {
        cout << "Error5!\n";
    }
}

bool Tak::canPlay(vector<int> play) {
    if (play.size() == 4) { // Case when there's a move inside the board of just one piece
        if (!this->board.getTiles()[play[0]][play[1]].isEmpty())
            if(this->board.getTiles()[play[0]][play[1]].getTop()->getColor() != currentPlayer)
                return false;
        vector<int> origin; origin.push_back(play[0]); origin.push_back(play[1]);
        vector<int> dest; dest.push_back(play[2]); dest.push_back(play[3]);
        bool originEmpty = this->board.getTiles()[play[0]][play[1]].isEmpty();
        bool destEmpty = this->board.getTiles()[play[2]][play[3]].isEmpty();
        if(!destEmpty && !originEmpty && isAdjacent(origin, dest)){
            string destStone = this->board.getTiles()[play[2]][play[3]].getTop()->getSymbol();
            return destStone != "C";
        }
        return false;
    } else if (play.size() == 3) { // Case when a piece from outside the board is move to there
        int piece = play[0];
        if (piece == 1 || piece == 2) { // if is a flat or standing stone
            if ((this->currentPlayer == 1 && numFStonesP1 < 1) || (this->currentPlayer == 2 && numFStonesP2 < 1)) {
                cout << "Insuficient number of pieces\n";
                return false;
            }
            int line = play[1];
            int col = play[2];
            return this->board.getTiles()[line][col].isEmpty();
        } else { // if is a capstone
            if ((this->currentPlayer == 1 && numCStonesP1 < 1) || (this->currentPlayer == 2 && numCStonesP2 < 1)) {
                cout << "Insuficient number of pieces\n";
                return false;
            }
            return true;
        }
    } else {
        cout << "Error6\n";
        return false;
    }
}

bool Tak::isAdjacent(vector<int> origin, vector<int> dest) {
    int originCol = origin[1];
    int originLin = origin[0];
    int destCol = dest[1];
    int destLin = dest[0];

    if (originCol == destCol) return ((originLin == destLin + 1) || (originLin == destLin - 1));
    if (originLin == destLin) return ((originCol == destCol + 1) || (originCol == destCol - 1));
    return false;
}

bool Tak::endOfGame() {
    // true if the board is full, if at least one player built a path or if the player is out of pieces
    int numPiecesLeft;
    if (this->currentPlayer == 1) {
        numPiecesLeft = numCStonesP1 + numFStonesP1;
    } else {
        numPiecesLeft = numCStonesP2 + numFStonesP2;
    }
    return (this->board.allTilesFull() || isPathBuilt(this->board.getTiles()) || (numPiecesLeft == 0));
}

void Tak::getWinner() {
    // if there's just a path, wins that player
    // if there are two paths, one for each player, wins the last one to make a move
    // else wins the player with the most visible number of pieces
    cout << "\n\n";
    if(isPathBuilt(this->board.getTiles())){
        switch(currentPlayer){
            case 1:
                cout << "Player 1 won!";
                break;
            case 2:
                cout << "Player 2 won!";
                break;
            default:
                cout << "Error7\n";
        }
    }
    else {
        switch(visibleWinner()){
            case 1:
                cout << "Player 1 won!";
                break;
            case 2:
                cout << "Player 2 won!";
                break;
            default:
                cout << "Error8\n";
        }
    }
}

void Tak::drawHeader() {
    cout << "\n\n";
    switch (getMode()) {
        case 1:
            cout << "Player 1: Human;";
            cout << "\tPlayer 2: Human";
            break;
        case 2:
            cout << "Player 1: Human;";
            cout << "\tPlayer 2: Computer";
            cout << "\nDifficulty for Computer: " << getDifficulty("P2");
            break;
        case 3:
            cout << "Player 1: Computer;";
            cout << "\tPlayer 2: Computer";
            cout << "\nDifficulty for Player 1: " << getDifficulty("P1");
            cout << "\nDifficulty for Player 2: " << getDifficulty("P2");
            break;
        default:
            cout << "Error9!\n";
    }

    cout << "\nCurrent Player :" << ((this->currentPlayer==1)?"Player 1\n":"Player 2\n");
    cout << "\nS - Standing Stone; F - Flat Stone; C - Capstone\n";
    cout << "\n\n";
}

int Tak::getMode() {
    return this->mode;
}

void Tak::drawBoard() {
    this->board.draw();
}

vector<int> Tak::getPlay() {
    vector<int> play;

    cout << "\nPlease choose your move:\n";
    cout << "1. Move piece from the board\n";

    int sel = 0;

    bool hasPieces = this->currentPlayer==1?numFStonesP1+numCStonesP1>0:numFStonesP2+numCStonesP2>0;
    if (hasPieces) {
        cout << "2. Move piece from outside the board\n";
        while (sel != 1 && sel != 2) {
            cout << "\nInput: ";
            cin >> sel;
        }
    } else {
        while (sel != 1) {
            cout << "\nInput: ";
            cin >> sel;
        }
    }

    vector<int> origin, dest;
    int pieceToMove;

    switch (sel) {
        case 1:
            origin = getOriginTile();
            play.push_back(origin[0]);
            play.push_back(origin[1]);
            dest = getDestTile();
            play.push_back(dest[0]);
            play.push_back(dest[1]);
            break;
        case 2:
            pieceToMove = getPieceToMove();
            play.push_back(pieceToMove);
            dest = getDestTile();
            play.push_back(dest[0]);
            play.push_back(dest[1]);
            break;
        default:
            cout << "Error10!\n";
            break;
    }

    return play;
}

int Tak::getPieceToMove() {

    cout << "\nPIECE TO MOVE";

    int available;
    int sel = 0;

    if ((this->currentPlayer==1?numFStonesP1>0:numFStonesP2>0) && (this->currentPlayer==1?numCStonesP1>0:numCStonesP2>0)){
        available = this->currentPlayer==1?numFStonesP1:numFStonesP2;
        cout << "\n1. Flatstone (" << available << ")";
        cout << "\n2. Standingstone (" << available << ")";
        available = this->currentPlayer==1?numCStonesP1:numCStonesP2;
        cout << "\n3. Capstone (" << available << ")";
        while (sel != 1 && sel != 2 && sel != 3) {
            cout << "\nInput: ";
            cin >> sel;
        }
    } else if (this->currentPlayer==1?numCStonesP1>0:numCStonesP2>0) {
        available = this->currentPlayer==1?numCStonesP1:numCStonesP2;
        cout << "\n3. Capstone (" << available << ")";
        while (sel != 3) {
            cout << "\nInput: ";
            cin >> sel;
        }
    } else {
        available = this->currentPlayer==1?numFStonesP1:numFStonesP2;
        cout << "\n1. Flatstone (" << available << ")";
        cout << "\n2. Standingstone (" << available << ")";
        while (sel != 1 && sel != 2) {
            cout << "\nInput: ";
            cin >> sel;
        }
    }

    return sel;
}

vector<int> Tak::getDestTile() {
    vector<int> position;

    cout << "\nDESTINATION TILE";
    cout << "\nPlease choose line. Insert a value between 1 and " << this->board.getSize();

    int sel = 0;
    while (sel < 1 || sel > this->board.getSize()) {
        cout << "\nInput: ";
        cin >> sel;
    }
    sel--;
    position.push_back(sel);

    cout << "\nPlease choose column. Insert a value between 1 and " << this->board.getSize();

    sel = 0;
    while (sel < 1 || sel > this->board.getSize()) {
        cout << "\nInput: ";
        cin >> sel;
    }

    sel--;
    position.push_back(sel);

    return position;
}

vector<int> Tak::getOriginTile() {
    vector<int> position;

    cout << "\nORIGIN TILE";
    cout << "\nPlease choose line. Insert a value between 1 and " << this->board.getSize();

    int sel = 0;
    while (sel < 1 || sel > this->board.getSize()) {
        cout << "\nInput: ";
        cin >> sel;
    }
    sel--;
    position.push_back(sel);

    cout << "\nPlease choose column. Insert a value between 1 and " << this->board.getSize();

    sel = 0;
    while (sel < 1 || sel > this->board.getSize()) {
        cout << "\nInput: ";
        cin >> sel;
    }

    sel--;
    position.push_back(sel);

    return position;
}

void Tak::changePlayer() {
    (this->currentPlayer == 1)? this->currentPlayer = 2 : this->currentPlayer = 1;
}

string Tak::getDifficulty(string diff) {
    int d = (diff=="P1")?this->difficultyP1:this->difficultyP2;
    switch (d) {
        case 1:
            return "Easy";
        case 2:
            return "Normal";
        case 3:
            return "Hard";
        default:
            return "Error11!";
    }
}

bool Tak::pathVertical(vector<vector<Tile>> board) {

    for (int j = 0; j < this->board.getSize(); j++) {
        for (int k = 0; k < this->board.getSize(); k++) {
            board[j][k].resetVisited();
        }
    }

    bool hasPath = false;
    vector<vector<Tile>> tiles = board;

    for (int i = 0; i < board.size(); i++) { // Para cada elemento da primeira linha (topo do tabuleiro)
        if (tiles[0][i].isEmpty()) continue;
        else if (tiles[0][i].getTop()->getColor() == this->currentPlayer && !tiles[0][i].getTop()->isWall()) {
            hasPath = hasPath || getPathV(tiles, 0, i);
        }
    }

    return hasPath;
}

bool Tak::getPathV(vector<vector<Tile>> tiles, int line, int col) {
    if (line == this->board.getSize()-1) return true;

    tiles[line][col].setVisited();

    bool path1 = false, path2 = false, path3 = false, path4 = false;

    if (line == 0) {
        if (!tiles[line+1][col].isEmpty())
            if (tiles[line+1][col].getTop()->getColor() == this->currentPlayer && !tiles[line+1][col].getVisited() && !tiles[line+1][col].getTop()->isWall()) path1 = getPathV(tiles, line+1, col);
    } else if (line == this->board.getSize()-1) {
        if (!tiles[line-1][col].isEmpty())
            if (tiles[line-1][col].getTop()->getColor() == this->currentPlayer && !tiles[line-1][col].getVisited() && !tiles[line-1][col].getTop()->isWall()) path1 = getPathV(tiles, line-1, col);
    } else {
        if (!tiles[line+1][col].isEmpty())
            if (tiles[line+1][col].getTop()->getColor() == this->currentPlayer && !tiles[line+1][col].getVisited() && !tiles[line+1][col].getTop()->isWall()) path1 = getPathV(tiles, line+1, col);
        if (!tiles[line-1][col].isEmpty())
            if (tiles[line-1][col].getTop()->getColor() == this->currentPlayer && !tiles[line-1][col].getVisited() && !tiles[line-1][col].getTop()->isWall()) path2 = getPathV(tiles, line-1, col);

    }

    if (col == 0) {
        if (!tiles[line][col+1].isEmpty())
            if (tiles[line][col+1].getTop()->getColor() == this->currentPlayer && !tiles[line][col+1].getVisited() && !tiles[line][col+1].getTop()->isWall()) path3 = getPathV(tiles, line, col+1);
    } else if (col == this->board.getSize()-1) {
        if (!tiles[line][col-1].isEmpty())
            if (tiles[line][col-1].getTop()->getColor() == this->currentPlayer && !tiles[line][col-1].getVisited() && !tiles[line][col-1].getTop()->isWall()) path3 = getPathV(tiles, line, col-1);
    } else {
        if (!tiles[line][col+1].isEmpty())
            if (tiles[line][col+1].getTop()->getColor() == this->currentPlayer && !tiles[line][col+1].getVisited() && !tiles[line][col+1].getTop()->isWall()) path3 = getPathV(tiles, line, col+1);
        if (!tiles[line][col-1].isEmpty())
            if (tiles[line][col-1].getTop()->getColor() == this->currentPlayer && !tiles[line][col-1].getVisited() && !tiles[line][col-1].getTop()->isWall()) path4 = getPathV(tiles, line, col-1);
    }

    return path1 || path2 || path3 || path4;
}

bool Tak::getPathH(vector<vector<Tile>> tiles, int line, int col) {
    if (col == this->board.getSize()-1) return true;

    tiles[line][col].setVisited();

    bool path1 = false, path2 = false, path3 = false, path4 = false;

    if (line == 0) {
        if (!tiles[line+1][col].isEmpty())
            if (tiles[line+1][col].getTop()->getColor() == this->currentPlayer && !tiles[line+1][col].getVisited() && !tiles[line+1][col].getTop()->isWall()) path1 = getPathV(tiles, line+1, col);
    } else if (line == this->board.getSize()-1) {
        if (!tiles[line-1][col].isEmpty())
            if (tiles[line-1][col].getTop()->getColor() == this->currentPlayer && !tiles[line-1][col].getVisited() && !tiles[line-1][col].getTop()->isWall()) path1 = getPathV(tiles, line-1, col);
    } else {
        if (!tiles[line+1][col].isEmpty())
            if (tiles[line+1][col].getTop()->getColor() == this->currentPlayer && !tiles[line+1][col].getVisited() && !tiles[line+1][col].getTop()->isWall()) path1 = getPathV(tiles, line+1, col);
        if (!tiles[line-1][col].isEmpty())
            if (tiles[line-1][col].getTop()->getColor() == this->currentPlayer && !tiles[line-1][col].getVisited() && !tiles[line-1][col].getTop()->isWall()) path2 = getPathV(tiles, line-1, col);

    }

    if (col == 0) {
        if (!tiles[line][col+1].isEmpty())
            if (tiles[line][col+1].getTop()->getColor() == this->currentPlayer && !tiles[line][col+1].getVisited() && !tiles[line][col+1].getTop()->isWall()) path3 = getPathV(tiles, line, col+1);
    } else if (col == this->board.getSize()-1) {
        if (!tiles[line][col-1].isEmpty())
            if (tiles[line][col-1].getTop()->getColor() == this->currentPlayer && !tiles[line][col-1].getVisited() && !tiles[line][col-1].getTop()->isWall()) path3 = getPathV(tiles, line, col-1);
    } else {
        if (!tiles[line][col+1].isEmpty())
            if (tiles[line][col+1].getTop()->getColor() == this->currentPlayer && !tiles[line][col+1].getVisited() && !tiles[line][col+1].getTop()->isWall()) path3 = getPathV(tiles, line, col+1);
        if (!tiles[line][col-1].isEmpty())
            if (tiles[line][col-1].getTop()->getColor() == this->currentPlayer && !tiles[line][col-1].getVisited() && !tiles[line][col-1].getTop()->isWall()) path4 = getPathV(tiles, line, col-1);
    }

    return path1 || path2 || path3 || path4;
}

bool Tak::pathHorizontal(vector<vector<Tile>> board) {

    for (int j = 0; j < this->board.getSize(); j++) {
        for (int k = 0; k < this->board.getSize(); k++) {
            board[j][k].resetVisited();
        }
    }

    bool hasPath = false;
    vector<vector<Tile>> tiles = board;

    for (int i = 0; i < board.size(); i++) { // Para cada elemento da primeira linha (topo do tabuleiro)
        if (tiles[i][0].isEmpty()) continue;
        else if (tiles[i][0].getTop()->getColor() == this->currentPlayer && !tiles[i][0].getTop()->isWall()) {
            hasPath = hasPath || getPathH(tiles, i, 0);
        }
    }

    return hasPath;
}

int Tak::semiPathVertical(vector<vector<Tile>> board) {

    for (int j = 0; j < this->board.getSize(); j++) {
        for (int k = 0; k < this->board.getSize(); k++) {
            board[j][k].resetVisited();
        }
    }

    bool lenPath = 0;
    vector<vector<Tile>> tiles = board;

    for (int i = 0; i < board.size(); i++) { // Para cada elemento da primeira linha (topo do tabuleiro)
        if (tiles[0][i].isEmpty()) continue;
        else if (tiles[0][i].getTop()->getColor() == this->currentPlayer && !tiles[0][i].getTop()->isWall()) {
            int aux = getPathV2(tiles, 0, i);
            lenPath = lenPath>aux? lenPath : aux;
        }
    }

    return lenPath;
}

int Tak::getPathV2(vector<vector<Tile>> tiles, int line, int col) {
    tiles[line][col].setVisited();

    int path1 = 0, path2 = 0, path3 = 0, path4 = 0;

    if (line == 0) {
        if (!tiles[line+1][col].isEmpty())
            if (tiles[line+1][col].getTop()->getColor() == this->currentPlayer && !tiles[line+1][col].getVisited() && !tiles[line+1][col].getTop()->isWall()) path1 += getPathV(tiles, line+1, col);
    } else if (line == this->board.getSize()-1) {
        if (!tiles[line-1][col].isEmpty())
            if (tiles[line-1][col].getTop()->getColor() == this->currentPlayer && !tiles[line-1][col].getVisited() && !tiles[line-1][col].getTop()->isWall()) path1 += getPathV(tiles, line-1, col);
    } else {
        if (!tiles[line+1][col].isEmpty())
            if (tiles[line+1][col].getTop()->getColor() == this->currentPlayer && !tiles[line+1][col].getVisited() && !tiles[line+1][col].getTop()->isWall()) path1 += getPathV(tiles, line+1, col);
        if (!tiles[line-1][col].isEmpty())
            if (tiles[line-1][col].getTop()->getColor() == this->currentPlayer && !tiles[line-1][col].getVisited() && !tiles[line-1][col].getTop()->isWall()) path2 += getPathV(tiles, line-1, col);

    }

    if (col == 0) {
        if (!tiles[line][col+1].isEmpty())
            if (tiles[line][col+1].getTop()->getColor() == this->currentPlayer && !tiles[line][col+1].getVisited() && !tiles[line][col+1].getTop()->isWall()) path3 += getPathV(tiles, line, col+1);
    } else if (col == this->board.getSize()-1) {
        if (!tiles[line][col-1].isEmpty())
            if (tiles[line][col-1].getTop()->getColor() == this->currentPlayer && !tiles[line][col-1].getVisited() && !tiles[line][col-1].getTop()->isWall()) path3 += getPathV(tiles, line, col-1);
    } else {
        if (!tiles[line][col+1].isEmpty())
            if (tiles[line][col+1].getTop()->getColor() == this->currentPlayer && !tiles[line][col+1].getVisited() && !tiles[line][col+1].getTop()->isWall()) path3 += getPathV(tiles, line, col+1);
        if (!tiles[line][col-1].isEmpty())
            if (tiles[line][col-1].getTop()->getColor() == this->currentPlayer && !tiles[line][col-1].getVisited() && !tiles[line][col-1].getTop()->isWall()) path4 += getPathV(tiles, line, col-1);
    }

    return max({path1, path2, path3, path4});
}

int Tak::getPathH2(vector<vector<Tile>> tiles, int line, int col) {
    tiles[line][col].setVisited();

    int path1 = 0, path2 = 0, path3 = 0, path4 = 0;

    if (line == 0) {
        if (!tiles[line+1][col].isEmpty())
            if (tiles[line+1][col].getTop()->getColor() == this->currentPlayer && !tiles[line+1][col].getVisited() && !tiles[line+1][col].getTop()->isWall()) path1 += getPathV(tiles, line+1, col);
    } else if (line == this->board.getSize()-1) {
        if (!tiles[line-1][col].isEmpty())
            if (tiles[line-1][col].getTop()->getColor() == this->currentPlayer && !tiles[line-1][col].getVisited() && !tiles[line-1][col].getTop()->isWall()) path1 += getPathV(tiles, line-1, col);
    } else {
        if (!tiles[line+1][col].isEmpty())
            if (tiles[line+1][col].getTop()->getColor() == this->currentPlayer && !tiles[line+1][col].getVisited() && !tiles[line+1][col].getTop()->isWall()) path1 += getPathV(tiles, line+1, col);
        if (!tiles[line-1][col].isEmpty())
            if (tiles[line-1][col].getTop()->getColor() == this->currentPlayer && !tiles[line-1][col].getVisited() && !tiles[line-1][col].getTop()->isWall()) path2 += getPathV(tiles, line-1, col);

    }

    if (col == 0) {
        if (!tiles[line][col+1].isEmpty())
            if (tiles[line][col+1].getTop()->getColor() == this->currentPlayer && !tiles[line][col+1].getVisited() && !tiles[line][col+1].getTop()->isWall()) path3 += getPathV(tiles, line, col+1);
    } else if (col == this->board.getSize()-1) {
        if (!tiles[line][col-1].isEmpty())
            if (tiles[line][col-1].getTop()->getColor() == this->currentPlayer && !tiles[line][col-1].getVisited() && !tiles[line][col-1].getTop()->isWall()) path3 += getPathV(tiles, line, col-1);
    } else {
        if (!tiles[line][col+1].isEmpty())
            if (tiles[line][col+1].getTop()->getColor() == this->currentPlayer && !tiles[line][col+1].getVisited() && !tiles[line][col+1].getTop()->isWall()) path3 += getPathV(tiles, line, col+1);
        if (!tiles[line][col-1].isEmpty())
            if (tiles[line][col-1].getTop()->getColor() == this->currentPlayer && !tiles[line][col-1].getVisited() && !tiles[line][col-1].getTop()->isWall()) path4 += getPathV(tiles, line, col-1);
    }

    return max({path1, path2, path3, path4});
}

int Tak::semiPathHorizontal(vector<vector<Tile>> board) {

    for (int j = 0; j < this->board.getSize(); j++) {
        for (int k = 0; k < this->board.getSize(); k++) {
            board[j][k].resetVisited();
        }
    }

    int lenPath = 0;
    vector<vector<Tile>> tiles = board;

    for (int i = 0; i < board.size(); i++) { // Para cada elemento da primeira linha (topo do tabuleiro)
        if (tiles[i][0].isEmpty()) continue;
        else if (tiles[i][0].getTop()->getColor() == this->currentPlayer && !tiles[i][0].getTop()->isWall()) {
            int aux = getPathH2(tiles, i, 0);
            lenPath = lenPath>aux? lenPath : aux;
        }
    }

    return lenPath;
}

bool Tak::isPathBuilt(vector<vector<Tile>> board) {
    return pathVertical(board) || pathHorizontal(board);
}

int Tak::lenghtSemiPathBuilt(vector<vector<Tile>> board) {
    return semiPathVertical(board) + semiPathHorizontal(board);
}

int Tak::visibleWinner() {
    int playerOnePieces = 0;
    int playerTwoPieces = 0;
    for(int i=0; i<this->board.getSize(); i++){
        for(int j=0; j<this->board.getSize(); j++){
            if(!this->board.getTiles()[i][j].isEmpty()){
                switch(this->board.getTiles()[i][j].getTop()->getColor()){
                    case 1:
                        playerOnePieces++;
                        break;
                    case 2:
                        playerTwoPieces++;
                        break;
                    default:
                        cout << "Error12!\n";
                }
            }
        }
    }
    return (playerOnePieces > playerTwoPieces)? 1 : 2;
}

int Tak::getNumFStones(int player) {
    return player==1?numFStonesP1:numFStonesP2;
}

int Tak::getNumCStones(int player) {
    return player==1?numCStonesP1:numCStonesP2;
}
