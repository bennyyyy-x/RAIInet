#include "link.h"

Link::Link(char c, int strength, bool isData, int x, int y) 
    : c{c}, strength{strength}, isData{isData}, x{x}, y{y} {}

//getters
char Link::getChar() const { return c; }
int Link::getStrength() const { return strength; }
bool Link::getType() const { return isData; }
bool Link::getReveal() const { return isRevealed; }
bool Link::getBoost() const { return getBoost; }
int Link::getX() const { return x; }
int Link::getY() const { return y; }
//setters
void Link::setStrength(int s) { strength = s; }
void Link::setX(int x) { x = x; }
void Link::setY(int y) { y = y; }

//abilities
void Link::boost() { isBoosted = true; }
void Link::polarize() { isData = !isData; }
void Link::reveal() { isRevealed = true; }