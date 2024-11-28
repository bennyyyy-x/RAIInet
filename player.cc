#include "player.h"
#include "ability.h"
#include <iostream>
#include <iomanip>

using namespace std;

Player::Player(int id, string abilities) : player_id{id}, data{0}, virus{0} {
    int min_len = abilities.length() < 5 ? abilities.length() : 5;
    for (int i = 0; i < min_len; ++i) {
        char c = abilities[i];
        if (c == 'L') {
            cards.push_back(make_unique<LinkBoost>());
        } else if (c == 'F') {
            cards.push_back(make_unique<Firewall>());
        } else if (c == 'D') {
            cards.push_back(make_unique<Download>());
        } else if (c == 'P') {
            cards.push_back(make_unique<Polarize>());
        } else if (c == 'S') {
            cards.push_back(make_unique<Scan>());
        } else if (c == 'E') {
            cards.push_back(make_unique<Exchange>());
        } else if (c == 'T') {
            cards.push_back(make_unique<Teleport>());
        } else if (c == 'H') {
            cards.push_back(make_unique<Hijack>());
        }
    }
}

void Player::download(const Link& link) {
    cout << "Player id: " << player_id << " downloading link " << link.getChar() << endl;
    if (link.getIsData()) {
        data++;
    } else {
        virus++;
    }
}

int Player::getNumAbilities() const {
    int count = 0;
    for (size_t i = 0; i < cards.size(); ++i) {
        if (!cards[i]->isUsed()) {
            count++;
        }
    }
    return count;
}

int Player::getData() const { return data; }

int Player::getVirus() const { return virus; }

int Player::getPlayerId() const { return player_id; }

bool Player::useAbility(int id, Board& board, vector<string> params) {
    cout << "In useAbility, id = " << id << endl; 
    id--;
    if (cards[id]->isUsed()) {
        return false;
    }
    bool success = cards[id]->activate(*this, board, params);
    cout << "after success??" << endl;
    if (success) {
        cards[id]->setUsed();
    }
    return success;
}

string convertAbilityName(AbilityName name) {
    switch (name) {
        case AbilityName::LINKBOOST: return "Link Boost";
        case AbilityName::FIREWALL: return "Firewall";
        case AbilityName::DOWNLOAD: return "Download";
        case AbilityName::POLARIZE: return "Polarize";
        case AbilityName::SCAN: return "Scan";
        case AbilityName::EXCHANGE: return "Exchange";
        case AbilityName::TELEPORT: return "Teleport";
        case AbilityName::HIJACK: return "Hijack";
    }
    return "Error";
}

void Player::printAbility() const {
    cout << setw(5) << left << "ID"
         << setw(14) << left << "Ability"
         << "Status" << endl;
    for (int i = 0; i < 5; ++i) {
        cout << setw(5) << left << i + 1
             << setw(14) << left << convertAbilityName(cards[i]->getName())
             << (cards[i]->isUsed() ? "Used" : "") << endl;
    }
}

AbilityName Player::getAbilityName(int id) {
    return cards[id - 1]->getName();
}
