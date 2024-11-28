#ifndef PLAYER_H_
#define PLAYER_H_

#include <vector>
#include <string>
#include "ability.h"
#include "link.h"
#include "constants.h"

using namespace std;

class Player {
    int player_id;
    vector<Ability> cards;
    int data, virus;

public:
    Player(int id, string abilities);
    void printAbility() const;
    void download(const Link& link);
    int getNumAbilities() const;
    int getData() const;
    int getVirus() const;
    int getPlayerId() const;
    bool useAbility(int id, Board& board, vector<string> params);
    AbilityName getAbilityName(int id);
};

#endif // PLAYER_H_
