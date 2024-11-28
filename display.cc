#include "display.h"
#include "board.h"
#include "constants.h"
#include <iostream>
#include <string>
#include <memory>

using namespace std;

TextDisplay::TextDisplay(shared_ptr<Board> b) : b{b} {}

void TextDisplay::notify(int players_turn) {
    cout << "Player 1:" << endl;
    cout << "Downloaded: ";
    cout << b->getPlayer1().getData() << "D, " << b->getPlayer1().getVirus() << "V" << endl;
    cout << "Abilities: " << b->getPlayer1().getNumAbilities() << endl;

    //Prints out links 
    for (char c = 'a'; c <= 'h'; c++) {
        cout << c << ": ";
        //Display link if:
        //  - It is the link owner's turn
        //  - It is the opponent's turn, but the link is revealed
        if (players_turn == 1 || (players_turn == 2 && b->getLink(c).isRevealed())) {
            if (b->getLink(c).getIsData()) { //If is data
                cout << "D";
            } else {
                cout << "V";
            }
            cout << b->getLink(c).getStrength() << " ";
        } else {
            cout << "?  ";
        }

        if (c == 'd' || c == 'h') {
            cout << endl;
        }
    }
    //Prints out board
    for (int i = 0; i < BOARD_WIDTH; ++i) {
        cout << "=";
    }
    cout << endl;
    for (int y = BOARD_WIDTH - 1; y >= 0; --y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            cout << b->getTile(x, y).getChar();
        }
        cout << endl;
    }
    for (int i = 0;  i < BOARD_WIDTH; ++i) {
        cout << "=";
    }
    cout <<  endl;

    cout << "Player 2:" << endl;
    cout << "Downloaded: ";
    cout << b->getPlayer2().getData() << "D, " << b->getPlayer2().getVirus() << "V" << endl;
    cout << "Abilities: " << b->getPlayer2().getNumAbilities() << endl;

    //Prints out links 
    for (char c = 'A'; c <= 'H'; c++) {
        cout << c << ": ";
        if (players_turn == 2 || (players_turn == 1 && b->getLink(c).isRevealed())) {
            if (b->getLink(c).getIsData()) {
                cout << "D";
            } else {
                cout << "V";
            }
            cout << b->getLink(c).getStrength() << " ";
        } else {
            cout << "?  ";
        }
        if (c == 'D' || c == 'H') {
            cout << endl;
        }
    }
}

void TextDisplay::message(string message) {
    cout << message << endl;
}

int convertX(int x) {
    return x * TILE_WIDTH + (x + 1) * LINE_WIDTH + BOARD_CORNER_X;
}

int convertY(int y) {
    y = 7 - y;
    return y * TILE_WIDTH + (y + 1) * LINE_WIDTH + BOARD_CORNER_Y;
}

GraphicalDisplay::Info::Info(int x, int y, bool downloaded) : x{x}, y{y}, downloaded{downloaded} {}


GraphicalDisplay::Info GraphicalDisplay::getInfo(char link) {
    if (isPlayer1Link(link)) {
        return linkInformation[link - 'a'];
    } else {
        return linkInformation[link - 'A' + 8];
    }
}


void GraphicalDisplay::updateCoord(char link, int x, int y, bool downloaded) {
    if (isPlayer1Link(link)) {
        linkInformation[link - 'a'].x = x;
        linkInformation[link - 'a'].y = y;
        linkInformation[link - 'a'].downloaded = downloaded;
    } else {
        linkInformation[link - 'A' + 8].x = x;
        linkInformation[link - 'A' + 8].y = y;
        linkInformation[link - 'A' + 8].downloaded = downloaded;
    }
}


