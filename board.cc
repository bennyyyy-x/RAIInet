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

Board::Board(string link1_string, string link2_string) : tiles{board_width, vector<Tile>(board_width)} {
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

// add error checking?? or add in if statements so if 
// link moves outside board or on server ports dont do anything?
// also remmeber to add in input error checking for link chars
void Board::move(char link, Direction dir) {
    if (islower(link)) {
        link1[link - 'a'].move(dir);
    } else {
        link2[link - 'A'].move(dir);
    }
}

int Board::battle(char l1, char l2, int initiator) {
    link1[l1 - 'a'].reveal();
    link2[l2 - 'A'].reveal();

    int st1 = link1[l1 - 'a'].getStrength();
    int st2 = link2[l1 - 'A'].getStrength();
    if (st1 == st2) {
        return initiator;
    }
    if (st1 > st2) {
        return 1;
    }
    return 2;
}
