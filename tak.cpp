#include "tak.h"
#include <string>

Tak::Tak(int mode, int difficultyP1, int difficultyP2, int size) {
    this->mode = mode;
    this->difficultyP1 = difficultyP1;
    this->difficultyP2 = difficultyP2;
    this->currentPlayer = 2;

    this->numFStonesP1 = 21;
    this->numFStonesP2 = 21;
    this->numCStonesP1 = 1;
    this->numCStonesP2 = 1;
    this->board = Board(size, size);
}

void Tak::gameCycle() {
    if (endOfGame()) {
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
        vector<vector<int>> allMoves = getAllPossibleMoves(this->board.getTiles(), this->currentPlayer);
        play = minimax(allMoves);

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

    int option = 0;
    while (option != 1 && option != 2) {
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
    } else {
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

    int piecesC = 1, piecesF = 1; // To Do - get real number of pieces for each type
    vector<int> aux;

    for (int line = 0; line < currentBoard.size(); line++) {
        for (int col = 0; col < currentBoard.size(); col++) {
            // If the tile is empty, get all possible moves to there
            if (currentBoard[line][col].isEmpty()) {
               if (piecesC > 0) {
                   aux = {1, line, col};
                   possibleMoves.push_back(aux);
                   aux = {2, line, col};
                   possibleMoves.push_back(aux);
               }
               if (piecesF > 0) {
                   aux = {3, line, col};
                   possibleMoves.push_back(aux);
               }
            }
            // To Do - get all the other moves, i.e., all the stack moves
        }
    }

    return possibleMoves;
}

vector<vector<Tile>> Tak::simulateMakeMove(vector<vector<Tile>> board, vector<int> play) {
    if(play.size() == 5){
        stack<Stone> aux = board[play[1]][play[0]].getStackMove(play[4]);
        for(int i=play[4]-1; i>=0; i--){
            if(play[3] == play[1]){
                board[play[3]][play[2]-i].add2(aux.top());
                aux.pop();
            }
            if(play[2] == play[0]){
                board[play[3]-i][play[2]].add2(aux.top());
                aux.pop();
            }
        }
    }
    else if (play.size() == 4) {
        Stone toMove = board[play[0]][play[1]].pop();
        board[play[2]][play[3]].add2(toMove);
    } else if (play.size() == 3) {
        switch (play[0]) {
            case 1:
                this->currentPlayer==1?this->numFStonesP1--:this->numFStonesP2--;
                break;
            case 2:
                this->currentPlayer==1?this->numFStonesP1--:this->numFStonesP2--;
                break;
            case 3:
                this->currentPlayer==1?this->numCStonesP1--:this->numCStonesP2--;
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
    if(play.size() == 5){
        vector<vector<Tile>> board = this->board.getTiles();
        stack<Stone> aux = board[play[1]][play[0]].getStackMove(play[4]);
        for(int i=play[4]-1; i>=0; i--){
            if(play[3] == play[1]){
                board[play[3]][play[2]-i].add2(aux.top());
                aux.pop();
            }
            if(play[2] == play[0]){
                board[play[3]-i][play[2]].add2(aux.top());
                aux.pop();
            }
        }
        this->board.update(board);
    }
    else if (play.size() == 4) {
        vector<vector<Tile>> board = this->board.getTiles();
        Stone toMove = board[play[0]][play[1]].pop();
        cout << "After pop";
        board[play[2]][play[3]].add2(toMove);
        cout << "After add2";
        this->board.update(board);
    } else if (play.size() == 3) {
        vector<vector<Tile>> board = this->board.getTiles();
        switch (play[0]) {
            case 1:
                this->currentPlayer==1?this->numFStonesP1--:this->numFStonesP2--;
                break;
            case 2:
                this->currentPlayer==1?this->numFStonesP1--:this->numFStonesP2--;
                break;
            case 3:
                this->currentPlayer==1?this->numCStonesP1--:this->numCStonesP2--;
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

bool Tak::isPlayable(vector<int> origin, vector<int> dest, int numOfPieces) {
    if(origin[1] != dest[1] && origin[0] != dest[0]){return false;}
    Tile aux = this->board.getTiles()[origin[1]][origin[0]];
    if(aux.getSize() < numOfPieces){ return false;}
    stack<Stone> pieces = aux.getStackMove(numOfPieces);
    for(int i=numOfPieces; i>=0; i--){
        if(origin[1] == dest[1]){
            if(this->board.getTiles()[dest[1]][dest[0]-i].getTop()->getSymbol() == "C"){
                return false;
            }
            if(this->board.getTiles()[dest[1]][dest[0]-i].getTop()->isWall()){
                if(pieces.top().getSymbol() != "C"){
                    return false;
                }
            }
            pieces.pop();

        }
        if(origin[0] == dest[0]){
            if(this->board.getTiles()[dest[1]-i][dest[0]].getTop()->getSymbol() == "C"){
                return false;
            }
            if(this->board.getTiles()[dest[1]-i][dest[0]].getTop()->isWall()){
                if(pieces.top().getSymbol() != "C"){
                    return false;
                }
            }
            pieces.pop();
        }
    }
    return true;
}

bool Tak::canPlay(vector<int> play) {
    if(play.size() == 5){ // Case when there's a move inside the board with two or more pieces
        if (!this->board.getTiles()[play[0]][play[1]].isEmpty())
            if (this->board.getTiles()[play[0]][play[1]].getTop()->getColor() != currentPlayer)
                return false;
        vector<int> origin; origin.push_back(play[0]); origin.push_back(play[1]);
        vector<int> dest; dest.push_back(play[2]); dest.push_back(play[3]);
        stack<Stone> pieces;

        bool originEmpty = this->board.getTiles()[play[0]][play[1]].isEmpty();

        if(!originEmpty){
            if(isPlayable(origin, dest, play[4])){
                return true;
            }
        }
        return (isAdjacent(origin, dest) && !originEmpty);
    }
    else if (play.size() == 4) { // Case when there's a move inside the board of just one piece
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
            if ((this->currentPlayer == 1 && this->numFStonesP1 < 1) || (this->currentPlayer == 2 && this->numFStonesP2 < 1)) {
                cout << "Insuficient number of pieces\n";
                return false;
            }
            int line = play[1];
            int col = play[2];
            return this->board.getTiles()[line][col].isEmpty();
        } else { // if is a capstone
            if ((this->currentPlayer == 1 && this->numCStonesP1 < 1) || (this->currentPlayer == 2 && this->numCStonesP2 < 1)) {
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
        numPiecesLeft = this->numCStonesP1 + this->numFStonesP1;
    } else {
        numPiecesLeft = this->numCStonesP2 + this->numFStonesP2;
    }
    return (this->board.allTilesFull() || isPathBuilt(this->board.getTiles()) || (numPiecesLeft == 0));
}

void Tak::getWinner() {
    // if there's just a path, wins that player
    // if there are two paths, one for each player, wins the last one to make a move
    // else wins the player with the most visible number of pieces
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

    bool hasPieces = this->currentPlayer==1?this->numFStonesP1+this->numCStonesP1>0:this->numFStonesP2+this->numCStonesP2>0;
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
            if(this->board.getTiles()[play[0]][play[1]].getSize() > 1){
                int pieces = 0;
                cout << "Please choose how many pieces to move:\n";
                cout << "Input: ";
                pieces = 0;
                while(pieces < 1 || pieces > this->board.getTiles()[play[0]][play[1]].getSize()){
                    cout << "\nInput: ";
                    cin >> pieces;
                }
                dest = getDestTile();
                play.push_back(dest[0]);
                play.push_back(dest[1]);
                play.push_back(pieces);
                break;
            }
            else {
                dest = getDestTile();
                play.push_back(dest[0]);
                play.push_back(dest[1]);
                break;
            }
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

    if ((this->currentPlayer==1?this->numFStonesP1>0:this->numFStonesP2>0) && (this->currentPlayer==1?this->numCStonesP1>0:this->numCStonesP2>0)){
        available = this->currentPlayer==1?this->numFStonesP1:this->numFStonesP2;
        cout << "\n1. Flatstone (" << available << ")";
        cout << "\n2. Standingstone (" << available << ")";
        available = this->currentPlayer==1?this->numCStonesP1:this->numCStonesP2;
        cout << "\n3. Capstone (" << available << ")";
        while (sel != 1 && sel != 2 && sel != 3) {
            cout << "\nInput: ";
            cin >> sel;
        }
    } else if (this->currentPlayer==1?this->numCStonesP1>0:this->numCStonesP2>0) {
        available = this->currentPlayer==1?this->numCStonesP1:this->numCStonesP2;
        cout << "\n3. Capstone (" << available << ")";
        while (sel != 3) {
            cout << "\nInput: ";
            cin >> sel;
        }
    } else {
        available = this->currentPlayer==1?this->numFStonesP1:this->numFStonesP2;
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
        else if (tiles[0][i].getTop()->getColor() == this->currentPlayer) {
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
            if (tiles[line+1][col].getTop()->getColor() == this->currentPlayer && !tiles[line+1][col].getVisited()) path1 = getPathV(tiles, line+1, col);
    } else if (line == this->board.getSize()-1) {
        if (!tiles[line-1][col].isEmpty())
            if (tiles[line-1][col].getTop()->getColor() == this->currentPlayer && !tiles[line-1][col].getVisited()) path1 = getPathV(tiles, line-1, col);
    } else {
        if (!tiles[line+1][col].isEmpty())
            if (tiles[line+1][col].getTop()->getColor() == this->currentPlayer && !tiles[line+1][col].getVisited()) path1 = getPathV(tiles, line+1, col);
        if (!tiles[line-1][col].isEmpty())
            if (tiles[line-1][col].getTop()->getColor() == this->currentPlayer && !tiles[line-1][col].getVisited()) path2 = getPathV(tiles, line-1, col);

    }

    if (col == 0) {
        if (!tiles[line][col+1].isEmpty())
            if (tiles[line][col+1].getTop()->getColor() == this->currentPlayer && !tiles[line][col+1].getVisited()) path3 = getPathV(tiles, line, col+1);
    } else if (col == this->board.getSize()-1) {
        if (!tiles[line][col-1].isEmpty())
            if (tiles[line][col-1].getTop()->getColor() == this->currentPlayer && !tiles[line][col-1].getVisited()) path3 = getPathV(tiles, line, col-1);
    } else {
        if (!tiles[line][col+1].isEmpty())
            if (tiles[line][col+1].getTop()->getColor() == this->currentPlayer && !tiles[line][col+1].getVisited()) path3 = getPathV(tiles, line, col+1);
        if (!tiles[line][col-1].isEmpty())
            if (tiles[line][col-1].getTop()->getColor() == this->currentPlayer && !tiles[line][col-1].getVisited()) path4 = getPathV(tiles, line, col-1);
    }

    return path1 || path2 || path3 || path4;
}

bool Tak::getPathH(vector<vector<Tile>> tiles, int line, int col) {
    if (col == this->board.getSize()-1) return true;

    tiles[line][col].setVisited();

    bool path1 = false, path2 = false, path3 = false, path4 = false;

    if (line == 0) {
        if (!tiles[line+1][col].isEmpty())
            if (tiles[line+1][col].getTop()->getColor() == this->currentPlayer && !tiles[line+1][col].getVisited()) path1 = getPathV(tiles, line+1, col);
    } else if (line == this->board.getSize()-1) {
        if (!tiles[line-1][col].isEmpty())
            if (tiles[line-1][col].getTop()->getColor() == this->currentPlayer && !tiles[line-1][col].getVisited()) path1 = getPathV(tiles, line-1, col);
    } else {
        if (!tiles[line+1][col].isEmpty())
            if (tiles[line+1][col].getTop()->getColor() == this->currentPlayer && !tiles[line+1][col].getVisited()) path1 = getPathV(tiles, line+1, col);
        if (!tiles[line-1][col].isEmpty())
            if (tiles[line-1][col].getTop()->getColor() == this->currentPlayer && !tiles[line-1][col].getVisited()) path2 = getPathV(tiles, line-1, col);

    }

    if (col == 0) {
        if (!tiles[line][col+1].isEmpty())
            if (tiles[line][col+1].getTop()->getColor() == this->currentPlayer && !tiles[line][col+1].getVisited()) path3 = getPathV(tiles, line, col+1);
    } else if (col == this->board.getSize()-1) {
        if (!tiles[line][col-1].isEmpty())
            if (tiles[line][col-1].getTop()->getColor() == this->currentPlayer && !tiles[line][col-1].getVisited()) path3 = getPathV(tiles, line, col-1);
    } else {
        if (!tiles[line][col+1].isEmpty())
            if (tiles[line][col+1].getTop()->getColor() == this->currentPlayer && !tiles[line][col+1].getVisited()) path3 = getPathV(tiles, line, col+1);
        if (!tiles[line][col-1].isEmpty())
            if (tiles[line][col-1].getTop()->getColor() == this->currentPlayer && !tiles[line][col-1].getVisited()) path4 = getPathV(tiles, line, col-1);
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
        else if (tiles[i][0].getTop()->getColor() == this->currentPlayer) {
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
        else if (tiles[0][i].getTop()->getColor() == this->currentPlayer) {
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
            if (tiles[line+1][col].getTop()->getColor() == this->currentPlayer && !tiles[line+1][col].getVisited()) path1 += getPathV(tiles, line+1, col);
    } else if (line == this->board.getSize()-1) {
        if (!tiles[line-1][col].isEmpty())
            if (tiles[line-1][col].getTop()->getColor() == this->currentPlayer && !tiles[line-1][col].getVisited()) path1 += getPathV(tiles, line-1, col);
    } else {
        if (!tiles[line+1][col].isEmpty())
            if (tiles[line+1][col].getTop()->getColor() == this->currentPlayer && !tiles[line+1][col].getVisited()) path1 += getPathV(tiles, line+1, col);
        if (!tiles[line-1][col].isEmpty())
            if (tiles[line-1][col].getTop()->getColor() == this->currentPlayer && !tiles[line-1][col].getVisited()) path2 += getPathV(tiles, line-1, col);

    }

    if (col == 0) {
        if (!tiles[line][col+1].isEmpty())
            if (tiles[line][col+1].getTop()->getColor() == this->currentPlayer && !tiles[line][col+1].getVisited()) path3 += getPathV(tiles, line, col+1);
    } else if (col == this->board.getSize()-1) {
        if (!tiles[line][col-1].isEmpty())
            if (tiles[line][col-1].getTop()->getColor() == this->currentPlayer && !tiles[line][col-1].getVisited()) path3 += getPathV(tiles, line, col-1);
    } else {
        if (!tiles[line][col+1].isEmpty())
            if (tiles[line][col+1].getTop()->getColor() == this->currentPlayer && !tiles[line][col+1].getVisited()) path3 += getPathV(tiles, line, col+1);
        if (!tiles[line][col-1].isEmpty())
            if (tiles[line][col-1].getTop()->getColor() == this->currentPlayer && !tiles[line][col-1].getVisited()) path4 += getPathV(tiles, line, col-1);
    }

    return max({path1, path2, path3, path4});
}

int Tak::getPathH2(vector<vector<Tile>> tiles, int line, int col) {
    tiles[line][col].setVisited();

    int path1 = 0, path2 = 0, path3 = 0, path4 = 0;

    if (line == 0) {
        if (!tiles[line+1][col].isEmpty())
            if (tiles[line+1][col].getTop()->getColor() == this->currentPlayer && !tiles[line+1][col].getVisited()) path1 += getPathV(tiles, line+1, col);
    } else if (line == this->board.getSize()-1) {
        if (!tiles[line-1][col].isEmpty())
            if (tiles[line-1][col].getTop()->getColor() == this->currentPlayer && !tiles[line-1][col].getVisited()) path1 += getPathV(tiles, line-1, col);
    } else {
        if (!tiles[line+1][col].isEmpty())
            if (tiles[line+1][col].getTop()->getColor() == this->currentPlayer && !tiles[line+1][col].getVisited()) path1 += getPathV(tiles, line+1, col);
        if (!tiles[line-1][col].isEmpty())
            if (tiles[line-1][col].getTop()->getColor() == this->currentPlayer && !tiles[line-1][col].getVisited()) path2 += getPathV(tiles, line-1, col);

    }

    if (col == 0) {
        if (!tiles[line][col+1].isEmpty())
            if (tiles[line][col+1].getTop()->getColor() == this->currentPlayer && !tiles[line][col+1].getVisited()) path3 += getPathV(tiles, line, col+1);
    } else if (col == this->board.getSize()-1) {
        if (!tiles[line][col-1].isEmpty())
            if (tiles[line][col-1].getTop()->getColor() == this->currentPlayer && !tiles[line][col-1].getVisited()) path3 += getPathV(tiles, line, col-1);
    } else {
        if (!tiles[line][col+1].isEmpty())
            if (tiles[line][col+1].getTop()->getColor() == this->currentPlayer && !tiles[line][col+1].getVisited()) path3 += getPathV(tiles, line, col+1);
        if (!tiles[line][col-1].isEmpty())
            if (tiles[line][col-1].getTop()->getColor() == this->currentPlayer && !tiles[line][col-1].getVisited()) path4 += getPathV(tiles, line, col-1);
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
        else if (tiles[i][0].getTop()->getColor() == this->currentPlayer) {
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
    return player==1?this->numFStonesP1:this->numFStonesP2;
}

int Tak::getNumCStones(int player) {
    return player==1?this->numCStonesP1:this->numCStonesP2;
}
