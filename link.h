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
};

#endif // LINK_H_
