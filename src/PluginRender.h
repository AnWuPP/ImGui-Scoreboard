#pragma once

#include "PluginGUI.h"
#include "PlayerList.h"
#include <kthook/kthook.hpp>
#include <d3d9.h>
#include <memory>

// Сигнатуры функций
using PresentSignature = HRESULT(__stdcall*)(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
using ResetSignature = HRESULT(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);

class PluginRender {
    PlayerList& playerList;
    bool ImGuiinited;
    PluginGUI GUI;

    std::uintptr_t findDevice(std::uint32_t Len);
    void* getFunctionAddress(int VTableIndex);

    kthook::kthook_signal<PresentSignature> hookPresent{};
    kthook::kthook_signal<ResetSignature> hookReset{};
    std::optional<HRESULT> onPresent(const decltype(hookPresent)& hook, IDirect3DDevice9* pDevice, const RECT*, const RECT*, HWND, const RGNDATA*);
    std::optional<HRESULT> onLost(const decltype(hookReset)& hook, IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* parameters);
    void onReset(const decltype(hookReset)& hook, HRESULT& returnValue, IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* parameters);

    kthook::kthook_simple<WNDPROC> hookWndproc{};
    HRESULT __stdcall onWndproc(const decltype(hookWndproc)& hook, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
    PluginRender(PlayerList& _playerList);
    ~PluginRender();
};