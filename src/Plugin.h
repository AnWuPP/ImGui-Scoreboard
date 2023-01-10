#pragma once

#include "PluginRender.h"
#include <kthook/kthook.hpp>

using CTimerProto = void( __cdecl* )();

class Plugin {
public:
    Plugin(HMODULE hndl);
    HMODULE hModule;
private:
    bool inited;
    PluginRender render;
    kthook::kthook_simple<CTimerProto> hookCTimerUpdate{ reinterpret_cast<void*>(0x561B10) };
    void mainloop(const decltype(hookCTimerUpdate)& hook);
};