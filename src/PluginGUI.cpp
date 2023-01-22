#include "PluginGUI.h"
#include <d3d9.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include <sampapi/CNetGame.h>
#include <sampapi/CChat.h>
#include <sampapi/CGame.h>
#include <RakHook/rakhook.hpp>
#include <format>
#include <iterator>
#include <vector>

namespace samp = sampapi::v037r1;

D3DCOLOR ARGB2ABGR(D3DCOLOR color) {
    return (((color >> 24) | 0xFF) << 24) |          // Alpha
            ((color >> 16) & 0xFF) |         // Red  -> Blue
            ((color >> 8) & 0xFF) << 8 |     // Green
            ((color) & 0xFF) << 16;          // Blue -> Red
}

PluginGUI::PluginGUI(PlayerList& _playerList) : mainWindow(false), moveLeftAtScroll(false), playerList(_playerList) { }

void PluginGUI::init() {
    ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.0, 0.0, 0.0, 0.7);
    ImGui::GetStyle().WindowBorderSize = 0.f;
}

void PluginGUI::drawHeader() {
    auto windowSize = ImGui::GetIO().DisplaySize;
    auto drawList = ImGui::GetWindowDrawList();
    auto pos = ImGui::GetCursorScreenPos();

    auto width = windowSize.x / 2.7f;
    auto height = 30.f;

    auto textOnline = std::format("Online: {}", playerList.Count() + 1);
    auto toSize = ImGui::CalcTextSize(textOnline.c_str());

    drawList->AddText(ImVec2(pos.x + 4.f, pos.y - 20.f), -1, samp::RefNetGame()->m_szHostname);
    drawList->AddText(ImVec2(pos.x + width - toSize.x - 4.f, pos.y - 20.f), -1, textOnline.c_str());
    
    drawList->AddRectFilled(pos, ImVec2(pos.x + width, pos.y + height), 0xFA010101);
    ImVec2 id = ImGui::CalcTextSize("ID"), score = ImGui::CalcTextSize("SCORE"),
        ping = ImGui::CalcTextSize("PING");
    drawList->AddText(ImVec2(pos.x + 22.f - id.x / 2.f, pos.y + height / 2.f - 8.f), -1, "ID");
    drawList->AddText(ImVec2(pos.x + 44.f, pos.y + height / 2.f - 8.f), -1, "NICKNAME");
    drawList->AddText(ImVec2(pos.x + width - 150.f - score.x / 2.f - (moveLeftAtScroll? ImGui::GetStyle().ScrollbarSize + 2.f : 0.f), pos.y + height / 2.f - 8.f), -1, "SCORE");
    drawList->AddText(ImVec2(pos.x + width - 60.f - ping.x / 2.f - (moveLeftAtScroll? ImGui::GetStyle().ScrollbarSize + 2.f : 0.f), pos.y + height / 2.f - 8.f), -1, "PING");
    ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + height + 2.f));
}

