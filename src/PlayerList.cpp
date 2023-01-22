#include "PlayerList.h"
void PlayerList::AddPlayer(uint16_t id) {
    player_list_.insert(id);
}

bool PlayerList::IsPlayerExists(uint16_t id) {
    return player_list_.find(id) != player_list_.end();
}

void PlayerList::RemovePlayer(uint16_t id) {
    if (IsPlayerExists(id)) {
        player_list_.erase(id);
    }
}

std::set<uint16_t>::iterator PlayerList::begin() {
    return player_list_.begin();
}

std::set<uint16_t>::iterator PlayerList::end() {
    return player_list_.end();
}

size_t PlayerList::Count() const {
    return player_list_.size();
}

void PlayerList::ClearList() {
    player_list_.clear();
}