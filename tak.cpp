#include "tak.h"
#include <string>

Tak::Tak(int mode, int difficultyP1, int difficultyP2) {
    this->mode = mode;
    this->difficultyP1 = difficultyP1;
    this->difficultyP2 = difficultyP2;
    this->currentPlayer = 2;

    // Considering a 5x5 board
    // To Do Later on: generate dynamically the number
    // of pieces and size of the board using user's input
    this->numFStonesP1 = 21;
    this->numFStonesP2 = 21;
    this->numCStonesP1 = 1;
    this->numCStonesP2 = 1;
    this->board = Board(5, 5);
}

void Tak::gameCycle() {
    if (endOfGame()) {
        getWinner();
        return ;
    }
    changePlayer();

    drawHeader();
    drawBoard();

    vector<int> play = getPlay();

    while (!canPlay(play)) {
        play = getPlay();
    }

    makeMove(play);
    gameCycle();
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
        board[play[2]][play[3]].add2(toMove);
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
                cout << "Error!\n";
        }
        board[play[1]][play[2]].add(play[0], this->currentPlayer);
        this->board.update(board);
    } else {
        cout << "Error!\n";
    }
}


bool Tak::isPlayable(vector<int> origin, vector<int> dest, int numOfPieces) {
    if(origin[1] != dest[1] && origin[0] != dest[0]){return false;}
    Tile aux = this->board.getTiles()[origin[1]][origin[0]];
    if(aux.getSize() < numOfPieces){ return false;}
    stack<Stone> pieces = aux.getStackMove(numOfPieces);
    for(int i=numOfPieces; i>=0; i--){
        if(origin[1] == dest[1]){
            if(this->board.getTiles()[dest[1]][dest[0]-i].getTop().toString() == "C"){
                return false;
            }
            if(this->board.getTiles()[dest[1]][dest[0]-i].getTop().isWall()){
                if(pieces.top().toString() != "C"){
                    return false;
                }
            }
            pieces.pop();

        }
        if(origin[0] == dest[0]){
            if(this->board.getTiles()[dest[1]-i][dest[0]].getTop().toString() == "C"){
                return false;
            }
            if(this->board.getTiles()[dest[1]-i][dest[0]].getTop().isWall()){
                if(pieces.top().toString() != "C"){
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
        if(this->board.getTiles()[play[1]][play[0]].getTop().getColor() != currentPlayer){
            return false;
        }
        vector<int> origin; origin.push_back(play[0]); origin.push_back(play[1]);
        vector<int> dest; dest.push_back(play[2]); dest.push_back(play[3]);
        stack<Stone> pieces;

        bool originEmpty = this->board.getTiles()[play[1]][play[0]].isEmpty();

        if(!originEmpty){
            if(isPlayable(origin, dest, play[4])){
                return true;
            }
        }
        return (isAdjacent(origin, dest) && !originEmpty);
    }
    else if (play.size() == 4) { // Case when there's a move inside the board of just one piece
        if(this->board.getTiles()[play[1]][play[0]].getTop().getColor() != currentPlayer){
            return false;
        }
        vector<int> origin; origin.push_back(play[0]); origin.push_back(play[1]);
        vector<int> dest; dest.push_back(play[2]); dest.push_back(play[3]);
        bool originEmpty = this->board.getTiles()[play[0]][play[1]].isEmpty();
        bool destEmpty = this->board.getTiles()[play[2]][play[3]].isEmpty();
        if(!destEmpty && !originEmpty){
            string destStone = this->board.getTiles()[play[2]][play[3]].getTop()->getSymbol();
            if(destStone == "C"){
                return false;
            }
            else{return true;}
        }

        return (isAdjacent(origin, dest) && !originEmpty && !destEmpty);

        // Still to do: verify is the destination tile has a standing stone
        // or a capstone, which might turn the movement into an invalid one
        // depending on the piece to move

        // Also to do: check if the piece to move is the same color as the current player
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
        cout << "Error\n";
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
    return (this->board.allTilesFull() || isPathBuilt() || (numPiecesLeft == 0));
}

void Tak::getWinner() {
    // if there's just a path, wins that player
    // if there are two paths, one for each player, wins the last one to make a move
    // else wins the player with the most visible number of pieces
    if(isPathBuilt()){
        switch(currentPlayer){
            case 1:
                cout << "Player 1 won!";
                break;
            case 2:
                cout << "Player 2 won!";
                break;
            default:
                cout << "Error\n";
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
                cout << "Error\n";
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
            cout << "\nDifficulty for Computer: " << getDifficulty("P1");
            break;
        case 3:
            cout << "Player 1: Computer;";
            cout << "\tPlayer 2: Computer";
            cout << "\nDifficulty for Player 1: " << getDifficulty("P1");
            cout << "\nDifficulty for Player 2: " << getDifficulty("P2");
            break;
        default:
            cout << "Error!\n";
    }

    cout << "\nS - Standing Stone; F - Flat Stone; C - Capstone\n";
    cout << "\n\n";
}

int Tak::getMode() {
    return this->mode;
}

void Tak::drawBoard() {
    this->board.draw(this->currentPlayer);
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
            if(this->board.getTiles()[play[1]][play[0]].getSize() > 1){
                int pieces = 0;
                cout << "Please choose how many pieces to move:\n";
                cout << "Input: ";
                pieces = 0;
                while(pieces < 1 || pieces > this->board.getTiles()[play[1]][play[0]].getSize()){
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
            cout << "Error!\n";
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
            return "Error!";
    }
}

bool Tak::pathVertical() {

    for (int j = 0; j < this->board.getSize(); j++) {
        for (int k = 0; k < this->board.getSize(); k++) {
            this->board.getTiles()[j][k].resetVisited();
        }
    }

    bool hasPath = false;
    vector<vector<Tile>> tiles = this->board.getTiles();

    for (int i = 0; i < this->board.getSize(); i++) { // Para cada elemento da primeira linha (topo do tabuleiro)
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

bool Tak::pathHorizontal() {

    for (int j = 0; j < this->board.getSize(); j++) {
        for (int k = 0; k < this->board.getSize(); k++) {
            this->board.getTiles()[j][k].resetVisited();
        }
    }

    bool hasPath = false;
    vector<vector<Tile>> tiles = this->board.getTiles();

    for (int i = 0; i < this->board.getSize(); i++) { // Para cada elemento da primeira linha (topo do tabuleiro)
        if (tiles[i][0].isEmpty()) continue;
        else if (tiles[i][0].getTop()->getColor() == this->currentPlayer) {
            hasPath = hasPath || getPathH(tiles, i, 0);
        }
    }

    return hasPath;
}

bool Tak::isPathBuilt() {
    return pathVertical() || pathHorizontal();
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
                        cout << "Error\n!";
                }
            }
        }
    }
    return (playerOnePieces > playerTwoPieces)? 1 : 2;
}