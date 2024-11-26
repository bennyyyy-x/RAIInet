#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <sstream>
#include "board.h"
#include "constants.h"

using namespace std;

// Convert int to link string
string getLinkString(int x) {
    return (x < 4 ? "V" : "D") + to_string(x + 1);
}

// Generate random order of links
string getRandomLinks() {
    vector<int> order = {0, 1, 2, 3, 4, 5, 6, 7};
    random_device rd;
    shuffle(order.begin(), order.end(), rd);
    
    string ans = getLinkString(order[0]);
    for (int i = 1; i < 8; ++i) {
        ans += getLinkString(order[i]);
    }
    return ans;
}

Board::Board(string link1_string, string link2_string) : tiles{BOARD_WIDTH, vector<Tile>(BOARD_WIDTH)} {
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
        int x = c - 'a', y = 7;
        if (c == 'd' || c == 'e') {
            y = 6;
        }
        link1.push_back(Link{c, int(link[1]), link[0] == 'D', x, y});
        c = char(c + 1);
    }

    istringstream iss2{link2_string};
    char c = 'A';
    while (iss2 >> link) {
        int x = c - 'A', y = 7;
        if (c == 'D' || c == 'E') {
            y = 1;
        }
        link2.push_back(Link{c, int(link[1]), link[0] == 'D', x, y});
        c = char(c + 1);
    }

    // TODO Unfinished
}

pair<int, int> Board::getCoords(char link) {
    if (islower(link)) {
        return {link1[link - 'a'].getX(), link1[link - 'a'].getY()};
    } 
    return {link2[link - 'A'].getX(), link2[link - 'A'].getY()};
}

bool Board::isEmpty(int x, int y) {
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

// Return false if move was unable to be made
bool move_helper(Link& link, Direction dir) {
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
    } else if (tmp_y > BOARD_WIDTH - 1) { // Moves off top edge
        if (islower(c)) { // Illegal
            return false;
        }
        link.setDownload(ByPlayer2);
    }

    if (tmp_y == 0 && (tmp_x == 3 || tmp_x == 4)) { // Bottom server ports
        if (isupper(c)) {
            return false;
        }
        link.setDownload(ByPlayer2);
    } else if (tmp_y == BOARD_WIDTH - 1 && (tmp_x == 3 || tmp_x == 4)) { // Top server ports
        if (islower(c)) {
            return false;
        }
        link.setDownload(ByPlayer1);
    }

    link.setX(tmp_x);
    link.setY(tmp_y);
    return true;
}

// TODO: rememeber to add in input error checking for link chars
void Board::move(char link, Direction dir) {
    if (islower(link)) {
        int x = link1[link - 'a'].getX(), y = link1[link - 'a'].getY();
        if (move_helper(link1[link - 'a'], dir)) {
            tiles[x][y].setChar('.');
            if (link1[link - 'a'].downloadStatus() != NotDownloaded) {
                int new_x = link1[link - 'a'].getX(), new_y = link1[link - 'a'].getY();
                tiles[new_x][new_y].setChar(link1[link - 'a'].getChar());
            }
        }
    } else {
        int x = link2[link - 'A'].getX(), y = link2[link - 'A'].getY();
        if (move_helper(link2[link - 'A'], dir)) {
            tiles[x][y].setChar('.');
            if (link2[link - 'A'].downloadStatus() != NotDownloaded) {
                int new_x = link2[link - 'A'].getX(), new_y = link2[link - 'A'].getY();
                tiles[new_x][new_y].setChar(link2[link - 'A'].getChar());
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

    int battle_x, battle_y, other_x, other_y;
    if (initiator == 1) {
        battle_x = link2[i2].getX();
        battle_y = link2[i2].getY();
        other_x = link1[i1].getX();
        other_y = link1[i1].getY();
    } else {
        other_x = link2[i2].getX();
        other_y = link2[i2].getY();
        battle_x = link1[i1].getX();
        battle_y = link1[i1].getY();
    }

    tiles[other_x][other_y].setChar('.');
    
    //if player 1 is winner, player 1 downloads player 2's link
    if ((st1 == st2 && initiator == 1) || st1 > st2) {
        link2[i2].setDownload(ByPlayer1);
        tiles[battle_x][battle_y].setChar(l1);
        return;
    } 
    link1[i1].setDownload(ByPlayer2);
    tiles[battle_x][battle_y].setChar(l2);
}

void Board::render() const {

}