GraphicalDisplay::GraphicalDisplay(shared_ptr<Board> b, int width, int height) : b{b}, w{width, height} {
    for (int i = 0; i < 8; ++i) {
        Link& link = b->getLink(char('a' + i));
        linkInformation.push_back({link.getX(), link.getY(), link.downloadStatus() != DownloadStatus::NotDownloaded});
    }
    for (int i = 0; i < 8; ++i) {
        Link& link = b->getLink(char('A' + i));
        linkInformation.push_back({link.getX(), link.getY(), link.downloadStatus() != DownloadStatus::NotDownloaded});
    }

    w.fillRectangle(0, 0, BOARD_WIDTH_GRAPH, 30, Xwindow::Black);
    w.drawString(70, 20 , "PLAYER 1", Xwindow::White);
    w.drawString(10, 50, "DOWNLOADED:   1D, 3V", Xwindow::Black);
    w.drawString(10, 70, "ABILITIES:    5", Xwindow::Black);
    w.drawString(10, 90, "a: V1 b: D4 c: V3 d: V2", Xwindow::Black);
    w.drawString(10, 110, "e: D3 f: V4 g: D2 h: D1", Xwindow::Black);

    w.fillRectangle(BOARD_CORNER_X, BOARD_CORNER_Y - 5, BOARD_WIDTH_GRAPH, 5, Xwindow::Black); // Top edge
    w.fillRectangle(BOARD_CORNER_X, BOARD_CORNER_Y, BOARD_WIDTH_GRAPH, BOARD_WIDTH_GRAPH, Xwindow::Black); // Entire board

    for (int x = 0; x < BOARD_WIDTH; ++x) {
        for (int y = 0; y < BOARD_WIDTH; ++y) {
            if ((x == 3 || x == 4) && (y == 0 || y == 7)) {
                w.fillRectangle(convertX(x), convertY(y), TILE_WIDTH, TILE_WIDTH, Xwindow::Blue); // Server port
                continue;
            }
            char c = b->getTile(x, y).getChar();
            Link& link = b->getLink(c);
            updateTile(x, y, c, link.isRevealed(), link.getIsData());
        }
    }

    w.fillRectangle(BOARD_CORNER_X, BOARD_CORNER_Y + BOARD_WIDTH_GRAPH, BOARD_WIDTH_GRAPH, 5, Xwindow::Black); // Bottom edge

    w.fillRectangle(0, 100 + BOARD_WIDTH_GRAPH + BOARD_CORNER_Y, BOARD_WIDTH_GRAPH, 30, Xwindow::Black);
    w.drawString(70, 120 + BOARD_WIDTH_GRAPH + BOARD_CORNER_Y, "PLAYER 2", Xwindow::White);
    w.drawString(10, 30 + BOARD_WIDTH_GRAPH + BOARD_CORNER_Y, "DOWNLOADED:   1D, 3V", Xwindow::Black);
    w.drawString(10, 50 + BOARD_WIDTH_GRAPH + BOARD_CORNER_Y, "ABILITIES:    5", Xwindow::Black);
    w.drawString(10, 70 + BOARD_WIDTH_GRAPH + BOARD_CORNER_Y, "a: V1 b: D4 c: V3 d: V2", Xwindow::Black);
    w.drawString(10, 90 + BOARD_WIDTH_GRAPH + BOARD_CORNER_Y, "e: D3 f: V4 g: D2 h: D1", Xwindow::Black);

}

void GraphicalDisplay::updateTile(int x, int y, char link, bool isRevealed, bool isData) {
    if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_WIDTH) {
        return;
    }
    if (link == '.') { // empty tile
        w.fillRectangle(convertX(x), convertY(y), TILE_WIDTH, TILE_WIDTH, Xwindow::White);
        return;
    }
    if (!isLink(link)) {
        return;
    }
    w.fillRectangle(convertX(x), convertY(y), TILE_WIDTH, TILE_WIDTH, Xwindow::White);
    int color = Xwindow::Black;
    if (isRevealed) {
        if (isData) {
            color = Xwindow::Green;
        } else {
            color = Xwindow::Red;
        }
    }
    w.drawString(convertX(x) + CHAR_OFF_X, convertY(y) + CHAR_OFF_Y, string(1, link), color);
}

void GraphicalDisplay::notify(int players_turn) {
    cout << "In Graphical Notify" << endl;
    if (players_turn == 1) {
        w.drawString(70, 20 , "PLAYER 1", Xwindow::Red);
        w.drawString(70, 120 + BOARD_WIDTH_GRAPH + BOARD_CORNER_Y, "PLAYER 2", Xwindow::White);
    } else {
        w.drawString(70, 120 + BOARD_WIDTH_GRAPH + BOARD_CORNER_Y, "PLAYER 2", Xwindow::Red);
        w.drawString(70, 20 , "PLAYER 1", Xwindow::White);
    }

    for (int i = 0; i < BOARD_WIDTH; ++i) {
        Info& info = linkInformation[i];
        Link& link = b->getLink(char('a' + i));
        if (info.downloaded) {
            continue;
        }
        if (link.downloadStatus() != DownloadStatus::NotDownloaded) {
            info.downloaded = true;
            updateTile(info.x, info.y, '.');
            continue;
        }
        if (info.x != link.getX() || info.y != link.getY()) {
            updateTile(info.x, info.y, '.');
            updateTile(link.getX(), link.getY(), link.getChar(), link.isRevealed(), link.getIsData());
            info.x = link.getX();
            info.y = link.getY();
        }
    }

    for (int i = 0; i < BOARD_WIDTH; ++i) {
        Info& info = linkInformation[i + 8];
        Link& link = b->getLink(char('A' + i));
        if (info.downloaded) {
            continue;
        }
        if (link.downloadStatus() != DownloadStatus::NotDownloaded) {
            info.downloaded = true;
            updateTile(info.x, info.y, '.');
            continue;
        }
        if (info.x != link.getX() || info.y != link.getY()) {
            updateTile(info.x, info.y, '.');
            updateTile(link.getX(), link.getY(), link.getChar(), link.isRevealed(), link.getIsData());
            info.x = link.getX();
            info.y = link.getY();
        }
    }
}

void GraphicalDisplay::message(string msg) {
    cout << msg << endl;
}
