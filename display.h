#ifndef DISPLAY_H_
#define DISPLAY_H_
#include "board.h"
#include "window.h"
#include <memory>

using namespace std;

class Observer {
    public:
        virtual void notify(int players_turn) = 0;
};

class TextDisplay : public Observer {
    shared_ptr<Board> b;
    public:
        //TODO make ctor
        void notify(int players_turn) override;
};

class Graphical :  public Observer {
    Board *b;
    Xwindow w;
    public:
        //TODO make ctor
        void notify(int players_turn) override;
};

#endif // DISPLAY_H_