void PluginGUI::drawBox(int id, std::string nick,
        int score, int ping, ImU32 color) {
    auto windowSize = ImGui::GetIO().DisplaySize;
    auto drawList = ImGui::GetWindowDrawList();
    auto pos = ImGui::GetCursorScreenPos();

    auto width = windowSize.x / 2.7f;
    moveLeftAtScroll = ImGui::GetScrollMaxY() > 0.f;
    if (moveLeftAtScroll)
        width -= ImGui::GetStyle().ScrollbarSize + 2.f;
    auto height = 24.f;
    ImVec4 col = ImGui::ColorConvertU32ToFloat4(color);
    col.w = 1.f;
    color = ImGui::GetColorU32(col);
    auto idStr = std::to_string(id);
    auto scoreStr = std::to_string(score);
    auto pingStr = std::to_string(ping);
    auto idSize = ImGui::CalcTextSize(idStr.c_str());
    auto scoreSize = ImGui::CalcTextSize(scoreStr.c_str());
    auto pingSize = ImGui::CalcTextSize(pingStr.c_str());
    bool hovered = ImGui::IsMouseHoveringRect(pos, ImVec2(pos.x + width, pos.y + height));
    
    drawList->AddRectFilled(pos, ImVec2(pos.x + width, pos.y + height), 0xcc010101);
    drawList->AddRectFilled(pos, ImVec2(pos.x + 5.5, pos.y + height),
        ImGui::ColorConvertFloat4ToU32(ImVec4(col.x, col.y, col.z, 0.8f)));

    if (hovered) {
        drawList->AddRectFilledMultiColor(ImVec2(pos.x + 5.5, pos.y),
            ImVec2(pos.x + width, pos.y + height),
            ImGui::ColorConvertFloat4ToU32(ImVec4(col.x, col.y, col.z, 0.4f)),
            ImGui::ColorConvertFloat4ToU32(ImVec4(0.f, 0.f, 0.f, 0.f)),
            ImGui::ColorConvertFloat4ToU32(ImVec4(0.f, 0.f, 0.f, 0.f)),
            ImGui::ColorConvertFloat4ToU32(ImVec4(col.x, col.y, col.z, 0.4f))
        );
        if (ImGui::IsMouseDoubleClicked(0)) {
            RakNet::BitStream rpc;
            rpc.Write(id);
            rpc.Write(0);
            rakhook::send_rpc(22, &rpc, PacketPriority::HIGH_PRIORITY, PacketReliability::UNRELIABLE_SEQUENCED, (char)0, false);

            mainWindow = false;
            samp::RefGame()->EnableHUD(true);
            samp::RefGame()->EnableRadar(true);
            samp::RefChat()->m_nMode = samp::RefChat()->DISPLAY_MODE_NORMAL;
            samp::RefGame()->SetCursorMode(samp::CURSOR_NONE, false);
        }
    }
    drawList->AddText(ImVec2(pos.x + 22.f - idSize.x / 2.f, pos.y + height / 2.f - 8.f), color, idStr.c_str());
    drawList->AddText(ImVec2(pos.x + 44.f, pos.y + height / 2.f - 8.f), color, nick.c_str());
    drawList->AddText(ImVec2(pos.x + width - 150.f - scoreSize.x / 2.f, pos.y + height / 2.f - 8.f), color, scoreStr.c_str());
    drawList->AddText(ImVec2(pos.x + width - 60.f - pingSize.x / 2.f, pos.y + height / 2.f - 8.f), color, pingStr.c_str());
    ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + height + 2.f));
}

void PluginGUI::process() {
    if (samp::RefNetGame() == nullptr || !mainWindow)
        return;
    static auto lastUpdate = std::chrono::system_clock::now();
    auto now = std::chrono::system_clock::now();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate) >= std::chrono::milliseconds(1500)) {
        samp::RefNetGame()->UpdatePlayers();
        lastUpdate = now;
    }
    auto windowSize = ImGui::GetIO().DisplaySize;
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(windowSize);
    ImGui::Begin("##main", &mainWindow, ImGuiWindowFlags_NoTitleBar |
                                        ImGuiWindowFlags_NoMove |
                                        ImGuiWindowFlags_NoResize |
                                        ImGuiWindowFlags_NoBringToFrontOnFocus |
                                        ImGuiWindowFlags_UnsavedDocument);
    auto width = windowSize.x / 2.7f;
    auto height = 30.f;
    ImGui::SetCursorScreenPos(ImVec2(windowSize.x / 2.f - width / 2.f, 70.f));
    drawHeader();
    ImGui::BeginChild("##scrollArea", ImVec2(width, windowSize.y - 120.f - height));
    auto players = samp::RefNetGame()->GetPlayerPool();
    drawBox(players->m_localInfo.m_nId,
        players->m_localInfo.m_szName,
        players->GetLocalPlayerScore(),
        players->GetLocalPlayerPing(),
        ARGB2ABGR(players->m_localInfo.m_pObject->GetColorAsARGB())
        );
    ImGuiListClipper clipper;
    clipper.Begin(playerList.Count());
    while (clipper.Step()) {
        auto it = std::next(playerList.begin(), clipper.DisplayStart);
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i, ++it) {
            auto id = *it;
            auto players = samp::RefNetGame()->GetPlayerPool();
            drawBox(id,
                players->GetName(id),
                players->GetScore(id),
                players->GetPing(id),
                ARGB2ABGR(players->GetPlayer(id)->GetColorAsARGB())
                );
        }
    }
    ImGui::EndChild();
    ImGui::End();
}