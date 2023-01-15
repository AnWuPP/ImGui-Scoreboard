#include "Plugin.h"
#include <sampapi/CChat.h>
#include <RakHook/rakhook.hpp>
#include <RakNet/StringCompressor.h>

namespace samp = sampapi::v037r1;

Plugin::Plugin(HMODULE hndl) : hModule(hndl), inited(false), rpc(playerList), render(playerList) {
    using namespace std::placeholders;
    hookCTimerUpdate.set_cb(std::bind(&Plugin::mainloop, this, _1));
    hookCTimerUpdate.install();
}

void Plugin::mainloop(const decltype(hookCTimerUpdate)& hook) {
    if (!inited && samp::RefChat() != nullptr && rakhook::initialize()) {
        using namespace std::placeholders;
        samp::RefChat()->AddMessage(0x6495EDFF, "Scoreboard{FFFFFF} loaded | Author: {6495ED}AnWu{FFFFFF} | https://blast.hk");
        StringCompressor::AddReference();
        rakhook::on_receive_rpc += std::bind(&PluginRPC::onJoinClient, &rpc, _1, _2);
        rakhook::on_receive_rpc += std::bind(&PluginRPC::onQuitClient, &rpc, _1, _2);
        rakhook::on_receive_rpc += std::bind(&PluginRPC::onSetPlayerColor, &rpc, _1, _2);
        rakhook::on_receive_rpc += std::bind(&PluginRPC::onSetPlayerName, &rpc, _1, _2);
        rakhook::on_receive_rpc += std::bind(&PluginRPC::onUpdateScoresAndPings, &rpc, _1, _2);
        rakhook::on_receive_packet += std::bind(&PluginRPC::onConnectionAccept, &rpc, _1);
        inited = true;
    }
    return hook.get_trampoline()();
}
