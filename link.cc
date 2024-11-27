#include "link.h"

Link::Link(char c, int strength, bool isData, int x, int y) 
    : c{c}, strength{strength}, isData{isData}, x{x}, y{y} {}

//getters
char Link::getChar() const { return c; }
int Link::getStrength() const { return strength; }
bool Link::getIsData() const { return isData; }
bool Link::isRevealed() const { return revealed; }
bool Link::isBoosted() const { return boosted; }
DownloadStatus Link::downloadStatus() const { return downloaded; }
int Link::getX() const { return x; }
int Link::getY() const { return y; }

//setters
void Link::setStrength(int s) { strength = s; }
void Link::setX(int x) { this->x = x; }
void Link::setY(int y) { this->y = y; }
void Link::setDownload(DownloadStatus ds) {
    downloaded = ds;
    if (ds != NotDownloaded) {
        reveal();
    }
}
void Link::setBoosted() { boosted = true; }

//abilities
void Link::boost() { boosted = true; }
void Link::polarize() { isData = !isData; }
void Link::reveal() { revealed = true; }
