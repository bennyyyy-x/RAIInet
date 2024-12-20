#include "link.h"
#include "tile.h"
#include "board.h"
#include "player.h"
#include "ability.h"
#include "display.h"
#include "constants.h"
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
        return Direction::Up;
    } else if (dir == "down") {
        return Direction::Down;
    } else if (dir == "left") {
        return Direction::Left;
    } else if (dir == "right") {
        return Direction::Right;
    }
    throw invalid_argument("direction is not one of up/down/left/right");
}

bool abilityHelper(shared_ptr<Board> board, Player& player, int id, shared_ptr<istream> in, int& players_turn, bool& hijack_enabled);

int gameEndCheck(shared_ptr<Board> board) {
    Player& player1 = board->getPlayer1();
    Player& player2 = board->getPlayer2();
    if (player1.getData() == 4 || player2.getVirus() == 4) {
        return 1;
    } else if (player1.getVirus() == 4 || player2.getData() == 4) {
        return 2;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    string ability1 = "LFDSP", ability2 = "LFDSP";
    string link1 = "", link2 = "";
    bool graphics_enabled = false;

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
            graphics_enabled = true;
        }
    }

    shared_ptr<Board> board = make_shared<Board>(link1, link2, ability1, ability2);
    shared_ptr<TextDisplay> textDisplay;
    shared_ptr<GraphicalDisplay> graphicalDisplay;
    if (!graphics_enabled) {
        textDisplay = make_shared<TextDisplay>(board);
        board->attach(textDisplay);
    } else {
        graphicalDisplay = make_shared<GraphicalDisplay>(board, BOARD_WIDTH_GRAPH, 130 + BOARD_WIDTH_GRAPH + BOARD_CORNER_Y);
        board->attach(graphicalDisplay);
    }

    stack<shared_ptr<istream>> input_streams;
    input_streams.push(make_shared<istream>(cin.rdbuf()));
    int players_turn = 1;
    bool ability_used = false;
    bool hijack_enabled = false;
    board->render(players_turn); // Render initially
    
    string command;
    while (!input_streams.empty()) {
        shared_ptr<istream> in = input_streams.top();
        if (in->fail()) {
            in->clear();
            in->ignore();
        }
        input_streams.pop();

        while (*in >> command) {
            command = tolower(command);
            Player& player = players_turn == 1 ? board->getPlayer1() : board->getPlayer2();

            if (command == "sequence") {
                string filename;
                *in >> filename;
                shared_ptr<ifstream> fin = make_shared<ifstream>(filename);
                input_streams.push(in);
                if (fin->is_open()) {
                    input_streams.push(fin);
                }
                break;

            } else if (command == "move") {
                char link;
                string direction;
                Direction dir;
                *in >> link;
                if (!isLink(link)) {
                    board->sendMessage("Invalid link character entered");
                    input_streams.push(in);
                    break;
                }
                if (!hijack_enabled && players_turn == 1 && isPlayer2Link(link)) {
                    board->sendMessage("Player 1 cannot move Player 2's link");
                    input_streams.push(in);
                    break;
                }
                if (!hijack_enabled && players_turn == 2 && isPlayer1Link(link)) {
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

                bool moveSuccessful = board->move(link, dir);
                if (moveSuccessful) {
                    players_turn = 3 - players_turn; // switch turn
                    ability_used = false;
                    hijack_enabled = false;
                    board->render(players_turn); // Refresh the board after each move

                    int winner = gameEndCheck(board);
                    if (winner != 0) {
                        board->endGame(winner);
                        while (!input_streams.empty()) {
                            input_streams.pop();
                        }
                        break;
                    }
                } else {
                    board->sendMessage("Move failed");
                }

            } else if (command == "abilities") {
                player.printAbility();

            } else if (command == "ability") {
                if (ability_used) {
                    board->sendMessage("Player already used ability in this turn, unable to use another one");
                    input_streams.push(in);
                    break;
                }
                int id;
                *in >> id;
                if (id <= 0 || id > ABILITY_CARD_NUMS) { // id is 1-5, not 0-4
                    board->sendMessage("Invalid ability card id: " + to_string(id));
                    input_streams.push(in);
                    break;
                }
                ability_used = abilityHelper(board, player, id, in, players_turn, hijack_enabled);
                if (ability_used) {
                    board->render(players_turn); // render after applying ability
                } else {
                    board->sendMessage("Unable to use ability");
                }

            } else if (command == "board") {
                board->render(players_turn);

            } else if (command == "quit") {
                while (!input_streams.empty()) {
                    input_streams.pop();
                }
                // Make this flag false so when using quit command
                // the screen will not keep being displayed
                graphics_enabled = false;
                break;
            }

            if (in->fail()) {
                in->clear();
                in->ignore();
            }
        }
    }

    // Keep displaying graphical end screen
    while (graphics_enabled && cin >> command) {
        command = tolower(command);
        if (command == "quit" || command == "q") {
            break;
        }
    }
}

