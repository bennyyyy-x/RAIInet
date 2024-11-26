#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <sstream>
#include <memory>
#include "display.h"
#include "board.h"
#include "constants.h"

using namespace std;

void Subject::attach(shared_ptr<Observer> ptr) {
    observers.push_back(ptr);
}

void Subject::detach(shared_ptr<Observer>& ptr) {
    for (auto it = observers.begin(); it != observers.end();) {
        if (auto observer = it->lock()) {
            if (observer == ptr) {
                it = observers.erase(it);
            } else {
                ++it;
            }
        } else {
            it = observers.erase(it); // Remove expired weak_ptr
        }
    }
}

void Subject::notifyObservers(int players_turn) {
    cout << "IN NOTIFYOBSERVERS" << endl;
    for (auto it = observers.begin(); it != observers.end();) {
        if (auto ptr = it->lock()) {
            ptr->notify(players_turn);
            ++it;
        } else {
            observers.erase(it);
        }
    }
}

// Convert int to link string
string getLinkString(int x) {
    return (x < 4 ? "V" : "D") + to_string(x + 1);
}

// Generate random order of links
string getRandomLinks() {
    vector<int> order = {0, 1, 2, 3, 4, 5, 6, 7};
    random_device rd;
    shuffle(order.begin(), order.end(), rd);
    
    string ans = getLinkString(order[0]) + " ";
    for (int i = 1; i < 8; ++i) {
        ans += getLinkString(order[i]) + " ";
    }
    return ans;
}

Board::Board(string link1_string, string link2_string, string ability1, string ability2)
    : tiles{BOARD_WIDTH, vector<Tile>(BOARD_WIDTH)}, player1{ability1}, player2{ability2} {
    if (link1_string == "") {
        link1_string = getRandomLinks();
    }
    if (link2_string == "") {
        link2_string = getRandomLinks();
    }

    istringstream iss1{link1_string};
    string link;
    char c = 'a';
    while (iss1 >> link) {
        int x = c - 'a', y = BOARD_WIDTH - 1;
        if (c == 'd' || c == 'e') {
            y = 6;
        }
        link1.push_back(Link{c, int(link[1]), link[0] == 'D', x, y});
        c = char(c + 1);
    }

    istringstream iss2{link2_string};
    c = 'A';
    while (iss2 >> link) {
        int x = c - 'A', y = BOARD_WIDTH - 1;
        if (c == 'D' || c == 'E') {
            y = 1;
        }
        link2.push_back(Link{c, int(link[1]), link[0] == 'D', x, y});
        c = char(c + 1);
    }

    for (int i = 0; i < BOARD_WIDTH; ++i) {
        if (i == 3 || i == 4) {
            tiles[0][i].setChar('S');
            tiles[1][i].setChar(char('A' + i));
            tiles[BOARD_WIDTH - 1][i].setChar('S');
            tiles[BOARD_WIDTH - 2][i].setChar(char('a' + i));
        } else {
            tiles[0][i].setChar(char('A' + i));
            tiles[BOARD_WIDTH - 1][i].setChar(char('a' + i));
        }
    } // TODO Player construction
}

bool Board::isEmpty(int x, int y) const {
    return tiles[x][y].isEmpty();
}

int convertToY(Direction dir) {
    if (dir == Direction::UP) {
        return 1;
    } else if (dir == Direction::DOWN) {
        return -1;
    }
    return 0;
}

int convertToX(Direction dir) {
    if (dir == Direction::RIGHT) {
        return 1;
    } else if (dir == Direction::LEFT) {
        return -1;
    }
    return 0;
}

void Board::download(DownloadStatus status, const Link& link) {
    if (status == ByPlayer1) {
        player1.download(link);
    } else if (status == ByPlayer2) {
        player2.download(link);
    }
}

