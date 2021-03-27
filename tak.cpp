#include "tak.h"
#include <string>

Tak::Tak(int mode, int difficultyP1, int difficultyP2) {
    this->mode = mode;
    this->difficultyP1 = difficultyP1;
    this->difficultyP2 = difficultyP2;
    this->currentPlayer = 1;
    this->board = Board(4, 4);
}

void Tak::gameCycle() {
    if (endOfGame()) {
        getWinner();
        return ;
    }

    drawHeader();
    drawBoard();

    vector<int> play = getPlay();

    while (!canPlay()) {
        play = getPlay();
    }

    makeMove(play);
    changePlayer();
    gameCycle();
}

bool Tak::endOfGame() {
    return false;
}

void Tak::getWinner() {

}

void Tak::drawHeader() {
    switch (getMode()) {
        case 1:
            cout << "Player 1: Human";
            cout << "\tPlayer 2: Human";
            break;
        case 2:
            cout << "Player 1: Human";
            cout << "\tPlayer 2: Computer";
            cout << "\nDifficulty for Computer: " << getDifficulty("P1");
            break;
        case 3:
            cout << "Player 1: Computer";
            cout << "\tPlayer 2: Computer";
            cout << "\nDifficulty for Player 1: " << getDifficulty("P1");
            cout << "\nDifficulty for Player 2: " << getDifficulty("P2");
            break;
        default:
            cout << "Error!\n";
    }
}

int Tak::getMode() {
    return this->mode;
}

void Tak::drawBoard() {
    this->board.draw();
}

vector<int> Tak::getPlay() {
    return vector<int>();
}

bool Tak::canPlay() {
    return false;
}

void Tak::makeMove(vector<int> play) {

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


