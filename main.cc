#include "link.h"
#include "tile.h"
#include "board.h"
#include "player.h"
#include "ability.h"
#include "display.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;



int main(int argc, char* argv[]) {
    string ability1 = "LFDSP", ability2 = "LFDSP";
    string link1 = "", link2 = "";
    bool graphical_display = false;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-ability1" && i + 1 < argc) {
            ability1 = argv[++i];
        } else if (arg == "-ability2" && i + 1 < argc) {
            ability2 = argv[++i];
        } else if (arg == "-link1" && i + 1 < argc) {
            ifstream fin{argv[++i]};
            string tmp;
            while (getline(fin, tmp)) {
                link1 += tmp;
            }
        } else if (arg == "-link2" && i + 1 < argc) {
            ifstream fin{argv[++i]};
            string tmp;
            while (getline(fin, tmp)) {
                link2 += tmp;
            }
        } else if (arg == "-graphics") {
            graphical_display = true;
        }
    }

    shared_ptr<Board> board = make_shared<Board>(link1, link2, ability1, ability2);
    if (!graphical_display) {
        shared_ptr<TextDisplay> textDisplay = make_shared<TextDisplay>(board);
        board->attach(textDisplay);
    } // TODO graphical display

    board->render(1);
    board->render(2);

}
