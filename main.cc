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

bool isPlayer1Link(char link) {
    return link >= 'a' && link <= 'h';
}

bool isPlayer2Link(char link) {
    return link >= 'A' && link <= 'H';
}

bool isLink(char link) {
    return isPlayer1Link(link) || isPlayer2Link(link);
}

bool ability_helper(shared_ptr<Board> board, Player& player, int id, shared_ptr<istream> in, int& players_turn);

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

    stack<shared_ptr<istream>> input_streams;
    input_streams.push(make_shared<istream>(cin.rdbuf()));
    int players_turn = 1;
    bool ability_used = false;
    while (!input_streams.empty()) {
        shared_ptr<istream> in = input_streams.top();
        if (in->fail()) {
            in->clear();
            in->ignore();
        }
        input_streams.pop();
        string command;

        while (*in >> command) {
            command = tolower(command);
            Player& player = players_turn == 1 ? board->getPlayer1() : board->getPlayer2();

            if (command == "sequence") {
                string filename;
                *in >> filename;
                shared_ptr<ifstream> fin = make_shared<ifstream>(filename);
                input_streams.push(in);
                input_streams.push(fin);
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
                if (players_turn == 1 && isPlayer2Link(link)) {
                    board->sendMessage("Player 1 cannot move Player 2's link");
                    input_streams.push(in);
                    break;
                }
                if (players_turn == 2 && isPlayer1Link(link)) {
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
                ability_used = false;

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
                if (id < 0 || id >= ABILITY_CARD_NUMS) {
                    board->sendMessage("Invalid ability card id: " + to_string(id));
                    input_streams.push(in);
                    break;
                }
                ability_used = ability_helper(board, player, id, in, players_turn);

            } else if (command == "board") {
                board->render(players_turn);

            } else if (command == "quit") {
                while (!input_streams.empty()) {
                    input_streams.pop();
                }
                break;
            }

            if (in->fail()) {
                in->clear();
                in->ignore();
            }
        }
    }
}

// return bool whether ability was applied
bool ability_helper(shared_ptr<Board> board, Player& player, int id, shared_ptr<istream> in, int& players_turn) {
    AbilityName ability = player.getAbilityName(id);
    char link, link1, link2;
    int x, y;

    switch (ability) {

        case AbilityName::LinkBoost:
            if (*in >> link) {
                if ((players_turn == 1 && isPlayer1Link(link))
                    || (players_turn == 2 && isPlayer2Link(link))) {
                    return player.useAbility(id, *board, {to_string(link)});
                }
            }
            board->sendMessage("Invalid link ID for link boost");
            return false;

        case AbilityName::Firewall:
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

        case AbilityName::Download:
            if (*in >> link) {
                if ((players_turn == 2 && isPlayer1Link(link))
                    || (players_turn == 1 && isPlayer2Link(link))) {
                    return player.useAbility(id, *board, {to_string(link)});
                }
            }
            board->sendMessage("Invalid link for Download");
            return false;

        case AbilityName::Polarize:
            if (*in >> link) {
                if ((players_turn == 1 && isPlayer1Link(link))
                    || (players_turn == 2 && isPlayer2Link(link))) {
                    return player.useAbility(id, *board, {to_string(link)});
                }
            }
            board->sendMessage("Invalid link for Polarize");
            return false;

        case AbilityName::Scan:
            if (*in >> link) {
                if ((players_turn == 2 && isPlayer1Link(link))
                    || (players_turn == 1 && isPlayer2Link(link))) {
                    return player.useAbility(id, *board, {to_string(link)});
                }
            }
            board->sendMessage("Invalid link for Scan");
            return false;

        case AbilityName::Exchange:
            if (*in >> link1 >> link2) {
                if ((isPlayer1Link(link1) && isPlayer2Link(link2))
                 || (isPlayer2Link(link1) && isPlayer1Link(link2))) {
                    return player.useAbility(id, *board, {to_string(link1), to_string(link2)});
                }
            }
            board->sendMessage("Invalid links for Exchange");
            return false;

        case AbilityName::Teleport:
            if (*in >> link >> x >> y) {
                if (!isLink(link)) {
                    board->sendMessage("Invalid link for Teleport");
                    return false;
                }
                if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_WIDTH) {
                    board->sendMessage("Invalid coordinates for Teleport");
                    return false;
                }
                char other = board->getTile(x, y).getChar();
                if ((isPlayer1Link(link) && isPlayer1Link(other))
                 || (isPlayer2Link(link) && isPlayer2Link(other))) {
                    board->sendMessage("Cannot move onto player's own link");
                    return false;
                }
                return player.useAbility(id, *board, {to_string(link), to_string(x), to_string(y)});
            }
            board->sendMessage("Invalid input for Teleport");
            return false;

        case AbilityName::Hijack:
            if (*in >> link) {
                if ((players_turn == 1 && isPlayer1Link(link))
                    || (players_turn == 2 && isPlayer2Link(link))) {
                    return player.useAbility(id, *board, {to_string(link)});
                }
            }
            board->sendMessage("Invalid link for Hijack");
            return false;
    }
    return false;
}
