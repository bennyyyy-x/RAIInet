#ifndef CONSTANTS_H_
#define CONSTANTS_H_

const int BOARD_WIDTH = 8;

enum Direction { Up = 0, Down, Left, Right };

enum DownloadStatus { NotDownloaded = 0, ByPlayer1, ByPlayer2 };

enum AbilityName {
    LinkBoost = 0,
    Firewall,
    Download,
    Polarize,
    Scan,
    Exchange,
    Teleport,
    Hijack
};

const int ABILITY_CARD_NUMS = 5;

enum FirewallStatus { NoFirewall = 0, Player1s, Player2s };

enum MoveStatus { IllegalMove = 0, NeedTileChange, DoNotNeedTileChange };

const int BOARD_CORNER_X = 0, BOARD_CORNER_Y = 125;
const int TILE_WIDTH = 20, LINE_WIDTH = 2;
const int BOARD_WIDTH_GRAPH = BOARD_WIDTH * TILE_WIDTH + (BOARD_WIDTH + 1) * LINE_WIDTH;
const int CHAR_OFF_X = 7, CHAR_OFF_Y = 15;

const int PLAYER_INFO_COORD[8][2] = {
    {10, 50},
    {10, 70},
    {10, 90},
    {10, 110},
    {10, 30 + BOARD_WIDTH_GRAPH + BOARD_CORNER_Y},
    {10, 50 + BOARD_WIDTH_GRAPH + BOARD_CORNER_Y},
    {10, 70 + BOARD_WIDTH_GRAPH + BOARD_CORNER_Y},
    {10, 90 + BOARD_WIDTH_GRAPH + BOARD_CORNER_Y}
};

#endif // CONSTANTS_H_
