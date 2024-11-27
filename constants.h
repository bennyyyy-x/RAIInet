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

enum FirewallStatus { NoFirewall = 0, ByPlayer1, ByPlayer2 };

#endif // CONSTANTS_H_
