#ifndef ABILITY_H_
#define ABILITY_H_

#include "constants.h"
#include <vector>
#include <string>

using namespace std;

// Forward Declaration
class Player;
class Board;

class Ability {
    bool used;
    AbilityName name;
public:
    virtual bool activate(Player& player, Board& board, vector<string> params) = 0;
    AbilityName getName() const;
    bool isUsed() const;
    void setUsed();
};

class LinkBoost : public Ability {
public:
    //TODO: ctor
    bool activate(Player& player, Board& board, vector<string> params) override;
};

#endif // ABILITY_H_
