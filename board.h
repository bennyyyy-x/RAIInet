#ifndef BOARD_H_
#define BOARD_H_

#include <memory>
#include <vector>
#include "display.h"

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

};

#endif // BOARD_H_