#include "ability.h"
#include "board.h"
#include "constants.h"
#include <cstdlib>
#include <ctime>

using namespace std;

Ability::Ability(AbilityName name) : used{false}, name{name} {}
AbilityName Ability::getName() const { return name; }
bool Ability::isUsed() const{ return used; }
void Ability::setUsed() { used = true; }

LinkBoost::LinkBoost() : Ability{AbilityName::LINKBOOST} {}

//Returns false if selected link to boost is already downloaded
bool LinkBoost::activate(Player& player, Board& board, vector<string> params) {
    char link = params[0][0];
    if (board.getLink(link).downloadStatus() != NotDownloaded) {
        return false;
    }
    board.getLink(link).boost();
    return true;
}

Firewall::Firewall() : Ability{AbilityName::FIREWALL} {}

bool Firewall::activate(Player& player, Board& board, vector<string> params) {
    int x = params[0][0], y = params[1][0];
    FirewallStatus wallOwner;
    if (player.getPlayerId() == 1) {
        wallOwner = Player1s;
    } else {
        wallOwner = Player2s;
    }
    board.getTile(x, y).setFirewall(wallOwner);

    return true;
}

Download::Download() : Ability{AbilityName::DOWNLOAD} {}

//Returns false if link to download is already downloaded
bool Download::activate(Player& player, Board& board, vector<string> params) {
    char link = params[0][0];
    if (board.getLink(link).downloadStatus() != NotDownloaded) {
        return false;
    }
    DownloadStatus ds;
    if (player.getPlayerId() == 1) {
        ds = ByPlayer1;
    } else {
        ds = ByPlayer2;
    }
    board.getLink(link).setDownload(ds); //set link to downloaded
    player.download(board.getLink(link)); //update player's D/V
    auto coords = board.getCoords(link);
    board.getTile(coords.first, coords.second).setChar('.'); //clear tile
    return true;
}

Polarize::Polarize() : Ability{AbilityName::POLARIZE} {}

bool Polarize::activate(Player& player, Board& board, vector<string> params) {
    char link = params[0][0];
    if (board.getLink(link).downloadStatus() != NotDownloaded) {
        return false;
    }
    board.getLink(link).polarize();
    return true;
}

Scan::Scan() : Ability{AbilityName::SCAN} {}

bool Scan::activate(Player& player, Board& board, vector<string> params) {
    char link = params[0][0];
    board.getLink(link).reveal();
    return true;
}

Exchange::Exchange() : Ability{AbilityName::EXCHANGE} {}

bool Exchange::activate(Player& player, Board& board, vector<string> params) {
    char l1 = params[0][0], l2 = params[1][0];
    DownloadStatus ds1 = board.getLink(l1).downloadStatus();
    DownloadStatus ds2 = board.getLink(l1).downloadStatus();

    if (ds1 != NotDownloaded || ds2 != NotDownloaded) {
        return false;
    }
    auto c1 = board.getCoords(l1), c2 = board.getCoords(l2);

    board.getTile(c1.first, c1.second).setChar(l2);
    board.getTile(c2.first, c2.second).setChar(l1);

    board.getLink(l1).setX(c2.first);
    board.getLink(l1).setY(c2.second);
    board.getLink(l2).setX(c1.first);
    board.getLink(l2).setY(c1.second);

    return true;
}

Teleport::Teleport() : Ability{AbilityName::TELEPORT} {}

//Teleports a link to a random empty (no firewall) valid tile
bool Teleport::activate(Player& player, Board& board, vector<string> params) {
    char link = params[0][0];
    if (board.getLink(link).downloadStatus() != NotDownloaded) {
        return false;
    }

    vector<pair<int, int>> emptyTiles;

    for (int i = 0; i < BOARD_WIDTH; ++i) {
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            char checkTile = board.getTile(i, j).getChar();
            auto checkWall = board.getTile(i, j).getFirewall();
            if (checkTile == '.' && checkWall == NoFirewall) {
                emptyTiles.emplace_back(i, j);
            }
        }
    }
    if (emptyTiles.empty()) {
        return false;
    }

    srand(time(0));
    int randIndex = rand() % emptyTiles.size();
    pair<int, int> target = emptyTiles[randIndex];
    pair<int, int> coords = board.getCoords(link);

    board.getTile(coords.first, coords.second).setChar('.');
    board.getTile(target.first, target.second).setChar(board.getLink(link).getChar());
    board.getLink(link).setX(target.first);
    board.getLink(link).setY(target.second);
    return true;
}

Hijack::Hijack() : Ability{AbilityName::HIJACK} {}

bool Hijack::activate(Player& player, Board& board, vector<string> params) {
    return true;
}
