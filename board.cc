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


bool isPlayer1Link(char link) {
    return link >= 'a' && link <= 'h';
}


bool isPlayer2Link(char link) {
    return link >= 'A' && link <= 'H';
}


bool isLink(char link) {
    return isPlayer1Link(link) || isPlayer2Link(link);
}


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
    for (auto it = observers.begin(); it != observers.end();) {
        if (auto ptr = it->lock()) {
            ptr->notify(players_turn);
            ++it;
        } else {
            observers.erase(it);
        }
    }
}


void Subject::sendMessage(string msg) {
    for (auto it = observers.begin(); it != observers.end();) {
        if (auto ptr = it->lock()) {
            ptr->message(msg);
            ++it;
        } else {
            observers.erase(it);
        }
    }
}


// Convert int to link string
string getLinkString(int x) {
    return (x < 4 ? "V" : "D") + to_string(x % 4 + 1);
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
    : tiles{BOARD_WIDTH, vector<Tile>(BOARD_WIDTH)}, player1{1, ability1}, player2{2, ability2} {
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
        link1.push_back(Link{c, link[1] - '1' + 1, link[0] == 'D', x, y});
        c = char(c + 1);
    }

    istringstream iss2{link2_string};
    c = 'A';
    while (iss2 >> link) {
        int x = c - 'A', y = 0;
        if (c == 'D' || c == 'E') {
            y = 1;
        }
        link2.push_back(Link{c, link[1] - '1' + 1, link[0] == 'D', x, y});
        c = char(c + 1);
    }

    for (int i = 0; i < BOARD_WIDTH; ++i) {
        if (i == 3 || i == 4) {
            tiles[i][0].setChar('S');
            tiles[i][1].setChar(char('A' + i));
            tiles[i][BOARD_WIDTH - 1].setChar('S');
            tiles[i][BOARD_WIDTH - 2].setChar(char('a' + i));
        } else {
            tiles[i][0].setChar(char('A' + i));
            tiles[i][BOARD_WIDTH - 1].setChar(char('a' + i));
        }
    } // TODO Player construction
}


bool Board::isEmpty(int x, int y) const {
    return tiles[x][y].isEmpty();
}


int convertToY(Direction dir) {
    if (dir == Direction::Up) {
        return 1;
    } else if (dir == Direction::Down) {
        return -1;
    }
    return 0;
}


int convertToX(Direction dir) {
    if (dir == Direction::Right) {
        return 1;
    } else if (dir == Direction::Left) {
        return -1;
    }
    return 0;
}


void Board::download(DownloadStatus status, const Link& link) {
    if (status == DownloadStatus::ByPlayer1) {
        player1.download(link);
    } else if (status == DownloadStatus::ByPlayer2) {
        player2.download(link);
    }
}


// Return false if move was unable to be made
MoveStatus move_helper(Link& link, Direction dir, Board& board) {
    // Cannot move downloaded link
    if (link.downloadStatus() != NotDownloaded) {
        cout << "Cannot move downloaded link" << endl;
        return MoveStatus::IllegalMove;
    }

    int tmp_x = link.getX() + convertToX(dir) * (link.isBoosted() ? 2 : 1); // Move with boost
    int tmp_y = link.getY() + convertToY(dir) * (link.isBoosted() ? 2 : 1);
    if (tmp_x < 0 || tmp_x > BOARD_WIDTH - 1) { // Moves off side edge (Illegal)
        cout << "Move off side edge" << endl;
        return MoveStatus::IllegalMove;
    }
    
    char c = link.getChar();
    Tile& destination = board.getTile(tmp_x, tmp_y);
    char other_c = destination.getChar();
    if (tmp_y < 0) { // Moves off bottom edge
        if (isupper(c)) {
            cout << "bottom edge illegal" << endl;
            return MoveStatus::IllegalMove; // Illegal
        }
        link.setDownload(DownloadStatus::ByPlayer1);
        cout << "download1" << endl;
        board.download(DownloadStatus::ByPlayer1, link);
        return MoveStatus::NeedTileChange;
    } else if (tmp_y >= BOARD_WIDTH) { // Moves off top edge
        if (islower(c)) {
            cout << "top edge illegal" << endl;
            return MoveStatus::IllegalMove; // Illegal
        }
        link.setDownload(DownloadStatus::ByPlayer2);
        cout << "download2" << endl;
        board.download(DownloadStatus::ByPlayer2, link);
        return MoveStatus::NeedTileChange;
    }

    if (tmp_y == 0 && (tmp_x == 3 || tmp_x == 4)) { // Bottom server ports
        if (isupper(c)) {
            cout << "bottom server port illegal" << endl;
            return MoveStatus::IllegalMove; // Illegal
        }
        link.setDownload(DownloadStatus::ByPlayer2);
        cout << "download3" << endl;
        board.download(DownloadStatus::ByPlayer2, link);
    } else if (tmp_y == BOARD_WIDTH - 1 && (tmp_x == 3 || tmp_x == 4)) { // Top server ports
        if (islower(c)) {
            cout << "top server port illegal" << endl;
            return MoveStatus::IllegalMove; // Illegal
        }
        link.setDownload(DownloadStatus::ByPlayer1);
        cout << "download4" << endl;
        board.download(DownloadStatus::ByPlayer1, link);
    }

    if (isLink(other_c) && isPlayer1Link(c) == isPlayer1Link(other_c)) { // Same player's link
        // cout << "c = " << c << " other_c = " << other_c << endl;
        // cout << "tmp_x = " << tmp_x << " tmp_y = " << tmp_y << endl;
        cout << "same player link" << endl;
        return MoveStatus::IllegalMove; // Illegal
    }

    if ((destination.getFirewall() == FirewallStatus::Player1s && isPlayer2Link(c)) // Move onto opp's firewall
     || (destination.getFirewall() == FirewallStatus::Player2s && isPlayer1Link(c))) {
        link.reveal();
        if (!link.getIsData()) { // virus gets immediately downloaded by owner
            DownloadStatus ds = isPlayer1Link(link.getChar()) ? DownloadStatus::ByPlayer1 : DownloadStatus::ByPlayer2;
            link.setDownload(ds);
            cout << "download5" << endl;
            board.download(ds, link);
            board.getTile(link.getX(), link.getChar()).setChar('.');
            return MoveStatus::DoNotNeedTileChange;
        }
    }

    if (isLink(other_c)) { // Moves on top another link
        if (isPlayer1Link(c)) {
            board.battle(c, other_c, 1);
        } else {
            board.battle(other_c, c, 2);
        }
        return MoveStatus::DoNotNeedTileChange;
    }

    link.setX(tmp_x);
    link.setY(tmp_y);
    return MoveStatus::NeedTileChange;
}


