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
    Ability(AbilityName name);
    virtual bool activate(Player& player, Board& board, vector<string> params) = 0;
    AbilityName getName() const;
    bool isUsed() const;
    void setUsed();
};

class LinkBoost : public Ability {
public:
    LinkBoost();
    bool activate(Player& player, Board& board, vector<string> params) override;
};

class Firewall : public Ability {
public:
    Firewall();
    bool activate(Player& player, Board& board, vector<string> params) override;
};

class Download : public Ability {
public:
    Download();
    bool activate(Player& player, Board& board, vector<string> params) override;
};

class Polarize : public Ability {
public:
    Polarize();
    bool activate(Player& player, Board& board, vector<string> params) override;
};

class Scan : public Ability {
public:
    Scan();
    bool activate(Player& player, Board& board, vector<string> params) override;
};

class Exchange : public Ability {
public:
    Exchange();
    bool activate(Player& player, Board& board, vector<string> params) override;
};

class Teleport : public Ability {
public:
    Teleport();
    bool activate(Player& player, Board& board, vector<string> params) override;
};

class Hijack : public Ability {
public:
    Hijack();
    bool activate(Player& player, Board& board, vector<string> params) override;
};

#endif // ABILITY_H_