// Return false if move was unable to be made
bool move_helper(Link& link, Direction dir, Board& board) {
    // Cannot move downloaded link
    if (link.downloadStatus() != NotDownloaded) {
        return false;
    }

    int tmp_x = link.getX() + convertToX(dir), tmp_y = link.getY() + convertToY(dir);
    if (tmp_x < 0 || tmp_x > BOARD_WIDTH - 1) { // Moves off side edge (Illegal)
        return false;
    }
    
    char c = link.getChar();
    if (tmp_y < 0) { // Moves off bottom edge
        if (isupper(c)) { // Illegal
            return false;
        }
        link.setDownload(ByPlayer1);
        board.download(ByPlayer1, link);
    } else if (tmp_y > BOARD_WIDTH - 1) { // Moves off top edge
        if (islower(c)) { // Illegal
            return false;
        }
        link.setDownload(ByPlayer2);
        board.download(ByPlayer2, link);
    }

    if (tmp_y == 0 && (tmp_x == 3 || tmp_x == 4)) { // Bottom server ports
        if (isupper(c)) {
            return false;
        }
        link.setDownload(ByPlayer2);
        board.download(ByPlayer2, link);
    } else if (tmp_y == BOARD_WIDTH - 1 && (tmp_x == 3 || tmp_x == 4)) { // Top server ports
        if (islower(c)) {
            return false;
        }
        link.setDownload(ByPlayer1);
        board.download(ByPlayer1, link);
    } else if (!board.isEmpty(tmp_x, tmp_y)) { // Moves on top another link
        char other_c = board.getTile(tmp_x, tmp_y).getChar();
        if (islower(c) == islower(other_c)) { // Same player's link
            return false;
        }
        if (islower(c)) {
            board.battle(c, other_c, 1);
        } else {
            board.battle(other_c, c, 2);
        }
        return false;
    }

    link.setX(tmp_x);
    link.setY(tmp_y);
    return true;
}

// TODO: rememeber to add in input error checking for link chars
void Board::move(char link, Direction dir) {
    if (islower(link)) {
        auto coord = getCoords(link);
        if (move_helper(link1[link - 'a'], dir, *this)) {
            tiles[coord.first][coord.second].setChar('.');
            if (link1[link - 'a'].downloadStatus() != NotDownloaded) {
                auto new_coord = getCoords(link);
                tiles[new_coord.first][new_coord.second].setChar(link);
            }
        }
    } else {
        auto coord = getCoords(link);
        if (move_helper(link2[link - 'A'], dir, *this)) {
            tiles[coord.first][coord.second].setChar('.');
            if (link2[link - 'A'].downloadStatus() != NotDownloaded) {
                auto new_coord = getCoords(link);
                tiles[new_coord.first][new_coord.second].setChar(link);
            }
        }
    }
}

//battle: player 1's link, player 2's link, player initiator of battle
void Board::battle(char l1, char l2, int initiator) {
    int i1 = l1 - 'a';
    int i2 = l2 - 'A';
    
    link1[i1].reveal();
    link2[i2].reveal();

    int st1 = link1[i1].getStrength();
    int st2 = link2[i2].getStrength();

    pair<int, int> battle_coords, other_coords;
    if (initiator == 1) {
        battle_coords = getCoords(l2);
        other_coords = getCoords(l1);
    } else {
        battle_coords = getCoords(l1);
        other_coords = getCoords(l2);
    }

    tiles[other_coords.first][other_coords.second].setChar('.');
    
    //if player 1 is winner, player 1 downloads player 2's link
    if ((st1 == st2 && initiator == 1) || st1 > st2) {
        link2[i2].setDownload(ByPlayer1);
        download(ByPlayer1, link2[i2]);
        tiles[battle_coords.first][battle_coords.second].setChar(l1);
        return;
    } 
    link1[i1].setDownload(ByPlayer2);
    download(ByPlayer2, link1[i1]);
    tiles[battle_coords.first][battle_coords.second].setChar(l2);
}

void Board::render(int players_turn) {
    cout << "IN RENDER" << endl;
    notifyObservers(players_turn);
}

pair<int, int> Board::getCoords(char link) const {
    if (islower(link)) {
        return {link1[link - 'a'].getX(), link1[link - 'a'].getY()};
    } 
    return {link2[link - 'A'].getX(), link2[link - 'A'].getY()};
}

Player& Board::getPlayer1() {
    return player1;
}

Player& Board::getPlayer2() {
    return player2;
}

Link &Board::getLink(char link) {
    if (islower(link)) {
        return link1[link - 'a'];
    }
    return link2[link - 'A'];
}

Tile& Board::getTile(int x, int y) {
    return tiles[x][y];
}