bool Board::move(char link, Direction dir) {
    // cout << "Coord for " << link << ": " << getCoords(link).first << ", " << getCoords(link).second << endl;
    MoveStatus ms;
    if (islower(link)) {
        auto coord = getCoords(link);
        ms = move_helper(link1[link - 'a'], dir, *this);
        if (ms == MoveStatus::NeedTileChange) {
            tiles[coord.first][coord.second].setChar('.');
            if (link1[link - 'a'].downloadStatus() == NotDownloaded) {
                auto new_coord = getCoords(link);
                tiles[new_coord.first][new_coord.second].setChar(link);
            }
        }
    } else {
        auto coord = getCoords(link);
        ms = move_helper(link2[link - 'A'], dir, *this);
        if (ms == MoveStatus::NeedTileChange) {
            tiles[coord.first][coord.second].setChar('.');
            if (link2[link - 'A'].downloadStatus() == NotDownloaded) {
                auto new_coord = getCoords(link);
                tiles[new_coord.first][new_coord.second].setChar(link);
            }
        }
    }
    return ms != MoveStatus::IllegalMove;
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

    // Update coordinates in Link class
    if (initiator == 1) {
        link1[i1].setX(battle_coords.first);
        link1[i1].setY(battle_coords.second);
    } else {
        link2[i2].setX(battle_coords.first);
        link2[i2].setY(battle_coords.second);
    }

    cout << "battle_coords" << battle_coords.first << ", " << battle_coords.second << endl;
    cout << "other_coords" << other_coords.first << ", " << other_coords.second << endl;

    tiles[other_coords.first][other_coords.second].setChar('.');
    
    //if player 1 is winner, player 1 downloads player 2's link
    if ((st1 == st2 && initiator == 1) || st1 > st2) {
        link2[i2].setDownload(DownloadStatus::ByPlayer1);
        cout << "download6" << endl;
        download(DownloadStatus::ByPlayer1, link2[i2]);
        tiles[battle_coords.first][battle_coords.second].setChar(l1);
        return;
    } 
    link1[i1].setDownload(DownloadStatus::ByPlayer2);
    cout << "download7" << endl;
    download(DownloadStatus::ByPlayer2, link1[i1]);
    tiles[battle_coords.first][battle_coords.second].setChar(l2);
}


void Board::render(int players_turn) {
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


Link& Board::getLink(char link) {
    if (islower(link)) {
        return link1[link - 'a'];
    }
    return link2[link - 'A'];
}


Tile& Board::getTile(int x, int y) {
    return tiles[x][y];
}

FirewallInfo::FirewallInfo(int x, int y, char c) : x{x}, y{y}, c{c} {}

void Board::addFirewall(int x, int y, char c) {
    firewallInfo.push_back({x, y, c});
}

vector<FirewallInfo>& Board::getFirewallInfo() {
    return firewallInfo;
}
