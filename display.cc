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

void TextDisplay::end(int winner) {
    cout << "WINNER: PLAYER " << winner << endl;
}

int convertX(int x) {
    return x * TILE_WIDTH + (x + 1) * LINE_WIDTH + BOARD_CORNER_X;
}

int convertY(int y) {
    y = 7 - y;
    return y * TILE_WIDTH + (y + 1) * LINE_WIDTH + BOARD_CORNER_Y;
}


GraphicalDisplay::Info::Info(int x, int y, bool downloaded, bool revealed, bool isData)
    : x{x}, y{y}, downloaded{downloaded}, revealed{revealed}, isData{isData} {}


GraphicalDisplay::Info GraphicalDisplay::getInfo(char link) {
    if (isPlayer1Link(link)) {
        return linkInfo[link - 'a'];
    } else {
        return linkInfo[link - 'A' + 8];
    }
}


void GraphicalDisplay::updateCoord(char link, int x, int y, bool downloaded, bool revealed) {
    if (isPlayer1Link(link)) {
        linkInfo[link - 'a'].x = x;
        linkInfo[link - 'a'].y = y;
        linkInfo[link - 'a'].downloaded = downloaded;
        linkInfo[link - 'a'].revealed = revealed;
    } else {
        linkInfo[link - 'A' + 8].x = x;
        linkInfo[link - 'A' + 8].y = y;
        linkInfo[link - 'A' + 8].downloaded = downloaded;
        linkInfo[link - 'A' + 8].revealed = revealed;
    }
}


GraphicalDisplay::GraphicalDisplay(shared_ptr<Board> b, int width, int height) : b{b}, w{width, height} {
    for (int i = 0; i < 8; ++i) {
        Link& link = b->getLink(char('a' + i));
        linkInfo.push_back({link.getX(),
                            link.getY(),
                            link.downloadStatus() != DownloadStatus::NotDownloaded,
                            link.isRevealed(),
                            link.getIsData()});
    }
    for (int i = 0; i < 8; ++i) {
        Link& link = b->getLink(char('A' + i));
        linkInfo.push_back({link.getX(),
                            link.getY(),
                            link.downloadStatus() != DownloadStatus::NotDownloaded,
                            link.isRevealed(),
                            link.getIsData()});
    }

    w.fillRectangle(0, 0, BOARD_WIDTH_GRAPH, 30, Xwindow::Black);
    w.drawString(70, 20 , "PLAYER 1", Xwindow::White);

    for (int i = 0; i < 8; ++i) {
        if (i < 4) {
            playerInfo.push_back(playerDisplayInfo(b->getPlayer1(), i, 1));
        } else {
            playerInfo.push_back(playerDisplayInfo(b->getPlayer2(), i - 4, 1));
        }
        w.drawString(PLAYER_INFO_COORD[i][0], PLAYER_INFO_COORD[i][1], playerInfo[i], Xwindow::Black);
    }

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
}


void GraphicalDisplay::updateTile(int x, int y, char link, bool isRevealed, bool isData) {
    if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_WIDTH) {
        return;
    }
    if (link == '.') { // empty tile
        w.fillRectangle(convertX(x), convertY(y), TILE_WIDTH, TILE_WIDTH, Xwindow::White);
        return;
    }
    if (!isLink(link) && link != 'w' && link != 'm') {
        return;
    }
    w.fillRectangle(convertX(x), convertY(y), TILE_WIDTH, TILE_WIDTH, Xwindow::White);
    int color = Xwindow::Black;
    if (link == 'w' || link == 'm') {
        color = Xwindow::Blue;
    } else if (isRevealed) {
        if (isData) {
            color = Xwindow::Green;
        } else {
            color = Xwindow::Red;
        }
        //TODO after a link is revealed - data is correctly displayed as green but virus not displayed as red
        //TODO mayhaps, calling board right after link is scanned does not update the colours
    }
    w.drawString(convertX(x) + CHAR_OFF_X, convertY(y) + CHAR_OFF_Y, string(1, link), color);
}


