#include "ability.h"

string Ability::getName() const { return ability; }
bool Ability::isUsed() const{ return used; }
void Ability::setUsed() { used = true; }

//Assumed that all input valid rn
bool LinkBoost::activate(Player& player, Board& board, vector<string> params) {
    char link = params[0][0];
    if (islower(link)) {
        board.getLink(link).setBoosted();
    }
}

