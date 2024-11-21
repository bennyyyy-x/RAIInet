#ifndef LINK_H_
#define LINK_H_

#include <string>

using namespace std;

class Link {
    char c;
    int strength;
    bool isData;
    bool isRevealed = false;
    int x, y;

public:
    Link(char c, int strength, bool isData, int x, int y);
    char getChar() const;
    int getStrength() const;
    bool getType() const;
    bool getReveal() const;
    int getX() const;
    int getY() const;

    void setStrength();
    void setType();
    void setReveal();
    void setX();
    void setY();
};

#endif // LINK_H_
