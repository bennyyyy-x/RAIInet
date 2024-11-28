#include "player.h"
#include <iostream>
#include <iomanip>

using namespace std;

Player::Player(int id, string abilities) : player_id{id}, data{0}, virus{0} {
    // TODO construct the abilities
}

void Player::download(const Link& link) {
    cout << "Player id: " << player_id << " downloading link " << link.getChar() << endl;
    if (link.getIsData()) {
        data++;
    } else {
        virus++;
    }
}

int Player::getNumAbilities() const { return cards.size(); }

int Player::getData() const { return data; }

int Player::getVirus() const { return virus; }

int Player::getPlayerId() const { return player_id; }

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
    cout << setw(5) << left << "ID"
         << setw(14) << left << "Ability"
         << "Status" << endl;
    for (int i = 0; i < 5; ++i) {
        cout << setw(5) << left << i + 1
             << setw(14) << left << cards[i].getName()
             << (cards[i].isUsed() ? "Used" : "") << endl;
    }
}

AbilityName Player::getAbilityName(int id) {
    return cards[id].getName();
}
