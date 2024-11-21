#include "tile.h"

char Tile::getChar() const {
    if (firewall == 0 || c != '.') {
        return c;
    }
    if (firewall == 1) {
        return 'm';
    }
    return 'w';
}
