#pragma once

#include "PlayerList.h"
#include <imgui.h>
#include <string>
#include <vector>

class PluginGUI {
public:
    PluginGUI(PlayerList& _playerList);
    bool mainWindow;
    void init();
    void process();
private:
    PlayerList& playerList;
    void drawHeader();
    void drawBox(int id, std::string nick, int score, int ping, ImU32 color);
    bool moveLeftAtScroll;
};