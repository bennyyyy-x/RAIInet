#ifndef LINK_H_
#define LINK_H_

#include <string>
#include "constants.h"

using namespace std;

class Link {
    char c;
    int strength;
    bool isData;
    bool boosted = false;
    bool revealed = false;
    bool downloaded = false;
    int x, y;

public:
    Link(char c, int strength, bool isData, int x, int y);

    //getters
    char getChar() const;
    int getStrength() const;
    bool getType() const;
    bool isRevealed() const;
    bool isBoosted() const;
    int getX() const;
    int getY() const;
    
    //setters
    void setStrength(int s);
    void setX(int x);
    void setY(int y);

    //abilities
    void boost();
    void polarize();
    void reveal();

    // Return false if move was unable to be made
    bool move(Direction dir);
};

#endif // LINK_H_
