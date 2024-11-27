#ifndef ABILITY_H_
#define ABILITY_H_

#include <vector>
#include <string>

using namespace std;

// Forward Declaration
class Player;
class Board;

class Ability {
    bool used;
    string ability;
public:
    virtual void activate(Player& player, Board& board, vector<string> params) = 0;
    string getName() const;
    bool isUsed() const;
};

class LinkBoost : public Ability {
public:
    //TODO: ctor
    void activate(Player& player, Board& board, vector<string> params) override;
};

#endif // ABILITY_H_
