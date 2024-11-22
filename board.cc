#include <vector>
#include <string>
#include <algorithm>
#include <random>
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

Board::Board(string link1, string link2) : tiles{board_width, vector<Tile>(8)} {
    if (link1 == "") {
        link1 = getRandomLinks();
    }
    if (link2 == "") {
        link2 = getRandomLinks();
    }

    
}

pair<int, int> Board::getCoords(char link) {
    if (isupper(link)) {
        return {link2[link - 'A'].getX(), link2[link - 'A'].getY()};
    } 
    return {link1[link - 'a'].getX(), link1[link - 'a'].getY()};
}

bool Board::isEmpty(int x, int y) {
    return tiles[x][y].isEmpty();
}

void Board::move(char link, int dir) {
    if (isupper(link)) {
        
    }
}
