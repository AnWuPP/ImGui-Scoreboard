#include "PluginRPC.h"
#include <d3d9.h>
#include <sampapi/CChat.h>
#include <string>
#include <format>

namespace samp = sampapi::v037r1;

D3DCOLOR ARGB2ABGR(D3DCOLOR color);
D3DCOLOR RGBA2ABGR(D3DCOLOR color) {
    return ((color >> 24) & 0xFF) |
            ((color >> 16) & 0xFF) << 8 |
            ((color >> 8) & 0xFF) << 16 |
            ((color & 0xFF) | 0xFF) << 24;
}

template <typename T>
std::string readWithSize(RakNet::BitStream& bs) {
    T size;
    if (!bs.Read(size) || size <= 0)
        return {};
    std::string str(size, '\0');
    bs.Read(str.data(), size);
    return str;
}

template <typename T>
void writeWithSize(RakNet::BitStream& bs, std::string_view str) {
    T size = static_cast<T>(str.size());
    bs.Write(size);
    bs.Write(str.data(), size);
}

PluginRPC::PluginRPC(PlayerList& _playerList) : playerList(_playerList) { }

bool PluginRPC::onJoinClient(unsigned char& _id, RakNet::BitStream* bs) {
    if (_id != 137)
        return true;

    uint16_t id;
    uint32_t color;
    char isNpc;
    std::string nickname;

    bs->Read(id);
    bs->Read(color);
    bs->Read(isNpc);
    nickname = readWithSize<char>(*bs);
    if (!isNpc && !playerList.IsPlayerExists(id)) {
        playerList.AddPlayer(id, nickname, RGBA2ABGR(color), 0, 0);
    }
    return true;
}

bool PluginRPC::onQuitClient(unsigned char& _id, RakNet::BitStream* bs) {
    if (_id != 138)
        return true;

    uint16_t id;
    char reason;

    bs->Read(id);
    bs->Read(reason);
    if (playerList.IsPlayerExists(id)) {
        playerList.RemovePlayer(id);
    }
    return true;
}

bool PluginRPC::onSetPlayerColor(unsigned char& _id, RakNet::BitStream* bs) {
    if (_id != 72)
        return true;

    uint16_t id;
    uint32_t color;

    bs->Read(id);
    bs->Read(color);
    if (playerList.IsPlayerExists(id)) {
        playerList.GetPlayer(id).color = RGBA2ABGR(color);
    }
    return true;
}

bool PluginRPC::onSetPlayerName(unsigned char& _id, RakNet::BitStream* bs) {
    if (_id != 11)
        return true;

    uint16_t id;
    std::string newname;
    char success;

    bs->Read(id);
    newname = readWithSize<char>(*bs);
    bs->Read(success);
    if (playerList.IsPlayerExists(id)) {
        playerList.GetPlayer(id).name = newname;
    }
    return true;
}

bool PluginRPC::onUpdateScoresAndPings(unsigned char& _id, RakNet::BitStream* bs) {
    if (_id != 155)
        return true;

    for(uint16_t i = 0, ie = bs->GetNumberOfBytesUsed() / 10; i != ie; ++i) {
        uint16_t id;
        uint32_t score;
        uint32_t ping;
        bs->Read(id);
        bs->Read(score);
        bs->Read(ping);
        if (playerList.IsPlayerExists(id)) {
            auto& player = playerList.GetPlayer(id);
            player.score = score;
            player.ping = ping;
        }
    }
    return true;
}