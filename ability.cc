#include "ability.h"

string Ability::getName() const { return ability; }
bool Ability::isUsed() const{ return used; }

void LinkBoost::activate(Player& player, Board& board, vector<string> params) {
    
}

