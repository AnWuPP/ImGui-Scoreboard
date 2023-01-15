#pragma once

#include <string>
#include <map>
#include <memory>

struct PlayerEntity {
    uint16_t id;
    std::string name;
    uint32_t color;
    uint16_t score;
    uint16_t ping;
    PlayerEntity() = default;
    PlayerEntity(uint16_t _id, std::string _name, uint32_t _color, uint16_t _score, uint16_t _ping);
};

class PlayerList {
    std::map<uint16_t, PlayerEntity> mPlayers;
    PlayerEntity invalidPlayer;
public:
    PlayerList();
    PlayerEntity& AddPlayer(uint16_t id, std::string name, uint32_t color, uint16_t score, uint16_t ping);
    PlayerEntity& GetPlayer(uint16_t id);
    PlayerEntity& operator[] (uint16_t id);
    bool IsPlayerExists(uint16_t id);
    void RemovePlayer(uint16_t id);
    std::map<uint16_t, PlayerEntity>::iterator begin();
    std::map<uint16_t, PlayerEntity>::iterator end();
    size_t Count() const;
    void ClearList();
};