// return bool whether ability was applied
bool abilityHelper(shared_ptr<Board> board, Player& player, int id, shared_ptr<istream> in, int& players_turn, bool& hijack_enabled) {
    AbilityName ability = player.getAbilityName(id);
    char link, link1, link2;
    int x, y;

    switch (ability) {

        case AbilityName::LINKBOOST:
            if (*in >> link) {
                if ((players_turn == 1 && isPlayer1Link(link))
                    || (players_turn == 2 && isPlayer2Link(link))) {
                    return player.useAbility(id, *board, {string(1, link)});
                }
            }
            board->sendMessage("Invalid link ID for link boost");
            return false;

        case AbilityName::FIREWALL:
            if (*in >> x >> y) {
                if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_WIDTH) {
                    board->sendMessage("Invalid coordinates for firewall");
                    return false;
                }
                Tile& tile = board->getTile(x, y);
                if (tile.getChar() == 'S' || tile.getFirewall() != FirewallStatus::NoFirewall) {
                    board->sendMessage("Tile is unavailable for placing firewall");
                    return false;
                }
                return player.useAbility(id, *board, {to_string(x), to_string(y)});
            }
            board->sendMessage("Unable to read coordinates for Firewall");
            return false;

        case AbilityName::DOWNLOAD:
            if (*in >> link) {
                if ((players_turn == 2 && isPlayer1Link(link))
                    || (players_turn == 1 && isPlayer2Link(link))) {
                    return player.useAbility(id, *board, {string(1, link)});
                }
            }
            board->sendMessage("Invalid link for Download");
            return false;

        case AbilityName::POLARIZE:
            if (*in >> link) {
                if ((players_turn == 1 && isPlayer1Link(link))
                    || (players_turn == 2 && isPlayer2Link(link))) {
                    return player.useAbility(id, *board, {string(1, link)});
                }
            }
            board->sendMessage("Invalid link for Polarize");
            return false;

        case AbilityName::SCAN:
            if (*in >> link) {
                if ((players_turn == 2 && isPlayer1Link(link))
                    || (players_turn == 1 && isPlayer2Link(link))) {
                    return player.useAbility(id, *board, {string(1, link)});
                }
            }
            board->sendMessage("Invalid link for Scan");
            return false;

        case AbilityName::EXCHANGE:
            if (*in >> link1 >> link2) {
                if ((isPlayer1Link(link1) && isPlayer2Link(link2))
                 || (isPlayer2Link(link1) && isPlayer1Link(link2))) {
                    return player.useAbility(id, *board, {string(1, link1), string(1, link2)});
                }
            }
            board->sendMessage("Invalid links for Exchange");
            return false;

        case AbilityName::TELEPORT:
            if (*in >> link) {
                if (isLink(link)) {
                    return player.useAbility(id, *board, {string(1, link)});
                }
            }
            board->sendMessage("Invalid input for Teleport");
            return false;

        case AbilityName::HIJACK:
            hijack_enabled = player.useAbility(id, *board, {});
            return hijack_enabled;
    }
    return false;
}
