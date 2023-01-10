#include "PluginGUI.h"
#include <d3d9.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include <sampapi/CNetGame.h>
#include <format>
#include <vector>

namespace samp = sampapi::v037r1;

D3DCOLOR ARGB2ABGR(D3DCOLOR color) {
    return ((color >> 24) << 24) |          // Alpha
            ((color >> 16) & 0xFF) |         // Red  -> Blue
            ((color >> 8) & 0xFF) << 8 |     // Green
            ((color) & 0xFF) << 16;          // Blue -> Red
}

PluginGUI::PluginGUI() : mainWindow(false), moveLeftAtScroll(false) { }

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

    auto textOnline = std::format("Online: {}", samp::RefNetGame()->GetPlayerPool()->GetCount(true));
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
    auto windowSize = ImGui::GetIO().DisplaySize;
    auto maxPlayers = samp::RefNetGame()->GetPlayerPool()->GetCount(true);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(windowSize);
    ImGui::Begin("##main", &mainWindow, ImGuiWindowFlags_NoTitleBar |
                                        ImGuiWindowFlags_NoMove |
                                        ImGuiWindowFlags_NoResize |
                                        ImGuiWindowFlags_NoBringToFrontOnFocus);
    auto width = windowSize.x / 2.7f;
    auto height = 30.f;
    ImGui::SetCursorScreenPos(ImVec2(windowSize.x / 2.f - width / 2.f, 70.f));
    drawHeader();
    ImGui::BeginChild("##scrollArea", ImVec2(width, windowSize.y - 120.f - height));
    drawBox(samp::RefNetGame()->GetPlayerPool()->m_localInfo.m_nId,
        samp::RefNetGame()->GetPlayerPool()->m_localInfo.m_szName,
        samp::RefNetGame()->GetPlayerPool()->GetLocalPlayerScore(),
        samp::RefNetGame()->GetPlayerPool()->GetLocalPlayerPing(),
        ARGB2ABGR(samp::RefNetGame()->GetPlayerPool()->m_localInfo.m_pObject->GetColorAsARGB())
        );
    for (int i = 0; i < maxPlayers; ++i) {
        if (samp::RefNetGame()->GetPlayerPool()->IsDisconnected(i))
            continue;
        drawBox(i,
            samp::RefNetGame()->GetPlayerPool()->GetName(i),
            samp::RefNetGame()->GetPlayerPool()->GetScore(i),
            samp::RefNetGame()->GetPlayerPool()->GetPing(i),
            ARGB2ABGR(samp::RefNetGame()->GetPlayerPool()->GetPlayer(i)->GetColorAsARGB())
            );
    }
    ImGui::EndChild();
    ImGui::End();
}