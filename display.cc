#include "display.h"
#include "board.h"
#include <iostream>
#include <string>
#include <memory>

using namespace std;

TextDisplay::TextDisplay(shared_ptr<Board> b) : b{b} {}

void TextDisplay::notify(int players_turn) {
    cout << "IN TEXTDISPLAY NOTIFY" << endl;
    cout << "Player 1:" << endl;
    cout << "Downloaded: ";
    cout << b->getPlayer1().getData() << "D, " << b->getPlayer1().getVirus() << "V" << endl;
    cout << "Abilities: " << b->getPlayer1().getNumAbilities() << endl;

    //Prints out links 
    for (char c = 'a'; c <= 'h'; c++) {
        cout << c << ": ";
        //Display link if:
        //  - It is the link owner's turn
        //  - It is the opponent's turn, but the link is revealed
        if (players_turn == 1 || (players_turn == 2 && b->getLink(c - 'a').isRevealed())) {
            if (b->getLink(c - 'a').getType()) { //If is data
                cout << "D";
            } else {
                cout << "V";
            }
            cout << b->getLink(c - 'a').getStrength() << " ";
        } else {
            cout << " ?";
        }

        if (c == 'd' || c == 'h') {
            cout << endl;
        }
    }
    //Prints out board
    for (int i = 0; i < BOARD_WIDTH; ++i) {
        cout << "=" << endl;
    }
    for (int y = BOARD_WIDTH - 1; y >= 0; --y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            b->getTile(x, y).getChar();
        }
    }
    for (int i = 0;  i < BOARD_WIDTH; ++i) {
        cout << "=" << endl;
    }

    cout << "Player 2:" << endl;
    cout << "Downloaded: ";
    cout << b->getPlayer2().getData() << "D, " << b->getPlayer2().getVirus() << "V" << endl;
    cout << "Abilities: " << b->getPlayer2().getNumAbilities() << endl;

    //Prints out links 
    for (char c = 'A'; c <= 'H'; c++) {
        cout << c << ": ";
        if (players_turn == 2 || (players_turn == 1 && b->getLink(c - 'A').isRevealed())) {
            if (b->getLink(c - 'A').getType()) {
                cout << "D";
            } else {
                cout << "V";
            }
            cout << b->getLink(c - 'A').getStrength() << " ";
        } else {
            cout << "?  ";
        }
        if (c == 'D' || c == 'H') {
            cout << endl;
        }
    }
}



