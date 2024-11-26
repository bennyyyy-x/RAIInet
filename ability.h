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
public:
    virtual void activate(Player& player, Board& board, vector<string> params) = 0;
};

#endif // ABILITY_H_
