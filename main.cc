#include "link.h"
#include "tile.h"
#include "board.h"
#include "player.h"
#include "ability.h"
#include "display.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stack>

using namespace std;

string tolower(string str) {
    string ans = "";
    for (char c : str) {
        ans += tolower(c);
    }
    return ans;
}

Direction convertDir(string dir) {
    dir = tolower(dir);
    if (dir == "up") {
        return UP;
    } else if (dir == "down") {
        return DOWN;
    } else if (dir == "left") {
        return LEFT;
    } else if (dir == "right") {
        return RIGHT;
    }
    throw invalid_argument("direction is not one of up/down/left/right");
}

int main(int argc, char* argv[]) {
    string ability1 = "LFDSP", ability2 = "LFDSP";
    string link1 = "", link2 = "";
    bool graphical_display = false;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-ability1" && i + 1 < argc) {
            ability1 = argv[++i];
        } else if (arg == "-ability2" && i + 1 < argc) {
            ability2 = argv[++i];
        } else if (arg == "-link1" && i + 1 < argc) {
            ifstream fin{argv[++i]};
            string tmp;
            while (getline(fin, tmp)) {
                link1 += tmp;
            }
        } else if (arg == "-link2" && i + 1 < argc) {
            ifstream fin{argv[++i]};
            string tmp;
            while (getline(fin, tmp)) {
                link2 += tmp;
            }
        } else if (arg == "-graphics") {
            graphical_display = true;
        }
    }

    shared_ptr<Board> board = make_shared<Board>(link1, link2, ability1, ability2);
    shared_ptr<TextDisplay> textDisplay;
    if (!graphical_display) {
        textDisplay = make_shared<TextDisplay>(board);
        board->attach(textDisplay);
    } // TODO graphical display

    stack<istream*> input_streams;
    input_streams.push(&cin);
    int players_turn = 1;
    while (!input_streams.empty()) {
        istream* in = input_streams.top();
        input_streams.pop();
        string command;

        while (*in >> command) {
            command = tolower(command);
            if (command == "sequence") {
                string filename;
                *in >> filename;
                ifstream fin{filename};
                input_streams.push(in);
                input_streams.push(&fin);
                break;
            } else if (command == "move") {
                char link;
                string direction;
                Direction dir;
                *in >> link;
                if ((link < 'a' || link > 'h') && (link < 'A' || link > 'H')) {
                    board->sendMessage("Invalid link character entered");
                    input_streams.push(in);
                    break;
                }
                if (players_turn == 1 && isupper(link)) {
                    board->sendMessage("Player 1 cannot move Player 2's link");
                    input_streams.push(in);
                    break;
                }
                if (players_turn == 2 && islower(link)) {
                    board->sendMessage("Player 2 cannot move Player 1's link");
                    input_streams.push(in);
                    break;
                }

                *in >> direction;
                try {
                    dir = convertDir(direction);
                } catch (const invalid_argument& e) {
                    board->sendMessage(e.what());
                    input_streams.push(in);
                    break;
                }

                board->move(link, dir);
                players_turn = 3 - players_turn; // switch turn
            } else if (command == "abilities") {
                if (players_turn == 1) {
                    board->getPlayer1().printAbility();
                } else {
                    board->getPlayer2().printAbility();
                }
            } else if (command == "ability") {

            } else if (command == "board") {

            } else if (command == "quit") {
                
            }
        }
    }
}
