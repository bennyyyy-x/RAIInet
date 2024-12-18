#ifndef BOARD_H_
#define BOARD_H_

#include <memory>
#include <vector>
#include <string>
#include "constants.h"
#include "tile.h"
#include "link.h"
#include "player.h"

using namespace std;

bool isPlayer1Link(char link);
bool isPlayer2Link(char link);
bool isLink(char link);

class Observer;

class Subject {
    vector<weak_ptr<Observer>> observers;

public:
    void attach(shared_ptr<Observer> ptr);
    void detach(shared_ptr<Observer>& ptr);

    // Calls the `notify` function of Observers, then the displays will
    // update itself by obtaining `Board`'s status
    void notifyObservers(int players_turn);
    void sendMessage(string message);
    void endGame(int winner);
};

class FirewallInfo {
    int x, y;
    char c;
public:
    FirewallInfo(int x, int y, char c);
    friend class Board;
    friend class GraphicalDisplay;
};

class Board : public Subject {
    vector<vector<Tile>> tiles;
    vector<Link> link1;
    vector<Link> link2;
    Player player1, player2;

    vector<FirewallInfo> firewallInfo;

public:
    // Sample string for link1/link2: "V1 D4 V3 V2 D3 V4 D2 D1"
    // Randomize the order if not given
    Board(string link1_string, string link2_string, string ability1, string ability2);

    // return whether a move has been successfully made
    bool move(char link, Direction dir);
    void battle(char l1, char l2, int initiator);
    bool isEmpty(int x, int y) const;
    void render(int players_turn);
    void download(DownloadStatus status, const Link& link);

    pair<int, int> getCoords(char link) const;
    Player& getPlayer1();
    Player& getPlayer2();
    Link& getLink(char link);
    Tile& getTile(int x, int y);

    void addFirewall(int x, int y, char c);
    vector<FirewallInfo>& getFirewallInfo();
};

#endif // BOARD_H_
