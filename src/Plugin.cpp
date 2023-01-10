#include "Plugin.h"
#include <sampapi/CChat.h>

namespace samp = sampapi::v037r1;

Plugin::Plugin(HMODULE hndl) : hModule(hndl), inited(false) {
    using namespace std::placeholders;
    hookCTimerUpdate.set_cb(std::bind(&Plugin::mainloop, this, _1));
    hookCTimerUpdate.install();
}

void Plugin::mainloop(const decltype(hookCTimerUpdate)& hook) {
    if (!inited && samp::RefChat() != nullptr) {
        samp::RefChat()->AddMessage(0x6495EDFF, "Scoreboard{FFFFFF} loaded | Author: {6495ED}AnWu{FFFFFF} | https://blast.hk");
        inited = true;
    }
    return hook.get_trampoline()();
}
