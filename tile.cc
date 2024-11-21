#include "tile.h"

Tile::Tile(char c, int firewall) {}

char Tile::getChar() const {
    if (firewall == 0 || c != '.') {
        return c;
    }
    if (firewall == 1) {
        return 'm';
    }
    return 'w';
}

void Tile::setChar(char c) { c = c; }
void Tile::setFirewall(int f) { firewall = f; }
bool Tile::isEmpty() const { return c == '.'; }
