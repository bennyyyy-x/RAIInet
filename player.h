#ifndef PLAYER_H_
#define PLAYER_H_

#include <vector>
#include <string>
#include "ability.h"
#include "link.h"

using namespace std;

class Player {
    vector<Ability> cards;
    int data, virus;

public:
    Player(string abilities);
    void printAbility();
    void download(const Link& link);
    int getData() const;
    int getVirus() const;
};

#endif // PLAYER_H_
