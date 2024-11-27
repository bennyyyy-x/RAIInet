#include "ability.h"
#include "board.h"
#include "constants.h"

AbilityName Ability::getName() const { return name; }
bool Ability::isUsed() const{ return used; }
void Ability::setUsed() { used = true; }

//Returns false if selected link to boost is already downloaded
bool LinkBoost::activate(Player& player, Board& board, vector<string> params) {
    char link = params[0][0];
    if (board.getLink(link).downloadStatus() != NotDownloaded) {
        return false;
    }
    board.getLink(link).boost();
    return true;
}

//Coordinates alr checked, is any case possible to return false?
bool Firewall::activate(Player& player, Board& board, vector<string> params) {
    int x = params[0][0], y = params[1][0];
    FirewallStatus wallOwner;
    if (player.playerid == 1) {
        wallOwner = Player1s;
    } else {
        wallOwner = Player2s;
    }
    board.getTile(x, y).setFirewall(wallOwner);

    return true;
}

//Returns false if link to download is already downloaded
bool Download::activate(Player& player, Board& board, vector<string> params) {
    char link = params[0][0];
    if (board.getLink(link).downloadStatus() != NotDownloaded) {
        return false;
    }
    DownloadStatus ds;
    if (player.playerid == 1) {
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

bool Polarize::activate(Player& player, Board& board, vector<string> params) {
    char link = params[0][0];
    if (board.getLink(link).downloadStatus() != NotDownloaded) {
        return false;
    }
    board.getLink(link).polarize();
    return true;
}

bool Scan::activate(Player& player, Board& board, vector<string> params) {
    char link = params[0][0];
    board.getLink(link).reveal();
    return true;
}

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
}
