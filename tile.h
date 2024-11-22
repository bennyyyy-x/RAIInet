#ifndef TILE_H_
#define TILE_H_

class Tile {
    // a-g, A-G, S, .
    char c;
    // 0: no firewall, 1: player 1's firewall, 2: player 2's firewall
    int firewall;

public:
    Tile(char c = '.', int firewall = 0);
    char getChar() const;
    void setChar(char c);
    void setFirewall(int f);
    bool isEmpty() const;
};

#endif TILE_H_
