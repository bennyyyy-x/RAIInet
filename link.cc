#include "link.h"

Link::Link(char c, int strength, bool isData, int x, int y) 
    : c{c}, strength{strength}, isData{isData}, x{x}, y{y} {}

//getters
char Link::getChar() const { return c; }
int Link::getStrength() const { return strength; }
bool Link::getType() const { return isData; }
bool Link::isRevealed() const { return revealed; }
bool Link::isBoosted() const { return boosted; }
int Link::getX() const { return x; }
int Link::getY() const { return y; }

//setters
void Link::setStrength(int s) { strength = s; }
void Link::setX(int x) { this->x = x; }
void Link::setY(int y) { this->y = y; }

//abilities
void Link::boost() { boosted = true; }
void Link::polarize() { isData = !isData; }
void Link::reveal() { revealed = true; }

int convertToY(Direction dir) {
    if (dir == Direction::UP) {
        return 1;
    } else if (dir == Direction::DOWN) {
        return -1;
    }
    return 0;
}

int convertToX(Direction dir) {
    if (dir == Direction::RIGHT) {
        return 1;
    } else if (dir == Direction::LEFT) {
        return -1;
    }
    return 0;
}

bool Link::move(Direction dir) {
    // Cannot move downloaded link
    if (downloaded) {
        return false;
    }

    int tmp_x = x + convertToX(dir), tmp_y = y + convertToY(dir);
    // Moves off side edge (Illegal)
    if (tmp_x < 0 || tmp_x > 7) {
        return false;
    }

    // TODO side edge
    
    if (tmp_y < 0) { // Moves off bottom edge
        if (isupper(c)) { // Illegal
            return false;
        }
        // Player 1 should download this link
        downloaded = true;
        // TODO think how to make player 1 download the link, how to call the download method
    } else if (tmp_y > 7) { // Moves off top edge
        if (islower(c)) { // Illegal
            return false;
        }
        // Player 2 should download this link
        downloaded = true;
        // TODO think how to make player 2 download the link, how to call the download method
    }

    x = tmp_x;
    y = tmp_y;
    return true;
}