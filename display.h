#ifndef DISPLAY_H_
#define DISPLAY_H_
#include "board.h"
#include "window.h"
#include <vector>
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
    shared_ptr<Board> b;
    Xwindow w;

    class Info {
        int x, y;
        bool downloaded;
        bool revealed;
        friend class GraphicalDisplay;
    public:
        Info(int x, int y, bool downloaded, bool revealed);
    };

    vector<Info> linkInfo;
    vector<string> playerInfo;

    Info getInfo(char link);
    void updateCoord(char link, int x, int y, bool downloaded, bool revealed);

public:
    // Constructor should draw out the base board
    GraphicalDisplay(shared_ptr<Board> b, int width=500, int height=500);

    // notify should compare the coordinates of each link, and redraw only if the coordinates changed
    // also depending on players_turn, cover or show the type and strength of links
    void notify(int players_turn) override;
    void message(string msg) override;

    string playerDisplayInfo(Player& player, int info_type, int players_turn);
    void updateTile(int x, int y, char link, bool isRevealed = false, bool isData = false);
};

#endif // DISPLAY_H_
