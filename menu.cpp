#include <iostream>
#include "menu.h"
#include "tak.h"

using namespace std;

void Menu::mainMenu() {
    drawMainMenu();
}

void Menu::drawMainMenu() {
    cout << "\t\tTAK\n\n";
    cout << "Welcome to Tak, a Board Game for 2 players.\n\n";
    cout << "Please choose the game mode:\n\n";
    cout << "1. Human vs Human\n";
    cout << "2. Human vs Computer\n";
    cout << "3. Computer vs Computer\n";

    int mode = 0;
    while (mode != 1 && mode != 2 && mode != 3) {
        cout << "\nInput: ";
        cin >> mode;
    }

    cout << "Game board size - Please select a number between 3 and 8\n\n";
    cout << "3 (3x3)\n";
    cout << "4 (4x4)\n";
    cout << "5 (5x5)\n";
    cout << "6 (6x6)\n";
    cout << "7 (7x7)\n";
    cout << "8 (8x8)\n";

    int size = 0;
    while (size < 3 || size > 8) {
        cout << "\nInput: ";
        cin >> size;
    }

    int difficultyP2 = 0;
    int difficultyP1 = 0;

    if (mode == 2) {
        cout << "Please choose the game difficulty for Player 2:\n\n";
        cout << "1. Easy\n";
        cout << "2. Normal\n";
        cout << "3. Hard\n";

        while (difficultyP2 != 1 && difficultyP2 != 2 && difficultyP2 != 3) {
            cout << "\nInput: ";
            cin >> difficultyP2;
        }

        difficultyP1 = 3;
    } else if (mode == 3) {
        cout << "Please choose the game difficulty for Player 1:\n\n";
        cout << "1. Easy\n";
        cout << "2. Normal\n";
        cout << "3. Hard\n";

        while (difficultyP1 != 1 && difficultyP1 != 2 && difficultyP1 != 3) {
            cout << "\nInput: ";
            cin >> difficultyP1;
        }

        cout << "Now, please choose the game difficulty for Player 2:\n\n";
        cout << "1. Easy\n";
        cout << "2. Normal\n";
        cout << "3. Hard\n";

        while (difficultyP2 != 1 && difficultyP2 != 2 && difficultyP2 != 3) {
            cout << "\nInput: ";
            cin >> difficultyP2;
        }
    }

    Tak newGame = Tak(mode, difficultyP1, difficultyP2, size);
    newGame.gameCycle();
}