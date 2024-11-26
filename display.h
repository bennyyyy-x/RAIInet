#ifndef DISPLAY_H_
#define DISPLAY_H_
#include "board.h"
#include "window.h"

class Observer {
    public:
        virtual void notify() = 0;
};

class TextDisplay : public Observer {
    Board *b;
    public:
        //TODO make ctor
        void notify() override;
};

class Graphical :  public Observer {
    Board *b;
    Xwindow w;
    public:
        //TODO make ctor
        void notify() override;
};

#endif // DISPLAY_H_
