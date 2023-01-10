#pragma once
#include <imgui.h>
#include <string>

class PluginGUI {
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