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
    DownloadStatus downloaded = NotDownloaded;
    int x, y;

public:
    Link(char c, int strength, bool isData, int x, int y);

    //getters
    char getChar() const;
    int getStrength() const;
    bool getType() const;
    bool isRevealed() const;
    bool isBoosted() const;
    DownloadStatus downloadStatus() const;
    int getX() const;
    int getY() const;
    
    //setters
    void setStrength(int s);
    void setX(int x);
    void setY(int y);
    void setDownload(DownloadStatus ds);

    //abilities
    void boost();
    void polarize();
    void reveal();
};

#endif // LINK_H_
