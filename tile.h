#ifndef TILE_H_
#define TILE_H_

#include "constants.h"

class Tile {
    // a-g, A-G, S, .
    char c;
    FirewallStatus firewall;

public:
    Tile(char c = '.', FirewallStatus firewall = FirewallStatus::NoFirewall);
    char getChar() const;
    bool isEmpty() const;
    void setChar(char new_c);
    void setFirewall(FirewallStatus f);
    FirewallStatus getFirewall() const;
};

#endif // TILE_H_
