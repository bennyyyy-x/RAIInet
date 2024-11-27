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
    bool activate(Player& player, Board& board, vector<string> params) override;
};

class Firewall : public Ability {
public:
    bool activate(Player& player, Board& board, vector<string> params) override;
};

class Download : public Ability {
public:
    bool activate(Player& player, Board& board, vector<string> params) override;
};

class Polarize : public Ability {
public:
    bool activate(Player& player, Board& board, vector<string> params) override;
};

class Scan : public Ability {
public:
    bool activate(Player& player, Board& board, vector<string> params) override;
};

class Exchange : public Ability {
public:
    bool activate(Player& player, Board& board, vector<string> params) override;
};

class Teleport : public Ability {
public:
    bool activate(Player& player, Board& board, vector<string> params) override;
};

class Hijack : public Ability {
public:
    bool activate(Player& player, Board& board, vector<string> params) override;
};

#endif // ABILITY_H_
