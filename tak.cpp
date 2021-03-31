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
    if (play.size() == 4) {
        vector<vector<Tile>> board = this->board.getTiles();
        Stone toMove = board[play[1]][play[0]].pop();
        board[play[3]][play[2]].add2(toMove);
        this->board.update(board);
    } else if (play.size() == 3) {
        vector<vector<Tile>> board = this->board.getTiles();
        board[play[2]][play[1]].add(play[0], this->currentPlayer);
        this->board.update(board);
    } else {
        cout << "Error!\n";
    }
}

// Still to do - move an entire stack or part of it
bool Tak::canPlay(vector<int> play) {
    if (play.size() == 4) { // Case when there's a move inside the board of just one piece
        if(this->board.getTiles()[play[1]][play[0]].getTop().getColor() != currentPlayer){
            return false;
        }
        vector<int> origin; origin.push_back(play[0]); origin.push_back(play[1]);
        vector<int> dest; dest.push_back(play[2]); dest.push_back(play[3]);
        bool originEmpty = this->board.getTiles()[play[1]][play[0]].isEmpty();
        bool destEmpty = this->board.getTiles()[play[3]][play[2]].isEmpty();
        if(!destEmpty && !originEmpty){
            string destStone = this->board.getTiles()[play[3]][play[2]].getTop().toString();
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
            int origin = play[1];
            int dest = play[2];
            if (!this->board.getTiles()[dest][origin].isEmpty()) return false;
            return true;
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
    // true if the board is full or if at least one player built a path
    if(this->board.allTilesFull() || this->board.isPathBuilt()){ // need to test if both functions are functional
        return true;
    }
    return false;
}

void Tak::getWinner() {
    // first, check if there's a path for one player
    // if there's one path for each one, wins the last player to make a move
    // else check which player has the most visible pieces
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
    if(this->board.allTilesFull()){
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

    cout << "Please choose your move:\n";
    cout << "1. Move piece from the board\n";
    cout << "2. Move piece from outside the board\n";

    int sel = 0;
    while (sel != 1 && sel != 2) {
        cout << "\nInput: ";
        cin >> sel;
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
            cout << "Error!\n";
            break;

    }

    return play;
}

int Tak::getPieceToMove() {

    cout << "\nPIECE TO MOVE";
    cout << "\n1. Flatstone";
    cout << "\n2. Standingstone";
    cout << "\n3. Capstone";

    int sel = 0;
    while (sel != 1 && sel != 2 && sel != 3) {
        cout << "\nInput: ";
        cin >> sel;
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
    position.push_back(sel);

    cout << "\nPlease choose column. Insert a value between 1 and " << this->board.getSize();

    sel = 0;
    while (sel < 1 || sel > this->board.getSize()) {
        cout << "\nInput: ";
        cin >> sel;
    }
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
    position.push_back(sel);

    cout << "\nPlease choose column. Insert a value between 1 and " << this->board.getSize();

    sel = 0;
    while (sel < 1 || sel > this->board.getSize()) {
        cout << "\nInput: ";
        cin >> sel;
    }
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
    int currentY = 0;
    for (int j = 0; j < this->board.getSize(); j++) {
        for (int i = 0; i < this->board.getSize(); i++) {
            if(!this->board.getTiles()[i][j].isEmpty()){
                if(this->board.getTiles()[i][j].getTop().getColor() == currentPlayer){ currentY = j;}
                if(currentY == 5){return true;}
            }
        }
    }
    return false;
}

bool Tak::pathHorizontal() {
    int currentX = 0;
    for (int i = 0; i < this->board.getSize(); i++) {
        for (int j = 0; j < this->board.getSize(); j++) {
            if(!this->board.getTiles()[i][j].isEmpty()){
                if(this->board.getTiles()[i][j].getTop().getColor() == currentPlayer){ currentX = i;}
                if(currentX == 5){return true;}
            }
        }
    }
    return false;
}

bool Tak::isPathBuilt() {
    return(pathVertical() || pathHorizontal());
}

int Tak::visibleWinner() {
    int playerOnePieces = 0;
    int playerTwoPieces = 0;
    for(int i=0; i<this->board.getSize(); i++){
        for(int j=0; j<this->board.getSize(); j++){
            if(!this->board.getTiles()[i][j].isEmpty()){
                switch(this->board.getTiles()[i][j].getTop().getColor()){
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
    if(playerOnePieces > playerTwoPieces){ return 1;}
    else{ return 2;}
}