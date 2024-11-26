#include "player.h"

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