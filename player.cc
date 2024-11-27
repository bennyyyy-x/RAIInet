#include "player.h"
#include <iostream>
#include <iomanip>

using namespace std;

Player::Player(string abilities) : data{0}, virus{0} {
    // TODO construct the abilities
}

void Player::download(const Link& link) {
    if (link.getType()) {
        data++;
    } else {
        virus++;
    }
}

int Player::getNumAbilities() const {
    return cards.size();
}

int Player::getData() const {
    return data;
}

int Player::getVirus() const {
    return virus;
}

bool Player::useAbility(int id, Board& board, vector<string> params) {
    if (cards[id].isUsed()) {
        return false;
    }
    bool success = cards[id].activate(*this, board, params);
    if (success) {
        cards[id].setUsed();
    }
    return success;
}


void Player::printAbility() const {
    cout  << setw(5) << left << "ID"  << setw(14) << left << "Ability" << "Status" << endl;
    for (int i = 0; i < 5; ++i) {
        cout << setw(5) << left << i + 1 << setw(14) << left << cards[i].getName() << (cards[i].isUsed() ? "Used" : "") << endl;
    }
}
