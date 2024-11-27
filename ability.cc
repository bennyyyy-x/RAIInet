#include "ability.h"
#include "board.h"
#include "constants.h"

AbilityName Ability::getName() const { return name; }
bool Ability::isUsed() const{ return used; }
void Ability::setUsed() { used = true; }

//Assumed that all input valid rn
bool LinkBoost::activate(Player& player, Board& board, vector<string> params) {
    char link = params[0][0];
    if (board.getLink(link).downloadStatus() != NotDownloaded) {
        board.getLink(link).setBoosted();
        return true;
    }
    return false;
}



