#include "PlayerList.h"

PlayerEntity::PlayerEntity(uint16_t _id, std::string _name, uint32_t _color, uint16_t _score, uint16_t _ping)
    : id(_id), name(_name), color(_color), score(_score), ping(_ping) { }

PlayerList::PlayerList() : invalidPlayer(65535, "", -1, 65535, 65535) { }

PlayerEntity& PlayerList::AddPlayer(uint16_t id, std::string name, uint32_t color, uint16_t score, uint16_t ping) {
    return mPlayers[id] = { id, name, color, score, ping };
}

bool PlayerList::IsPlayerExists(uint16_t id) {
    return mPlayers.find(id) != mPlayers.end();
}

PlayerEntity& PlayerList::GetPlayer(uint16_t id) {
    if (IsPlayerExists(id)) {
        return mPlayers[id];
    }
    return invalidPlayer;
}

void PlayerList::RemovePlayer(uint16_t id) {
    if (IsPlayerExists(id)) {
        mPlayers.erase(id);
    }
}

PlayerEntity& PlayerList::operator[] (uint16_t id) {
    return GetPlayer(id);
}

std::map<uint16_t, PlayerEntity>::iterator PlayerList::begin() {
    return mPlayers.begin();
}

std::map<uint16_t, PlayerEntity>::iterator PlayerList::end() {
    return mPlayers.end();
}

size_t PlayerList::Count() const {
    return mPlayers.size();
}

void PlayerList::ClearList() {
    mPlayers.clear();
}