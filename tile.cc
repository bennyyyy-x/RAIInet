#include "tile.h"

Tile::Tile(char c, int firewall) : c{c}, firewall{firewall} {}

char Tile::getChar() const {
    if (firewall == 0 || c != '.') {
        return c;
    }
    if (firewall == 1) {
        return 'm';
    }
    return 'w';
}
bool Tile::isEmpty() const { return c == '.'; }
void Tile::setChar(char new_c) { c = new_c; }
void Tile::setFirewall(int f) { firewall = f; }
