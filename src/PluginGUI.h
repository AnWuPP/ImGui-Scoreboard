#pragma once
#include <imgui.h>
#include <string>
#include <utility>
#include <vector>
#include <cstdint>
#include <chrono>
#include <d3d9.h>

class PluginGUI {
private:
  struct PlayerEntry {
    uint16_t id;
    std::string name;
    uint16_t ping;
    uint16_t score;
    D3DCOLOR color;

    PlayerEntry(const uint16_t id, const std::string name, const uint16_t score, const uint16_t ping, const D3DCOLOR color)
        : id(id), name(name), ping(ping), score(score), color(color) {};
  };
  std::vector<PlayerEntry> entries{};
public:
  PluginGUI();
  bool mainWindow;
  void init();
  void process();
private:
  void drawHeader();
  void drawBox(int id, std::string nick, int score, int ping, ImU32 color);
  bool moveLeftAtScroll;
};