string GraphicalDisplay::playerDisplayInfo(Player& player, int info_type, int players_turn) {
    string txt = "";
    if (info_type == 0) { //Downloaded links info
        txt += "DOWNLOADED:   ";
        txt += to_string(player.getData()) + "D, ";
        txt += to_string(player.getVirus()) + "V";
        return txt;
    } else if (info_type == 1) { //Num abilities left info
        txt += "ABILITIES:    " + to_string(player.getNumAbilities());
        return txt;
    } else { //Displaying links info
        char start, end;
        if (info_type == 2) { //Links display first line (a-d)
            start = 'a';
            end = 'd';
        } else {              //Links display 2nd line (e-h)
            start = 'e';
            end = 'h';
        }
        //If printing for player 2, convert to uppercase
        if (player.getPlayerId() == 2) {
            start = toupper(start);
            end = toupper(end);
        }

        for (char c = start; c <= end; ++c) {
            txt += string(1, c) + ": ";

            bool revealSafe = false;
            if (players_turn == player.getPlayerId()) { //if link owners turn
                revealSafe = true;
            } else if (b->getLink(c).isRevealed()) { //or if link is revealed
                revealSafe = true;
            }
            if (revealSafe) {
                if (b->getLink(c).getIsData()) {
                    txt += "D";
                } else {
                    txt += "V";
                }
                txt += to_string(b->getLink(c).getStrength()) + " ";
            } else {
                txt += "?  ";
            }
        }
    }
    return txt;
}

bool GraphicalDisplay::noLinkOnSquare(int x, int y) const {
    for (Info linkInfo : linkInfo) {
        if (x == linkInfo.x && y == linkInfo.y && linkInfo.downloaded == false) {
            return false;
        }
    }
    return true;
}

void GraphicalDisplay::notify(int players_turn) {
    if (players_turn == 1) {
        w.drawString(70, 20 , "PLAYER 1", Xwindow::Red);
        w.drawString(70, 120 + BOARD_WIDTH_GRAPH + BOARD_CORNER_Y, "PLAYER 2", Xwindow::White);
    } else {
        w.drawString(70, 120 + BOARD_WIDTH_GRAPH + BOARD_CORNER_Y, "PLAYER 2", Xwindow::Red);
        w.drawString(70, 20 , "PLAYER 1", Xwindow::White);
    }

    for (FirewallInfo info : b->getFirewallInfo()) {
        if (noLinkOnSquare(info.x, info.y)) {
            updateTile(info.x, info.y, info.c);
        }
    }

    for (int i = 0; i < BOARD_WIDTH * 2; ++i) {
        Info& info = linkInfo[i];
        Link& link = i < 8 ? b->getLink(char('a' + i)) : b->getLink(char('A' + i - 8));
        if (info.downloaded) {
            continue;
        }
        if (link.downloadStatus() != DownloadStatus::NotDownloaded) {
            info.downloaded = true;
            updateTile(info.x, info.y, '.');
            continue;
        }
    }

    for (int i = 0; i < BOARD_WIDTH * 2; ++i) {
        Info& info = linkInfo[i];
        Link& link = i < 8 ? b->getLink(char('a' + i)) : b->getLink(char('A' + i - 8));
        if (info.downloaded) {
            continue;
        }
        if (info.x != link.getX() || info.y != link.getY() || info.revealed != link.isRevealed() || info.isData != link.getIsData()) {
            int x = info.x, y = info.y;
            info.x = link.getX();
            info.y = link.getY();
            info.revealed = link.isRevealed();
            info.isData = link.getIsData();
            if (noLinkOnSquare(x, y)) {
                updateTile(x, y, '.');
            }
            updateTile(link.getX(), link.getY(), link.getChar(), link.isRevealed(), link.getIsData());   
        }
    }

    //Update Player information
    for (int i = 0; i < 8; ++i) {
        string updated = playerDisplayInfo(i < 4 ? b->getPlayer1() : b->getPlayer2(), i % 4, players_turn);
        if (updated != playerInfo[i]) {
            playerInfo[i] = updated;
            w.fillRectangle(PLAYER_INFO_COORD[i][0], PLAYER_INFO_COORD[i][1] - 10, BOARD_WIDTH_GRAPH, 15, Xwindow::White);
            w.drawString(PLAYER_INFO_COORD[i][0], PLAYER_INFO_COORD[i][1], playerInfo[i], Xwindow::Black);
        }
    }
}


void GraphicalDisplay::message(string msg) {
    cout << msg << endl;
}

void GraphicalDisplay::end(int winner) {
    w.fillRectangle(0, convertY(5), BOARD_WIDTH_GRAPH, 50, Xwindow::Red);
}
