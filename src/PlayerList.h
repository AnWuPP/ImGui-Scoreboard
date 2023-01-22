#pragma once

#include <string>
#include <set>
#include <memory>

class PlayerList {
  public:
    void AddPlayer(uint16_t id);
    bool IsPlayerExists(uint16_t id);
    void RemovePlayer(uint16_t id);
    size_t Count() const;
    void ClearList();
    std::set<uint16_t>::iterator begin();
    std::set<uint16_t>::iterator end();
  private:
    std::set<uint16_t> player_list_;
};