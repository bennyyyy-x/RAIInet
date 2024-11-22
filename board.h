#ifndef BOARD_H_
#define BOARD_H_

#include <memory>
#include <vector>
#include <string>
#include "display.h"
#include "constants.h"
#include "tile.h"
#include "link.h"

using namespace std;

class Subject {
    vector<weak_ptr<Observer>> observers;

public:
    void attach(weak_ptr<Observer> ptr);
    void detach(weak_ptr<Observer> ptr);

    // Calls the `notify` function of Observers, then the displays will
    // update itself by obtaining `Board`'s status
    void notifyObservers();
};

class Board : public Subject {
    vector<vector<Tile>> tiles;
    vector<Link> link1;
    vector<Link> link2;

public:
    // Sample string for link1/link2: "V1 D4 V3 V2 D3 V4 D2 D1"
    // Randomize the order if not given
    Board(string link1_string = "", string link2_string = "");

    pair<int, int> getCoords(char link);

    void move (char link, int dir);
    int battle (char l1, char l2, int initiator); //what should this return???
    bool isEmpty (int x, int y);
    void render() const;
};

#endif // BOARD_H_
