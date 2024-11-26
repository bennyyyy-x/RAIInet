#ifndef PLAYER_H_
#define PLAYER_H_

#include <vector>
#include "ability.h"
#include "link.h"

using namespace std;

class Player {
    vector<Ability> cards;

public:
    void printAbility();
};

#endif // PLAYER_H_
