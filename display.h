#ifndef DISPLAY_H_
#define DISPLAY_H_
#include "board.h"
#include "window.h"
#include <memory>

using namespace std;

class Observer {
public:
    virtual void notify(int players_turn) = 0;
    virtual void message(string msg) = 0;
};

class TextDisplay : public Observer {
    shared_ptr<Board> b;
public:
    TextDisplay(shared_ptr<Board> b);
    void notify(int players_turn) override;
    void message(string msg) override;
};

class GraphicalDisplay : public Observer {
    Board *b;
    Xwindow w;
public:
    //TODO make ctor
    void notify(int players_turn) override;
    void message(string msg) override;

};

#endif // DISPLAY_H_
