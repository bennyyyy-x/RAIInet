#ifndef PLAYER_H_
#define PLAYER_H_

#include <vector>
#include "ability.h"

using namespace std;

class Player {
    vector<Ability> cards;

public:
    void download(const Link &l);
};

#endif // PLAYER_H_
